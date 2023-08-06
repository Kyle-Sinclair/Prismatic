// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplodableComponent.h"

#include "Interactable.h"

// Sets default values for this component's properties
UExplodableComponent::UExplodableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	ExplodableMesh = CreateDefaultSubobject<UStaticMeshComponent>("Explodable Mesh");
	//ExplodableMesh->SetupAttachment(this);
	ExplodableMesh->AttachToComponent(this,FAttachmentTransformRules::KeepRelativeTransform);
	// ...
}


// Called when the game starts
void UExplodableComponent::BeginPlay()
{
	Super::BeginPlay();
	RegisterInteractableOwner();
	// ...
	
}

void UExplodableComponent::RegisterInteractableOwner()
{
	AActor* Owner = GetOwner();
	AttachedInteractable = Cast<AInteractable>(Owner);
	if(AttachedInteractable != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1,15.0f,FColor::Black,TEXT("Owner registered"));
	}
}


// Called every frame
void UExplodableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UExplodableComponent::OnHitByExplosion_Implementation(ABombActor* Satchel)
{
	
}

