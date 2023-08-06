// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CinematicsManager.h"
#include "GameFramework/GameMode.h"
#include "ResurrectionManager.h"
#include "DialogueManager.h"
#include "GP3GameMode.generated.h"

/**
 * 
 */
UCLASS()
class GP3_API AGP3GameMode : public AGameMode
{
	GENERATED_BODY()


public:
	AGP3GameMode();
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;


	UFUNCTION(BlueprintPure,BlueprintCallable)
	AResurrectionManager* GetResManager();
	UPROPERTY(VisibleAnywhere,BlueprintGetter=GetResManager)
	TObjectPtr<AResurrectionManager> ResManager;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AResurrectionManager> ResManagerBP;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ADialogueManager> DialogueManagerBP;
	UFUNCTION(BlueprintPure,BlueprintCallable)
	ADialogueManager* GetDialogueManager();
	UPROPERTY(VisibleAnywhere,BlueprintGetter=GetDialogueManager)
	TObjectPtr<ADialogueManager> DialogueManager;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACinematicsManager> CinematicManagerBP;
	UFUNCTION(BlueprintPure,BlueprintCallable)
	ACinematicsManager* GetCinematicManager();
	UPROPERTY(VisibleAnywhere,BlueprintGetter=GetCinematicManager)
	TObjectPtr<ACinematicsManager> CinematicManager;
protected:
	void BeginPlay() override;
	
};
