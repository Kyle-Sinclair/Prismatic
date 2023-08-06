#include "ResurrectionManager.h"

AResurrectionManager::AResurrectionManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AResurrectionManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void AResurrectionManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AResurrectionManager::TryKillPlayer()
{
	if(bProcessingDeath) return false;

	bProcessingDeath = true;

	OnKillPlayer();
	return true;
}

