// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraSpringArmComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "PhysicsEngine/PhysicsSettings.h"

void UCameraSpringArmComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		CapsuleComponent = Character->GetCapsuleComponent();
	}
}

void UCameraSpringArmComponent::UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime)
{
	//Calculate previous relative camera location
	FRotator PreviousRot = (PreviousArmOrigin - LastResultLoc).GetSafeNormal().Rotation();
	FVector PreviousCamLoc = PreviousRot.UnrotateVector(LastResultLoc - PreviousArmOrigin);

	//default USpringArmComponent start
	
	FRotator DesiredRot = GetTargetRotation();

	// If our viewtarget is simulating using physics, we may need to clamp deltatime
	if (bClampToMaxPhysicsDeltaTime)
	{
		// Use the same max timestep cap as the physics system to avoid camera jitter when the viewtarget simulates less time than the camera
		DeltaTime = FMath::Min(DeltaTime, UPhysicsSettings::Get()->MaxPhysicsDeltaTime);
	}

	// Apply 'lag' to rotation if desired
	if(bDoRotationLag)
	{
		if (bUseCameraLagSubstepping && DeltaTime > CameraLagMaxTimeStep && CameraRotationLagSpeed > 0.f)
		{
			const FRotator ArmRotStep = (DesiredRot - PreviousDesiredRot).GetNormalized() * (1.f / DeltaTime);
			FRotator LerpTarget = PreviousDesiredRot;
			float RemainingTime = DeltaTime;
			while (RemainingTime > UE_KINDA_SMALL_NUMBER)
			{
				const float LerpAmount = FMath::Min(CameraLagMaxTimeStep, RemainingTime);
				LerpTarget += ArmRotStep * LerpAmount;
				RemainingTime -= LerpAmount;

				DesiredRot = FRotator(FMath::QInterpTo(FQuat(PreviousDesiredRot), FQuat(LerpTarget), LerpAmount, CameraRotationLagSpeed));
				PreviousDesiredRot = DesiredRot;
			}
		}
		else
		{
			DesiredRot = FRotator(FMath::QInterpTo(FQuat(PreviousDesiredRot), FQuat(DesiredRot), DeltaTime, CameraRotationLagSpeed));
		}
	}
	
	PreviousDesiredRot = DesiredRot;

	// Get the spring arm 'origin', the target we want to look at
	FVector ArmOrigin = GetComponentLocation() + TargetOffset;
	// We lag the target, not the actual camera position, so rotating the camera around does not have lag
	FVector DesiredLoc = ArmOrigin;
	
	if (bDoLocationLag)
	{
		if (bUseCameraLagSubstepping && DeltaTime > CameraLagMaxTimeStep && CameraLagSpeed > 0.f)
		{
			const FVector ArmMovementStep = (DesiredLoc - PreviousDesiredLoc) * (1.f / DeltaTime);
			FVector LerpTarget = PreviousDesiredLoc;

			float RemainingTime = DeltaTime;
			while (RemainingTime > UE_KINDA_SMALL_NUMBER)
			{
				const float LerpAmount = FMath::Min(CameraLagMaxTimeStep, RemainingTime);
				LerpTarget += ArmMovementStep * LerpAmount;
				RemainingTime -= LerpAmount;

				DesiredLoc = FMath::VInterpTo(PreviousDesiredLoc, LerpTarget, LerpAmount, CameraLagSpeed);
				PreviousDesiredLoc = DesiredLoc;
			}
		}
		else
		{
			DesiredLoc = FMath::VInterpTo(PreviousDesiredLoc, DesiredLoc, DeltaTime, CameraLagSpeed);
		}

		// Clamp distance if requested
		bool bClampedDist = false;
		if (CameraLagMaxDistance > 0.f)
		{
			const FVector FromOrigin = DesiredLoc - ArmOrigin;
			if (FromOrigin.SizeSquared() > FMath::Square(CameraLagMaxDistance))
			{
				DesiredLoc = ArmOrigin + FromOrigin.GetClampedToMaxSize(CameraLagMaxDistance);
				bClampedDist = true;
			}
		}		

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		if (bDrawDebugLagMarkers)
		{
			DrawDebugSphere(GetWorld(), ArmOrigin, 5.f, 8, FColor::Green);
			DrawDebugSphere(GetWorld(), DesiredLoc, 5.f, 8, FColor::Yellow);

			const FVector ToOrigin = ArmOrigin - DesiredLoc;
			DrawDebugDirectionalArrow(GetWorld(), DesiredLoc, DesiredLoc + ToOrigin * 0.5f, 7.5f, bClampedDist ? FColor::Red : FColor::Green);
			DrawDebugDirectionalArrow(GetWorld(), DesiredLoc + ToOrigin * 0.5f, ArmOrigin,  7.5f, bClampedDist ? FColor::Red : FColor::Green);
		}
#endif
	}

	PreviousArmOrigin = ArmOrigin;
	PreviousDesiredLoc = DesiredLoc;

	// Now offset camera position back along our rotation
	DesiredLoc -= DesiredRot.Vector() * TargetArmLength;
	// Add socket offset in local space
	DesiredLoc += FRotationMatrix(DesiredRot).TransformVector(SocketOffset);
	
	//default USpringArmComponent end

	// Do a sweep to ensure we are not penetrating the world
	FVector ResultLoc = DesiredLoc;
	if (bDoTrace && (TargetArmLength != 0.0f))
	{
		
		if(!bUseNewCamera) //default USpringArmComponent logic
		{
			bIsCameraFixed = true;
			FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(SpringArm), false, GetOwner());

			FHitResult Result;
			GetWorld()->SweepSingleByChannel(Result, ArmOrigin, DesiredLoc, FQuat::Identity, ProbeChannel, FCollisionShape::MakeSphere(ProbeSize), QueryParams);
		
			UnfixedCameraPosition = DesiredLoc;

			ResultLoc = BlendLocations(DesiredLoc, Result.Location, Result.bBlockingHit, DeltaTime);

			if (ResultLoc == DesiredLoc) 
			{	
				bIsCameraFixed = false;
			}
		}
		else if(CapsuleComponent != nullptr)
		{
			bIsCameraFixed = true;

			ResultLoc = DesiredLoc;
			
			bInSight = false;
			int TraceCount = 0;
			
			FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(SpringArm), bTraceComplex, GetOwner());
			QueryParams.bIgnoreTouches = true;
			FHitResult SightResult;
			FHitResult TraceResult;
			
			const FVector TraceStart = DesiredLoc;
			const FVector TraceEnd = ArmOrigin;
			const FVector TraceDir = (TraceEnd - TraceStart).GetSafeNormal();

			const FVector PlayerLoc = CapsuleComponent->GetComponentLocation();
			const FVector TopLoc = PlayerLoc + (CapsuleComponent->GetScaledCapsuleHalfHeight())*FVector::UpVector;
			const FVector BottomLoc = PlayerLoc - (CapsuleComponent->GetScaledCapsuleHalfHeight())*FVector::UpVector;

			FVector PossibleLoc = TraceStart;

			while (TraceCount < 100)
			{
				TraceCount++;
				
				// Checks if camera can see top, middle or bottom of player
				if(!GetWorld()->LineTraceSingleByChannel(SightResult, PossibleLoc, PlayerLoc, ProbeChannel, QueryParams)
						|| !GetWorld()->LineTraceSingleByChannel(SightResult, PossibleLoc, TopLoc, ProbeChannel, QueryParams)
						|| !GetWorld()->LineTraceSingleByChannel(SightResult, PossibleLoc, BottomLoc, ProbeChannel, QueryParams))
				{
					bInSight = true;
				}

				// Check if collision between camera and player
				if(!bInSight && GetWorld()->LineTraceSingleByChannel(TraceResult, PossibleLoc, TraceEnd, ProbeChannel, QueryParams))
				{
					//move possible loc forward with a small offset
					PossibleLoc = TraceResult.Location + 10.f*TraceDir;
				}
				else bInSight = true;
					
				if(bInSight)
				{
					//Sweep possible location to see if there is slight clipping
					if(GetWorld()->SweepSingleByChannel(TraceResult, PossibleLoc, PossibleLoc+TraceDir, FQuat::Identity, ProbeChannel, FCollisionShape::MakeSphere(CollisionRadius),QueryParams))
					{
						// Using distance to impact push possible location forward to move out of clipping location
						float ImpactDist = ((TraceResult.ImpactPoint + TraceResult.ImpactNormal*CollisionRadius) - PossibleLoc).Length();
						PossibleLoc += TraceDir*ImpactDist;
					}					
						
					ResultLoc = PossibleLoc;
					break;
				}				
			}

			UnfixedCameraPosition = DesiredLoc;

			if (ResultLoc == DesiredLoc) 
			{
				bIsCameraFixed = false;
			}

			//Transform camera location to relative to interp forward separately
			FRotator TargetRot = TraceDir.Rotation();
			
			FVector RelativeLoc = ResultLoc - ArmOrigin;
			RelativeLoc = TargetRot.UnrotateVector(RelativeLoc);
				
			RelativeLoc.X = BlendCameraLocation(PreviousCamLoc.X, RelativeLoc.X, DeltaTime);
			RelativeLoc.Y = FMath::FInterpTo(PreviousCamLoc.Y, RelativeLoc.Y, DeltaTime, CollisionYZSpeed);
			RelativeLoc.Z = FMath::FInterpTo(PreviousCamLoc.Z, RelativeLoc.Z, DeltaTime, CollisionYZSpeed);
				
			ResultLoc = TargetRot.RotateVector(RelativeLoc) + ArmOrigin;
		}
	}
	else
	{
		ResultLoc = DesiredLoc;
		bIsCameraFixed = false;
		UnfixedCameraPosition = ResultLoc;
	}
	
	LastResultLoc = ResultLoc;
	
	// Form a transform for new world transform for camera
	FTransform WorldCamTM(DesiredRot, ResultLoc);
	// Convert to relative to component
	FTransform RelCamTM = WorldCamTM.GetRelativeTransform(GetComponentTransform());

	// Update socket location/rotation
	RelativeSocketLocation = RelCamTM.GetLocation();
	RelativeSocketRotation = RelCamTM.GetRotation();

	UpdateChildTransforms();
}

float UCameraSpringArmComponent::BlendCameraLocation(const float CurrentDist, const float TargetDist, float DeltaTime) const
{
	//use different interp speed depending on travel direction 
	const float TravelSign = (FMath::Abs(TargetDist) - FMath::Abs(CurrentDist));

	if(TravelSign < 0.f)
	{
		return FMath::FInterpTo(CurrentDist, TargetDist, DeltaTime, CollisionZoomInSpeed);
	}
	else return FMath::FInterpTo(CurrentDist, TargetDist, DeltaTime, CollisionZoomOutSpeed);

}

FVector UCameraSpringArmComponent::BlendLocations(const FVector& DesiredArmLocation, const FVector& TraceHitLocation,
                                                  bool bHitSomething, float DeltaTime)
{
	return Super::BlendLocations(DesiredArmLocation, TraceHitLocation, bHitSomething, DeltaTime);
}