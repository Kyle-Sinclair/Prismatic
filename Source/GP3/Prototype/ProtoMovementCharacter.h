#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProtoMovementCharacter.generated.h"

class UInputAction;
struct FInputActionValue;
enum class EAbility : uint8;

UCLASS(config=Game, BlueprintType)
class GP3_API AProtoMovementCharacter : public ACharacter
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> HDashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> VDashAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

public:
	AProtoMovementCharacter();

protected:
	UPROPERTY(EditAnywhere, DisplayName="Jump Force", BlueprintReadWrite, Category="Abilities|Air Jump", meta=(ClampMin = 0))
	float AirJumpForce = 100.f;

	UPROPERTY(EditAnywhere, DisplayName="Initial Velocity Multiplier", BlueprintReadWrite, Category="Abilities|Air Jump")
	FVector AirJumpCancelVelocity = FVector(1.f, 1.f, 0.f);

	//
	
	UPROPERTY(EditAnywhere, DisplayName="Action Delay", BlueprintReadWrite, Category="Abilities|Dash|Vertical Dash", meta=(ClampMin = 0))
	float VDashActionDelay = .1f;

	UPROPERTY(EditAnywhere, DisplayName="Impulse Force", BlueprintReadWrite, Category="Abilities|Dash|Vertical Dash", meta=(ClampMin = 0))
	float VDashForce = 100.f;
	
	UPROPERTY(EditAnywhere, DisplayName="Duration", BlueprintReadWrite, Category="Abilities|Dash|Vertical Dash", meta=(ClampMin = 0))
	float VDashDuration = .5f;

	UPROPERTY(EditAnywhere, DisplayName="Braking Scale", BlueprintReadWrite, Category="Abilities|Dash|Vertical Dash", meta=(ClampMin = 0, ClampMax=1))
	float VDashBrakingScale = .5f;

	UPROPERTY(EditAnywhere, DisplayName="Braking Slope", BlueprintReadWrite, Category="Abilities|Dash|Vertical Dash", meta=(ClampMin = 1, ClampMax=10))
	float VDashBrakingSlope = 2.f;

	UPROPERTY(EditAnywhere, DisplayName="Gravity Scale", BlueprintReadWrite, Category="Abilities|Dash|Vertical Dash", meta=(ClampMin = 0, ClampMax=1))
	float VDashGravityScale = .5f;

	UPROPERTY(EditAnywhere, DisplayName="Direction", BlueprintReadWrite, Category="Abilities|Dash|Vertical Dash")
	FVector VDashDirection = FVector(0, 0, 1);

	UPROPERTY(EditAnywhere, DisplayName="Initial Velocity Multiplier", BlueprintReadWrite, Category="Abilities|Dash|Vertical Dash")
	FVector VDashCancelVelocity = FVector(.5f, .5f, 0.f);
	
	//

	UPROPERTY(EditAnywhere, DisplayName="Action Delay", BlueprintReadWrite, Category="Abilities|Dash|Horizontal Dash", meta=(ClampMin = 0))
	float HDashActionDelay = .1f;

	UPROPERTY(EditAnywhere, DisplayName="Impulse Force", BlueprintReadWrite, Category="Abilities|Dash|Horizontal Dash", meta=(ClampMin = 0, Units="cm"))
	float HDashForce = 100.f;

	UPROPERTY(EditAnywhere, DisplayName="Duration", BlueprintReadWrite, Category="Abilities|Dash|Horizontal Dash", meta=(ClampMin = 0))
	float HDashDuration = .5f;

	UPROPERTY(EditAnywhere, DisplayName="Braking Scale", BlueprintReadWrite, Category="Abilities|Dash|Horizontal Dash", meta=(ClampMin = 0, ClampMax=1))
	float HDashBrakingScale = .5f;

	UPROPERTY(EditAnywhere, DisplayName="Braking Slope", BlueprintReadWrite, Category="Abilities|Dash|Horizontal Dash", meta=(ClampMin = 1, ClampMax=10))
	float HDashBrakingSlope = 2.f;

	UPROPERTY(EditAnywhere, DisplayName="Gravity Scale", BlueprintReadWrite, Category="Abilities|Dash|Horizontal Dash", meta=(ClampMin = 0, ClampMax=1))
	float HDashGravityScale = .5f;

	UPROPERTY(EditAnywhere, DisplayName="Direction", BlueprintReadWrite, Category="Abilities|Dash|Horizontal Dash")
	FVector HDashDirection = FVector(1, 0, 0);

	UPROPERTY(EditAnywhere, DisplayName="Initial Velocity Multiplier", BlueprintReadWrite, Category="Abilities|Dash|Horizontal Dash")
	FVector HDashCancelVelocity = FVector(.5f, .5f, .5f);


