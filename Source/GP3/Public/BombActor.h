#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BombActor.generated.h"

UCLASS()
class GP3_API ABombActor : public AActor
{
	GENERATED_BODY()

	//* Reference to sphere collider */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereComponent;
	
public:	
	ABombActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void Explode();
	UFUNCTION(BlueprintImplementableEvent)
	void OnExplode();
	UFUNCTION(BlueprintCallable)
	void Despawn();
	UFUNCTION(BlueprintImplementableEvent)
	void OnDespawn();

	/** Returns sphere colliders scaled radius */
	UFUNCTION(BlueprintCallable)
	float GetCollisionRadius() const;
};