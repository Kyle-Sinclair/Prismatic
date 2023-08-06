// Fill out your copyright notice in the Description page of Project Settings.


#include "GP3GameMode.h"

#include "GP3GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "ResurrectionManager.h"



AGP3GameMode::AGP3GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

}

void AGP3GameMode::BeginPlay()
{
	Super::BeginPlay();
	
	UGP3GameInstance* GameInstance = Cast<UGP3GameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); // Alice edit
	
}


void AGP3GameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 15.f, FColor::Green, "Init!");

	const FVector SpawnPosition = FVector(-1000.f,1000.f,0.f);
	const FRotator SpawnRotation = FRotator(0.f,0.f,0.f);
	const FActorSpawnParameters SpawnParameters;
	ResManager = GetWorld()->SpawnActor<AResurrectionManager>(ResManagerBP,SpawnPosition,SpawnRotation,SpawnParameters);
	DialogueManager = GetWorld()->SpawnActor<ADialogueManager>(DialogueManagerBP,SpawnPosition,SpawnRotation,SpawnParameters);
	CinematicManager = GetWorld()->SpawnActor<ACinematicsManager>(CinematicManagerBP,SpawnPosition,SpawnRotation,SpawnParameters);
}

AResurrectionManager* AGP3GameMode::GetResManager()
{
	return ResManager;
}
ADialogueManager* AGP3GameMode::GetDialogueManager()
{
	return DialogueManager;
}
ACinematicsManager* AGP3GameMode::GetCinematicManager()
{
	return CinematicManager;
}


