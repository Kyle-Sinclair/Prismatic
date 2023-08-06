// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveFileSubsystem.h"
#include "GP3SaveGame.h"

#define UNIQUE_SAVE_SLOT "SaveSlot"

void USaveFileSubsystem::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, TEXT("Save subsystem Initializing"));
	}
}

bool USaveFileSubsystem::IsThereASaveFileOnDisk()
{
	if(UGameplayStatics::DoesSaveGameExist(UNIQUE_SAVE_SLOT, 0))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Save game found");
		return false;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-2, 15.f, FColor::Green, "No save game found");
		return true;
	}
}

bool USaveFileSubsystem::CreateNewSaveGame()
{
	return true;
}


/*
*
bool UGP3GameInstance::CreateNewSaveGame()
{
if (CurrentSaveGame == nullptr)
{
USaveGame* NewSaveGame = UGameplayStatics::CreateSaveGameObject(UGP3SaveGame::StaticClass());
if (NewSaveGame != nullptr)
{
CurrentSaveGame = Cast<UGP3SaveGame>(NewSaveGame);
}
}
else
{ // Reset
CurrentSaveGame->CreateSlot(UNIQUE_SAVE_SLOT);
GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Created new empty slot");
}
return UGameplayStatics::SaveGameToSlot(CurrentSaveGame,UNIQUE_SAVE_SLOT, 0); // Alice edit
}


void UGP3GameInstance::OnNewGame_Implementation(FName MapNameToLoad)
{
	
GEngine->AddOnScreenDebugMessage(-3, 15.f, FColor::Green, "Calling on newgame");
bIsNewGame = true;
UGameplayStatics::OpenLevel(GetWorld(), MapNameToLoad, true);
CreateNewSaveGame();
	
}

void UGP3GameInstance::OnLoadGame_Implementation(FName MapNameToLoad)
{
LoadSaveSlotToCurrentSaveGamePtr();
bIsLoadedGame = true;
UGameplayStatics::OpenLevel(GetWorld(), MapNameToLoad, true);
	
}*/
