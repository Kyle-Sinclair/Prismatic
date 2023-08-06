// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BombActor.h"
#include "Components/ActorComponent.h"
#include "ExplodableComponent.generated.h"


UCLASS( ClassGroup=(Custom),BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent) )
class GP3_API UExplodableComponent : public UPrimitiveComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UExplodableComponent();
	UPROPERTY(VisibleAnywhere)
	class AInteractable* AttachedInteractable;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ExplodableMesh;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:	
	// Called every frame
	void RegisterInteractableOwner();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction Events")
	void OnHitByExplosion(ABombActor* Satchel);
	
	
	void OnHitByExplosion_Implementation(ABombActor* Satchel);
		
};
