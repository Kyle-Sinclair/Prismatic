#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Engine/GameInstance.h"
#include "CharacterUtil.h"
#include "Kismet/GameplayStatics.h"
#include "GP3SaveGame.generated.h"


UCLASS()
class GP3_API UGP3SaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString SaveGameName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FDateTime CreationTime;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<bool> AbilityUnlocks;
	UPROPERTY()
	FVector SpawnLocation;
	UPROPERTY()
	float MasterVolumeLevel = 1.f;
	UPROPERTY()
	float SFXVolumeLevel = 1.f;
	UPROPERTY()
	float MusicVolumeLevel = 1.f;
	
	
	void CreateSlot(const FString& SlotName)
	{
		SaveGameName = SlotName;
		CreationTime = FDateTime::Now();
		AbilityUnlocks.Init(false,4);
		SpawnLocation = FVector(0.f,0.f,0.f);
		MasterVolumeLevel = 1.f;
		SFXVolumeLevel = 1.f;
		MusicVolumeLevel = 1.f;
		// We need a line here that gives spawnlocation a default value
	}

	bool UpdateAbilityUnlocks(EAbility Ability);
	bool UpdateSpawnLocation(const FVector I_NewSpawnLocation);
	bool UpdateMasterVolumeLevel(float NewMasterVolume);
	bool UpdateSFXVolumeLevel(float NewSFXVolume);
	bool UpdateMusicVolumeLevel(float NewMusicVolume);
	
};
