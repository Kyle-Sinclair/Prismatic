#include "GP3GameInstance.h"

#include "GameFramework/Character.h"
#include "AlchemistCharacter.h"

#include "Kismet/GameplayStatics.h"

#define UNIQUE_SAVE_SLOT "SaveSlot"


void UGP3GameInstance::Init()
{
	Super::Init();
}


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
		UE_LOG(LogTemp, Warning, TEXT("Created new empty slot"));
		//Engine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Created new empty slot");
	}
	return UGameplayStatics::SaveGameToSlot(CurrentSaveGame,UNIQUE_SAVE_SLOT, 0); // Alice edit
}


void UGP3GameInstance::OnNewGame_Implementation(FName MapNameToLoad)
{
	
	//GEngine->AddOnScreenDebugMessage(-3, 15.f, FColor::Green, "Calling on newgame");
	UE_LOG(LogTemp, Warning, TEXT("Calling on newgame"));
	bIsLoadedGame = false;

	bIsNewGame = true;
	UGameplayStatics::OpenLevel(GetWorld(), MapNameToLoad, true);
	CreateNewSaveGame();
	
}

void UGP3GameInstance::OnLoadGame_Implementation(FName MapNameToLoad)
{
	LoadSaveSlotToCurrentSaveGamePtr();
	bIsLoadedGame = true;
	bIsNewGame = false;
	UGameplayStatics::OpenLevel(GetWorld(), MapNameToLoad, true);	
}


bool UGP3GameInstance::LoadSaveSlotToCurrentSaveGamePtr()
{
	CurrentSaveGame = nullptr;
	USaveGame* Slot = UGameplayStatics::LoadGameFromSlot(UNIQUE_SAVE_SLOT, 0);
	if (Slot != nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-3, 15.f, FColor::Green, "Slot found");
		UE_LOG(LogTemp, Warning, TEXT("Slot found"));

		CurrentSaveGame = Cast<UGP3SaveGame>(Slot);
		if (CurrentSaveGame != nullptr)
		{
			//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 15.f, FColor::Green, "Save game loaded");
			UE_LOG(LogTemp, Warning, TEXT(" Success loading %s"),UNIQUE_SAVE_SLOT);
			return true;
		}
	}
	return false;
}

void UGP3GameInstance::UpdatePlayerToReflectSaveStatus(AAlchemistCharacter* PlayerCharacter)
{
	//GEngine->AddOnScreenDebugMessage(-3, 15.f, FColor::Black, "Updating character");
	if(CurrentSaveGame != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Save Data Present"));
		//GEngine->AddOnScreenDebugMessage(-3, 15.f, FColor::Black, "Save Data Present");
		PlayerCharacter->SetActorLocation(CurrentSaveGame->SpawnLocation);
	}

}
//For now, we will save a tr ansform position that we teleport the character too
//It might be possible in future to move more of this functionality to the check point class itself

	bool UGP3GameInstance::WriteCheckPointToCurrentSaveFile(FVector CheckpointLocation)
	{
	//GEngine->AddOnScreenDebugMessage(-1,15.0f,FColor::Blue,FString::Printf(TEXT("World delta for current frame equals %s"),*targetLocation.ToString()));
	if(CurrentSaveGame == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot save check point location - Current save game is null"));
		return false;
	}
	if (CurrentSaveGame->UpdateSpawnLocation(CheckpointLocation))
	{
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Error saving check point position to the current save game"));
		return false;
	}
	
}

	bool UGP3GameInstance::WriteAbiltyGainedToCurrentSaveFile(EAbility AbilityUnlocked)
	{
	if (CurrentSaveGame->UpdateAbilityUnlocks(AbilityUnlocked))
	{
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Error saving ability unlock to the current save game"));
		return false;
	}
}

bool UGP3GameInstance::WriteSettingsToCurrentSaveFile(float NewMasterVolume)
{
	if(CurrentSaveGame != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Updating Volume"));
		//GEngine->AddOnScreenDebugMessage(-1,15.0f,FColor::Blue,TEXT("Updating Volume"));

		//CurrentSaveGame->UpdateMasterVolumeLevel(NewMasterVolume);
	}
	return true;
}


bool UGP3GameInstance::SaveCurrentSaveFileToSlot()
{
	if (CurrentSaveGame != nullptr)
	{
		return UGameplayStatics::SaveGameToSlot(CurrentSaveGame,UNIQUE_SAVE_SLOT, 0);
	}
	
	return false;
}


bool UGP3GameInstance::IsThereASaveFileOnDisk()
{
	if(UGameplayStatics::DoesSaveGameExist(UNIQUE_SAVE_SLOT, 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Save game found"));
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Save game found");
		return false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No save game found"));
		//GEngine->AddOnScreenDebugMessage(-2, 15.f, FColor::Green, "No save game found");
		return true;
	}
}