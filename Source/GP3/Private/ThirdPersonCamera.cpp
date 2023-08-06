#include "ThirdPersonCamera.h"

#include "CameraSpringArmComponent.h"
#include "MathUtil.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

UThirdPersonCamera::UThirdPersonCamera()
{
	CameraBoom = CreateDefaultSubobject<UCameraSpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, UCameraSpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	RotationOffset = FRotator(-15.f, 0.f, 0.f);
	DefaultRelativeOffset = FVector::ZeroVector;
	DefaultDistance = 500.f;
	RestingDuration = 2.f;
	DefaultAutoLocationSpeed = 2.f;

	RestingTimer = 0;
	
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
	CameraBoom->TargetArmLength = DefaultDistance;
	CameraBoom->SocketOffset = DefaultRelativeOffset;
	CameraBoom->SetRelativeRotation(RotationOffset);
}

void UThirdPersonCamera::SetupAttachment(USceneComponent* InParent, const FName InSocketName)
{
	CameraBoom->SetupAttachment(InParent, InSocketName);
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
}

void UThirdPersonCamera::BeginPlay()
{
	Super::BeginPlay();

	PawnOwner = Cast<APawn>(GetOwner());

	ResetZoomModeValues();
	DistanceMultiplier = 1.f;
	bZoomMode = false;
	TransitionRunMode(0);
}

void UThirdPersonCamera::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!PawnOwner) return;

	if(bSnapCamera)
	{
		SnapCameraLoop(DeltaTime);
	}
	else
	{
		if(RestingTimer < RestingDuration)
		{
			RestingTimer += DeltaTime;
		}
	}

	if(!bZoomMode)
	{
		FollowCamera->FieldOfView = RunModeFOV;
		CurrentDistance = RunModeDistance;
		CameraBoom->SocketOffset = RunModeRelativeOffset*DistanceMultiplier;
		CameraBoom->TargetOffset = RunModeWorldOffset;
		CameraBoom->CameraLagSpeed = RunModeLocLagSpeed;
	}
	else
	{
		ZoomModeTransitionLoop(DeltaTime);
	}

	if(bAiming)
	{
		AimCameraLoop(DeltaTime);
	}

	CameraBoom->TargetArmLength = FMath::Max(CurrentDistance*DistanceMultiplier, MinDistance);
}

void UThirdPersonCamera::SwivelCamera(float DeltaYaw)
{
	if(!IsResting() || bLockRotationInput) return;
	if (PawnOwner && PawnOwner->IsPawnControlled())
	{
		FRotator CurrentRotation = PawnOwner->GetControlRotation();

		CurrentRotation.Yaw += DeltaYaw*SwivelSpeed;
		
		PawnOwner->GetController()->SetControlRotation(CurrentRotation);
	}
}

void UThirdPersonCamera::TurnCamera(const float Yaw, const float Pitch)
{
	if(bLockRotationInput) return;
	
	if (PawnOwner && PawnOwner->IsPawnControlled())
	{
		PawnOwner->AddControllerYawInput(Yaw);
		PawnOwner->AddControllerPitchInput(Pitch);

		if(FMathf::Abs(Yaw) > .1f || FMathf::Abs(Pitch) > .1f) RestingTimer = 0;
	}
}

void UThirdPersonCamera::SnapCamera()
{
	if(bLockRotationInput) return;
	bSnapCamera = true;
}

void UThirdPersonCamera::SnapCameraLoop(float DeltaTime)
{
	if (PawnOwner && PawnOwner->IsPawnControlled())
	{
		FRotator CurrentRotation = PawnOwner->GetControlRotation();
		FRotator TargetRotation = GetOwner()->GetActorRotation();
		// added offset added to prevent interp locking at 180 degree distance
		TargetRotation += RotationOffset + FRotator(0,.01f, 0);
		TargetRotation.Normalize();

		CurrentRotation = FMath::QInterpTo(CurrentRotation.Quaternion(), TargetRotation.Quaternion(), DeltaTime, SnapRotationSpeed).Rotator();
		CurrentRotation.Roll = 0;
		
		if(CurrentRotation.GetManhattanDistance(TargetRotation) < 1.f)
		{
			bSnapCamera = false;
			RestCamera();
			
			CurrentRotation = TargetRotation;
		}
			
		PawnOwner->GetController()->SetControlRotation(CurrentRotation);
	}
}

void UThirdPersonCamera::SwitchToRunMode(const bool Activate)
{
	bRunMode = Activate;

	OnSwitchToRunMode(Activate);
}

void UThirdPersonCamera::TransitionRunMode(const float Alpha)
{
	if(bZoomIn) return;
	
	RunModeFOV = FMathf::Lerp(DefaultFOV, RunFOV, Alpha);
	RunModeDistance = FMathf::Lerp(DefaultDistance, RunDistance, Alpha);
	RunModeWorldOffset = FMath::Lerp(DefaultWorldOffset, RunWorldOffset, Alpha);
	RunModeRelativeOffset = FMath::Lerp(DefaultRelativeOffset, RunRelativeOffset, Alpha);
	RunModeLocLagSpeed = FMathf::Lerp(DefaultAutoLocationSpeed, RunAutoLocationSpeed, Alpha);
	OnTransitionRunMode(Alpha);
}

