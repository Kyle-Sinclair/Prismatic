// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP3SaveGame.h"
#include "Engine/GameInstance.h"
#include "AlchemistCharacter.h"

#include "GP3GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GP3_API UGP3GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	///// Initialisation //////
	virtual void Init() override;
public:
	
	UPROPERTY()
	UGP3SaveGame* CurrentSaveGame;
	UFUNCTION(BlueprintCallable)

	bool IsThereASaveFileOnDisk();
	UFUNCTION(BlueprintCallable)

	bool CreateNewSaveGame();

public:
	UFUNCTION(BlueprintCallable)
	bool LoadSaveSlotToCurrentSaveGamePtr();
	UFUNCTION(BlueprintCallable)
	bool WriteCheckPointToCurrentSaveFile(FVector CheckpointLocation);
	UFUNCTION(BlueprintCallable)
	bool WriteAbiltyGainedToCurrentSaveFile(EAbility AbilityUnlocked);
	UFUNCTION(BlueprintCallable)
	bool WriteSettingsToCurrentSaveFile(float NewMasterVolume);
	UFUNCTION(BlueprintCallable)
	bool SaveCurrentSaveFileToSlot();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Level Loading Events")
	void OnNewGame(FName MapNameToLoad);

	void OnNewGame_Implementation(FName MapNameToLoad);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Level Loading Events")
	void OnLoadGame(FName MapToLoadName);

	void OnLoadGame_Implementation(FName MapNameToLoad);

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerToReflectSaveStatus(AAlchemistCharacter* PlayerCharacter);
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	bool bIsNewGame = false;
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	bool bIsLoadedGame = false;
	
	
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	float MasterVolumeLevel = 1.f;
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	float SFXVolumeLevel = 1.f;
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	float MusicVolumeLevel = 1.f;
	
}
;