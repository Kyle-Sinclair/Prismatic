// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Explodable.generated.h"

UCLASS()
class GP3_API AExplodable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplodable();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction Events")
	void OnHitByExplosion(AActor* Satchel);
	
	
	void OnHitByExplosion_Implementation(AActor* Satchel);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
