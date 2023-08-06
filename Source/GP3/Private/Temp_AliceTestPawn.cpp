
#include "Temp_AliceTestPawn.h"
#include "EnhancedInputSubsystems.h"
#include "Components/BoxComponent.h"
#include "Engine/DataTable.h"
#include "Components/AudioComponent.h"
#include "SoundTrigger.h"

#include "DialogueUI.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"

ATemp_AliceTestPawn::ATemp_AliceTestPawn()
{

	PrimaryActorTick.bCanEverTick = true;
/*
	bIsTalking = false;
	bIsInTalkRange = false;
	AssociatedTrigger = nullptr;
*/	
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->SetupAttachment(RootComponent, NAME_None);
	Collider->SetGenerateOverlapEvents(true);
	Collider->SetBoxExtent(FVector(500.f, 500.f, 500.f), false);
	Collider->SetCollisionProfileName(TEXT("Trigger"), false);

	AudioComp = CreateDefaultSubobject<UAudioComponent>(FName("AudioComp"));
	AudioComp->SetupAttachment(GetRootComponent());
}

void ATemp_AliceTestPawn::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	Collider->OnComponentBeginOverlap.AddDynamic(this, &ATemp_AliceTestPawn::OnBoxOverlap);
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
	
}
void ATemp_AliceTestPawn::OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.f, FColor::Red, *FString::Printf(TEXT("Overlap begun, play SFX!")));
}


void ATemp_AliceTestPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATemp_AliceTestPawn::Movefa(const FInputActionValue& Value)
{
	float MovementValue = Value.Get<float>();

	if (Controller != nullptr)
	{
		FVector position = this->GetActorLocation();
		position.Y += MovementValue * Speed;
		this->SetActorLocation(position);
	}
}

void ATemp_AliceTestPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		
		EnhancedInputComponent->BindAction(MoveAction_1, ETriggerEvent::Triggered, this, &ATemp_AliceTestPawn::Movefa);
	}

}
/*
void ATemp_AliceTestPawn::ToggleTalking()
{
	if (bIsInTalkRange)
	{
		// If in talking range, handle talk status and UI
		bIsTalking = !bIsTalking;
		ToggleUI();
		if (bIsTalking && AssociatedTrigger)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.f, FColor::Red, *FString::Printf(TEXT("Talkytalk")));
		}
	}
}
*/
