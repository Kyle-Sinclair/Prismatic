#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityComponent.generated.h"

class ABombActor;

UCLASS( ClassGroup=(Custom), BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent) )
class GP3_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAbilityComponent();

private:
	UPROPERTY(Transient)
	TObjectPtr<class AAlchemistCharacter> AlchemistOwner;
	
protected:
	/** Amount of bombs the player can spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Abilities|Bomb")
	int MaxBombCount = 1;

	/** Unused direction previously used when throwing bomb */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Abilities|Bomb")
	FVector BombSpawnDirection;

	/** Amount of bombs currently spawned */
	UPROPERTY(Transient, BlueprintReadWrite, VisibleAnywhere, Category="Abilities|Bomb|Debug")
	int CurrentBombCount;
	
	//

	/** Selected duration for BombCooldown */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Abilities|Bomb")
	float BombCooldownDuration = .8f;
	
	UPROPERTY(Transient, VisibleAnywhere, Category="Abilities|Bomb|Debug")
	bool bBombCooldown;

	/** Timestamp when BombCooldown=true started */
	UPROPERTY(Transient)
	float BombCooldownStart;

	/** Queue containing bombs in spawned order */
	TQueue<TObjectPtr<ABombActor>> SpawnedBombQueue;
	
	UFUNCTION(BlueprintCallable)
	void EnqueueSpawnedBomb(ABombActor* NewBomb);

	UFUNCTION(BlueprintCallable)
	ABombActor* DequeueSpawnedBomb();	

public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Throws bomb only sends if can, otherwise try to explode existing bomb */
	UFUNCTION(BlueprintCallable)
	void ThrowBomb(bool bForce = false);
	UFUNCTION(BlueprintImplementableEvent)
	void OnThrowBomb();

	/** Lays bomb underneath player only sends if can, otherwise try to explode existing bomb */
	UFUNCTION(BlueprintCallable)
	void LayBomb(bool bForce = false);
	UFUNCTION(BlueprintImplementableEvent)
	void OnLayBomb();

	/** Explodes the bomb inside of the queue */
	UFUNCTION(BlueprintCallable)
	void ExplodeBomb(bool bForce = false);
	UFUNCTION(BlueprintImplementableEvent)
	void OnExplodeBomb();

	/** Clears bombs inside of queue, implemented on BP */
	UFUNCTION(BlueprintCallable)
	void ClearBombQueue();
	UFUNCTION(BlueprintImplementableEvent)
	void OnClearBombQueue();

	/** Sets bBombCooldown and saves the timestamp */
	UFUNCTION(BlueprintCallable)
	void SetBombCooldown(bool Active);
	UFUNCTION(BlueprintCallable)
	bool IsBombCooldown() const;

	/** Returns a normalized value that represents the progress of the bomb cooldown */
	UFUNCTION(BlueprintCallable)
	float GetBombCooldownTime() const;

	UFUNCTION(BlueprintGetter)
	FVector GetWorldSpaceBombSpawnDirection() const;	
};