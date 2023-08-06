#include "SoundTrigger.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"

ASoundTrigger::ASoundTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->SetupAttachment(RootComponent, NAME_None);
	Collider->SetGenerateOverlapEvents(true);
	Collider->SetBoxExtent(FVector(500.f, 500.f, 500.f), false);
	Collider->SetCollisionProfileName(TEXT("Trigger"), false);

	AudioComp = CreateDefaultSubobject<UAudioComponent>(FName("AudioComp"));
	AudioComp->SetupAttachment(RootComponent);
}

void ASoundTrigger::BeginPlay()
{
	Super::BeginPlay();
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ASoundTrigger::OnBoxOverlap);
	//Collider->OnComponentEndOverlap.AddDynamic(this, &ASoundTrigger::OnBoxEndOverlap);
	
	
}

	


void ASoundTrigger::OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.f, FColor::Red, *FString::Printf(TEXT("Overlap begun, play SFX!")));
}

void ASoundTrigger::OnBoxEndOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndex, UPrimitiveComponent* OverlappedComp)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.f, FColor::Red, *FString::Printf(TEXT("Overlap end!")));
}

void ASoundTrigger::PlaySound(USoundBase* SFX, FString Subtitle)
{
	
}


void ASoundTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

