// Fill out your copyright notice in the Description page of Project Settings.


#include "CinematicsManager.h"

// Sets default values
ACinematicsManager::ACinematicsManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACinematicsManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACinematicsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

