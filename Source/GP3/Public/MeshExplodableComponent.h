// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BombActor.h"
#include "Components/StaticMeshComponent.h"
#include "MeshExplodableComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom),BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent) )
class GP3_API UMeshExplodableComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
public:
	UMeshExplodableComponent();
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AInteractable* AttachedInteractable;
	void RegisterInteractableOwner();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction Events")
	void OnHitByExplosion(ABombActor* Satchel);
	
	
	void OnHitByExplosion_Implementation(ABombActor* Satchel);
};
