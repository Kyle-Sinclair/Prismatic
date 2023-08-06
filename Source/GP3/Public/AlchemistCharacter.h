#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AlchemistCharacter.generated.h"

struct FInputActionValue;
class UInputAction;

UCLASS(config=Game, BlueprintType)
class GP3_API AAlchemistCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAbilityComponent> AbilityComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UThirdPersonCamera> CharacterCamera;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

public:
	AAlchemistCharacter();

protected:
	/** Ignores gameplay action inputs, doesnt include camera control */
	bool bIgnoreActions;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void JumpAction(bool bForce = false);
	UFUNCTION(BlueprintImplementableEvent)
	void OnJumpAction();
	
	UFUNCTION(BlueprintCallable)
	void Move(FVector2D Value);
	UFUNCTION(BlueprintImplementableEvent)
	void OnMove();
	
	UFUNCTION(BlueprintCallable)
	void Look(FVector2D Value);
	UFUNCTION(BlueprintImplementableEvent)
	void OnLook();
	
	UFUNCTION(BlueprintCallable)
	void Die();	
	UFUNCTION(BlueprintImplementableEvent)
	void OnDying();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnResetExtraJump();

	UFUNCTION(BlueprintImplementableEvent)
	void OnResetGravity();

	UFUNCTION(BlueprintImplementableEvent)
	void OnBoost();

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	FVector GetGroundLocation() const;

	UFUNCTION(BlueprintCallable)
	void ResetExtraJump();

	UFUNCTION(BlueprintCallable)
	void ResetGravity();

	/** Tells player it has started a boost */
	UFUNCTION(BlueprintCallable)
	void Boost();

	/** Ignores gameplay action inputs, doesnt include camera control */
	UFUNCTION(BlueprintCallable)
	void IgnoreInputActions(bool bIgnore);

	/** Ignores gameplay action inputs, doesnt include camera control */
	UFUNCTION(BlueprintCallable)
	bool IsIgnoreInputActions() const;
};