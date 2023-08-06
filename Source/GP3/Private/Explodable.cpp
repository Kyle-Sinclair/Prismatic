// Fill out your copyright notice in the Description page of Project Settings.


#include "Explodable.h"

// Sets default values
AExplodable::AExplodable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


void AExplodable::OnHitByExplosion_Implementation(AActor* Satchel)
{
	
}

// Called when the game starts or when spawned
void AExplodable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplodable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