protected:
	virtual void BeginPlay() override;
	virtual void Landed(const FHitResult& Hit) override;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void JumpInput();

	UFUNCTION(BlueprintImplementableEvent)
	void OnAirJump();
	
	void HorizontalDashAction();
	void HDashLoop(float DeltaTime);
	UFUNCTION(BlueprintImplementableEvent)
	void OnHorizontalDashStart();
	UFUNCTION(BlueprintImplementableEvent)
	void OnHorizontalDashAction();
	UFUNCTION(BlueprintImplementableEvent)
	void OnHorizontalDashFinished();
	
	void VerticalDashAction();
	void VDashLoop(float DeltaTime);
	UFUNCTION(BlueprintImplementableEvent)
	void OnVerticalDashStart();
	UFUNCTION(BlueprintImplementableEvent)
	void OnVerticalDashAction();
	UFUNCTION(BlueprintImplementableEvent)
	void OnVerticalDashFinished();
	
	void ExitDash();

	FTimerHandle DashTimerHandle;

	UPROPERTY(Transient, VisibleAnywhere, Category="Abilities|Debug")
	bool CanAirJump;

	UPROPERTY(Transient, VisibleAnywhere, Category="Abilities|Debug")
	bool InHDash = false;
	UPROPERTY(Transient, VisibleAnywhere, Category="Abilities|Debug")
	bool InVDash = false;
	
	UPROPERTY(Transient, VisibleAnywhere, Category="Abilities|Debug")
	float DashLoopTimer;

	FVector DashDirection;
	
	FVector DashStartLocation;
	FVector DashEndLocation;

	UPROPERTY(EditAnywhere, Category="Abilities|Debug")
	bool DebugAbilityBypass = true;

	UPROPERTY(VisibleAnywhere, Category="Abilities|Debug")
	bool HasHorizontalDash = false;
	UPROPERTY(VisibleAnywhere, Category="Abilities|Debug")
	bool HasVerticalDash = false;
	UPROPERTY(VisibleAnywhere, Category="Abilities|Debug")
	int MaxDashCharges = 0;

	UPROPERTY(Transient, VisibleAnywhere, Category="Abilities|Debug")
	int CurrentDashCharges;

public:
	UFUNCTION(BlueprintCallable)
	void HorizontalDashStart();

	UFUNCTION(BlueprintCallable)
	void VerticalDashStart();

	UFUNCTION(BlueprintCallable)
	void ResetDashCharge();
	UFUNCTION(BlueprintCallable)
	bool IsInHorizontalDash();
	UFUNCTION(BlueprintCallable)
	bool IsInVerticalDash();
	UFUNCTION(BlueprintCallable)
	bool IsInDash();

	UFUNCTION(BlueprintCallable)
	void GiveAbility(EAbility Ability);
	UFUNCTION(BlueprintCallable)
	void GainDashCharge(int Amount = 1);
	UFUNCTION(BlueprintCallable)
	void SetDashCharges(int Amount);

	UFUNCTION(BlueprintCallable)
	bool HasAbility(EAbility Ability);

	UFUNCTION(BlueprintCallable)
	int GetMaxDashCharges() const;
	UFUNCTION(BlueprintCallable)
	int GetDashCharges();
	UFUNCTION(BlueprintCallable)
	bool HasDashCharge();

public:
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};