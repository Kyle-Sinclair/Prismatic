#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "CameraSpringArmComponent.generated.h"

UCLASS()
class GP3_API UCameraSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

private:
	/** Character Collider */
	TObjectPtr<class UCapsuleComponent> CapsuleComponent;

protected:
	/** bool to use regular SpringArmCollision or custom */
	UPROPERTY(EditAnywhere, Category="Camera Boom Variables")
	bool bUseNewCamera = true;

	/** Radius of collision check for pushing away to prevent clipping */
	UPROPERTY(EditAnywhere, Category="Camera Boom Variables")
	float CollisionRadius = 90.f;

	/** Interp speed used when collision causes zoom in */
	UPROPERTY(EditAnywhere, Category="Camera Boom Variables")
	float CollisionZoomInSpeed = 100.f;

	/** Interp speed used when collision causes zoom out */
	UPROPERTY(EditAnywhere, Category="Camera Boom Variables")
	float CollisionZoomOutSpeed = 25.f;

	/** Interp speed used when collision shifts location */
	UPROPERTY(EditAnywhere, Category="Camera Boom Variables")
	float CollisionYZSpeed = 50.f;

	/** Use complex tracing for collision */
	UPROPERTY(EditAnywhere, Category="Camera Boom Variables")
	bool bTraceComplex = false;
	
	UPROPERTY(VisibleAnywhere, Category="Camera Boom Debug")
	bool bInSight;

	UPROPERTY(Transient)
	FVector LastResultLoc;

public:
	virtual void BeginPlay() override;

protected:	
	virtual void UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime) override;

	float BlendCameraLocation(const float CurrentDist, const float TargetDist, float DeltaTime) const;
	
	virtual FVector BlendLocations(const FVector& DesiredArmLocation, const FVector& TraceHitLocation, bool bHitSomething, float DeltaTime) override;	
};