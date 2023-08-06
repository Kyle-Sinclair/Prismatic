#include "BombActor.h"

#include "Components/SphereComponent.h"

ABombActor::ABombActor()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereComponent->InitSphereRadius(50.f);
	RootComponent = SphereComponent;
	
	PrimaryActorTick.bCanEverTick = true;
}

void ABombActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABombActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABombActor::Explode()
{
	OnExplode();
}
void ABombActor::Despawn()
{
	OnDespawn();
}

float ABombActor::GetCollisionRadius() const
{
	return SphereComponent->GetScaledSphereRadius();
}