void UThirdPersonCamera::EnterZoomMode(const float TransitionDuration)
{
	bZoomMode = true;
	bZoomIn = true;

	ZoomDuration = TransitionDuration;
}

void UThirdPersonCamera::ExitZoomMode(const float TransitionDuration)
{
	bZoomMode = true;
	bZoomIn = false;

	ZoomDuration = TransitionDuration;
}

void UThirdPersonCamera::ZoomModeTransitionLoop(float DeltaTime)
{
	ZoomTransition += DeltaTime / (bZoomIn ? ZoomDuration : -ZoomDuration);

	FollowCamera->FieldOfView = FMath::Lerp(RunModeFOV, ZoomModeFOV, ZoomTransition);
	CurrentDistance = FMath::Lerp(RunModeDistance, ZoomModeDistance, ZoomTransition);
	CameraBoom->SocketOffset = FMath::Lerp(RunModeRelativeOffset, ZoomModeRelativeOffset, ZoomTransition)*DistanceMultiplier;
	CameraBoom->TargetOffset = FMath::Lerp(RunModeWorldOffset, ZoomModeWorldOffset, ZoomTransition);
	CameraBoom->CameraLagSpeed = FMath::Lerp(RunModeLocLagSpeed, ZoomModeLocLagSpeed, ZoomTransition);

	if(ZoomTransition >= 1.f)
	{
		FollowCamera->FieldOfView = ZoomModeFOV;
		CurrentDistance = ZoomModeDistance;
		CameraBoom->SocketOffset = ZoomModeRelativeOffset*DistanceMultiplier;
		CameraBoom->TargetOffset = ZoomModeWorldOffset;
		CameraBoom->CameraLagSpeed = ZoomModeLocLagSpeed;
		
		ZoomTransition = 1;
	}
	else if(ZoomTransition <= 0.f)
	{
		FollowCamera->FieldOfView = RunModeFOV;
		CurrentDistance = RunModeDistance;
		CameraBoom->SocketOffset = RunModeRelativeOffset*DistanceMultiplier;
		CameraBoom->TargetOffset = RunModeWorldOffset;
		CameraBoom->CameraLagSpeed = RunModeLocLagSpeed;

		ZoomTransition = 0;
		bZoomMode = false;
	}
}

void UThirdPersonCamera::EnterAimLock(const float TransitionSpeed)
{
	bLockRotationInput = true;
	AimTransitionSpeed = TransitionSpeed;
	bAiming = true;
}

void UThirdPersonCamera::ExitAimLock()
{
	bLockRotationInput = false;
	bAiming = false;
}

void UThirdPersonCamera::AimCameraLoop(float DeltaTime)
{
	if (PawnOwner && PawnOwner->IsPawnControlled())
	{
		FRotator CurrentRotation = PawnOwner->GetControlRotation();
		FRotator TargetRotation = AimTargetRotation;
		// added offset added to prevent interp locking at 180 degree distance
		TargetRotation += RotationOffset + FRotator(0,.01f, 0);
		TargetRotation.Normalize();

		CurrentRotation = FMath::QInterpTo(CurrentRotation.Quaternion(), TargetRotation.Quaternion(), DeltaTime, AimTransitionSpeed).Rotator();
		CurrentRotation.Roll = 0;
			
		PawnOwner->GetController()->SetControlRotation(CurrentRotation);
	}
}

void UThirdPersonCamera::SetAimTarget(const FRotator TargetRotation)
{
	AimTargetRotation = TargetRotation;
}

void UThirdPersonCamera::RestCamera()
{
	RestingTimer = RestingDuration;
}

bool UThirdPersonCamera::IsRunMode() const
{
	return bRunMode;
}

bool UThirdPersonCamera::IsResting() const
{
	return RestingTimer >= RestingDuration;
}

void UThirdPersonCamera::ResetZoomModeValues()
{
	ZoomModeFOV = DefaultFOV;
	ZoomModeDistance = DefaultDistance;
	ZoomModeRelativeOffset = DefaultRelativeOffset;
	ZoomModeWorldOffset = DefaultWorldOffset;
	ZoomModeLocLagSpeed = DefaultAutoLocationSpeed;
}

void UThirdPersonCamera::SetZoomFOV(const float Value)
{
	ZoomModeFOV = Value;
}

void UThirdPersonCamera::SetZoomDistance(const float Value)
{
	ZoomModeDistance = Value;
}

void UThirdPersonCamera::SetZoomWorldOffset(const FVector Value)
{
	RunModeWorldOffset = Value;
}

void UThirdPersonCamera::SetZoomRelativeOffset(const FVector Value)
{
	RunModeRelativeOffset = Value;
}

void UThirdPersonCamera::SetZoomLocationLagSpeed(const float Value)
{
	ZoomModeLocLagSpeed = Value;
}

void UThirdPersonCamera::OnRegister()
{
	Super::OnRegister();

	CameraBoom->TargetArmLength = DefaultDistance;
	CameraBoom->SocketOffset = DefaultRelativeOffset;
	CameraBoom->SetRelativeRotation(RotationOffset);
	CameraBoom->CameraLagSpeed = DefaultAutoLocationSpeed;
}