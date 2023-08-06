#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ThirdPersonCamera.generated.h"

UCLASS( ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent) )
class GP3_API UThirdPersonCamera : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraSpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

public:	
	UThirdPersonCamera();
	void SetupAttachment(USceneComponent* InParent, FName InSocketName = NAME_None);

protected:
	UPROPERTY(Transient)
	TObjectPtr<class APawn> PawnOwner;

protected:	
	/** Offset from player rotation */
	UPROPERTY(EditAnywhere, Category="Camera Variables")
	FRotator RotationOffset;

	/** transition speed for swiveling during turns */
	UPROPERTY(EditAnywhere, Category="Camera Variables")
	float SwivelSpeed;

	/** Minimum Boom Arm Length */
	UPROPERTY(EditAnywhere, Category="Camera Variables")
	float MinDistance = 50.f;

	/** Time until autorotation camera enables */
	UPROPERTY(EditAnywhere, Category="Camera Variables")
	float RestingDuration;

	UPROPERTY(Transient, VisibleAnywhere, Category="Camera Debug")
	float RestingTimer;
	
	//

	/** Default Field of View */
	UPROPERTY(EditAnywhere, Category="Camera Variables")
	float DefaultFOV;

	/** Default target arm length */
	UPROPERTY(EditAnywhere, Category="Camera Variables")
	float DefaultDistance;

	/** Default world space offset from player pivot */
	UPROPERTY(EditAnywhere, Category="Camera Variables")
	FVector DefaultWorldOffset;

	/** Default relative offset at end of spring arm */
	UPROPERTY(EditAnywhere, Category="Camera Variables") 
	FVector DefaultRelativeOffset;

	/** Default transition speed towards player */
	UPROPERTY(EditAnywhere, Category="Camera Variables")
	float DefaultAutoLocationSpeed;

	//

	UPROPERTY(Transient, VisibleAnywhere, Category="Camera Debug")
	bool bRunMode;

	/** Running Field of View */
	UPROPERTY(EditAnywhere, Category="Camera Variables")
	float RunFOV;

	/** Running target arm length */
	UPROPERTY(EditAnywhere, Category="Camera Variables")
	float RunDistance;

	/** Running world space offset from player pivot */
	UPROPERTY(EditAnywhere, Category="Camera Variables")
	FVector RunWorldOffset;

	/** Running relative offset at end of spring arm */
	UPROPERTY(EditAnywhere, Category="Camera Variables")
	FVector RunRelativeOffset;

	/** Running transition speed towards player */
	UPROPERTY(EditAnywhere, Category="Camera Variables")
	float RunAutoLocationSpeed;

	//

	UPROPERTY(Transient, VisibleAnywhere, Category="Camera Debug")
	bool bSnapCamera;

	/** Transition speed for camera reset */
	UPROPERTY(EditAnywhere, Category="Camera Variables")
	float SnapRotationSpeed;

	//

	bool bLockRotationInput;
	bool bZoomMode;
	bool bZoomIn;

	float ZoomTransition;
	float ZoomDuration;

	bool bAiming;
	float AimTransitionSpeed;
	FRotator AimTargetRotation;

	//
	
	float RunModeFOV;
	float RunModeDistance;
	FVector RunModeWorldOffset;
	FVector RunModeRelativeOffset;
	float RunModeLocLagSpeed;
	
	float ZoomModeFOV;
	float ZoomModeDistance;
	FVector ZoomModeWorldOffset;
	FVector ZoomModeRelativeOffset;
	float ZoomModeLocLagSpeed;	

	//

	UPROPERTY(Transient, BlueprintReadOnly)
	float CurrentDistance;

	/* Unused */
	UPROPERTY(Transient, BlueprintReadWrite)
	float DistanceOffset;

	UPROPERTY(Transient, BlueprintReadWrite)
	float DistanceMultiplier;

protected:
	virtual void BeginPlay() override;

	void SnapCameraLoop(float DeltaTime);

	void ZoomModeTransitionLoop(float DeltaTime);

	void AimCameraLoop(float DeltaTime);
	
	void ResetZoomModeValues();

	UFUNCTION(BlueprintCallable)
	void SwitchToRunMode(bool Activate);
	UFUNCTION(BlueprintImplementableEvent)
	void OnSwitchToRunMode(bool Active);
	UFUNCTION(BlueprintCallable)
	void TransitionRunMode(float Alpha);
	UFUNCTION(BlueprintImplementableEvent)
	void OnTransitionRunMode(float Alpha);
	

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OnRegister() override;

	/** Rotates camera using Yaw and pitch */
	void TurnCamera(float Yaw, float Pitch);

	/** Rotates camera yaw during rest mode using SwivelSpeed */
	UFUNCTION(BlueprintCallable)
	void SwivelCamera(float DeltaYaw);

	/** Snaps camera behind player */
	UFUNCTION(BlueprintCallable)
	void SnapCamera();

	/** Forces camera to enter rest mode */
	UFUNCTION(BlueprintCallable)
	void RestCamera();

	UFUNCTION(BlueprintCallable)
	bool IsRunMode() const;

	UFUNCTION(BlueprintCallable)
	bool IsResting() const;

	/** Ignores player inputs and aims towards AimTargetRotation */
	UFUNCTION(BlueprintCallable)
	void EnterAimLock(float TransitionSpeed = 10.f);

	UFUNCTION(BlueprintCallable)
	void ExitAimLock();

	UFUNCTION(BlueprintCallable)
	void SetAimTarget(FRotator TargetRotation);

	/** Starts transitioning towards assigned zoom values */
	UFUNCTION(BlueprintCallable)
	void EnterZoomMode(float TransitionDuration = .5f);

	/** Returns camera to default camera values */
	UFUNCTION(BlueprintCallable)
	void ExitZoomMode(float TransitionDuration = .5f);

	UFUNCTION(BlueprintCallable)
	void SetZoomFOV(float Value);
	UFUNCTION(BlueprintCallable)
	void SetZoomDistance(float Value);
	UFUNCTION(BlueprintCallable)
	void SetZoomWorldOffset(FVector Value);
	UFUNCTION(BlueprintCallable)
	void SetZoomRelativeOffset(FVector Value);
	UFUNCTION(BlueprintCallable)
	void SetZoomLocationLagSpeed(float Value);
};