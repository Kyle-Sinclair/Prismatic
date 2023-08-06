// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshExplodableComponent.h"

#include "Interactable.h"

UMeshExplodableComponent::UMeshExplodableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}
void UMeshExplodableComponent::BeginPlay()
{
	Super::BeginPlay();
	RegisterInteractableOwner();
	// ...
	
}


void UMeshExplodableComponent::RegisterInteractableOwner()
{
	AActor* Owner = GetOwner();
	AttachedInteractable = Cast<AInteractable>(Owner);
	if(AttachedInteractable != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1,15.0f,FColor::Black,TEXT("Owner registered"));
	}
}


void UMeshExplodableComponent::OnHitByExplosion_Implementation(ABombActor* Satchel)
{
	//AttachedInteractable->DoThing(Satchel);
	GEngine->AddOnScreenDebugMessage(-1,15.0f,FColor::Black,TEXT("Triggering explosion detected "));

	AttachedInteractable->DoSpecificThing(Satchel,this);
}
