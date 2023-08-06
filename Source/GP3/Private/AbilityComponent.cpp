#include "AbilityComponent.h"

#include "AlchemistCharacter.h"
#include "BombActor.h"
#include "MathUtil.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAbilityComponent::BeginPlay()
{
	AlchemistOwner = Cast<AAlchemistCharacter>(GetOwner());
	
	Super::BeginPlay();
}

void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FVector UAbilityComponent::GetWorldSpaceBombSpawnDirection() const
{
	return GetOwner()->GetActorQuat().RotateVector(BombSpawnDirection.GetSafeNormal());
}

void UAbilityComponent::EnqueueSpawnedBomb(ABombActor* NewBomb)
{
	SpawnedBombQueue.Enqueue(NewBomb);
}


ABombActor* UAbilityComponent::DequeueSpawnedBomb()
{
	ABombActor* QueuedBomb = nullptr;
	if(!SpawnedBombQueue.IsEmpty())
	{
		
		QueuedBomb = *SpawnedBombQueue.Peek();
		SpawnedBombQueue.Pop();
	}

	return QueuedBomb;
}


void UAbilityComponent::ThrowBomb(const bool bForce /*=false*/)
{
	if (!bForce && AlchemistOwner && AlchemistOwner->IsIgnoreInputActions()) return;
	
	if(CurrentBombCount >= MaxBombCount || bBombCooldown)
	{
		ExplodeBomb();
		return;
	}
	CurrentBombCount++;
	OnThrowBomb();
}

void UAbilityComponent::LayBomb(const bool bForce /*=false*/)
{
	if (!bForce && AlchemistOwner && AlchemistOwner->IsIgnoreInputActions()) return;
	
	if(CurrentBombCount >= MaxBombCount || bBombCooldown)
	{
		ExplodeBomb();
		return;
	}
	CurrentBombCount++;
	OnLayBomb();
}

void UAbilityComponent::ExplodeBomb(const bool bForce /*=false*/)
{
	if (!bForce && AlchemistOwner && AlchemistOwner->IsIgnoreInputActions()) return;
	
	if(CurrentBombCount == 0 || SpawnedBombQueue.IsEmpty()) return;
	CurrentBombCount--;
	OnExplodeBomb();
}
void UAbilityComponent::ClearBombQueue()
{
	if(CurrentBombCount == 0 || SpawnedBombQueue.IsEmpty()) return;
	OnClearBombQueue();
}

void UAbilityComponent::SetBombCooldown(const bool Active)
{
	bBombCooldown = Active;
	if(Active) BombCooldownStart = GetWorld()->GetTimeSeconds();
}

bool UAbilityComponent::IsBombCooldown() const
{
	return bBombCooldown;
}

float UAbilityComponent::GetBombCooldownTime() const
{
	if(!bBombCooldown) return 1.f;
	
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	return FMathf::Clamp((CurrentTime - BombCooldownStart)/BombCooldownDuration, 0, 1);
}