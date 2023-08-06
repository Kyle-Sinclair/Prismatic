
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MenuGameMode.generated.h"

UCLASS()

class GP3_API AMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Menu Events")
	void OnNewGame(FName MapNameToLoad);

	void OnNewGame_Implementation(FName MapNameToLoad);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Menu Events")
	void OnLoadGame(FName MapToLoadName);

	void OnLoadGame_Implementation(FName MapNameToLoad);
	
protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Menu")
	class UGP3GameInstance* GameInstance;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Menu Events")
	void OnShowMainMenu(bool NewGame);
};
