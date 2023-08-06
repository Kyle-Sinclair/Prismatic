// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP3SaveGame.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveFileSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class GP3_API USaveFileSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	public:
		virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	UPROPERTY()
	UGP3SaveGame* CurrentSaveGame;

	UFUNCTION(BlueprintCallable)
	bool IsThereASaveFileOnDisk();
	
	UFUNCTION(BlueprintCallable)
	bool CreateNewSaveGame();
};
