#include "MenuGameMode.h"
#include "GP3GameInstance.h"
#include "Kismet/GameplayStatics.h"




void AMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UGP3GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GameInstance != nullptr)
	{ 
		bool bIsNewGame = GameInstance->IsThereASaveFileOnDisk(); 
		OnShowMainMenu(bIsNewGame); 
	}
}

	void AMenuGameMode::OnLoadGame_Implementation(FName MapNameToLoad)
	{	
		UGameplayStatics::OpenLevel(GetWorld(), MapNameToLoad, true);
	}
	

	void AMenuGameMode::OnNewGame_Implementation(FName MapNameToLoad)
	{
	
		if (GameInstance == nullptr) return;
	

		//Debug Check only -> reports if file management classes failed to create a slot and doesn't move off map if so
	
		if(GameInstance->CreateNewSaveGame())
		{
			UGameplayStatics::OpenLevel(GetWorld(), MapNameToLoad, true);
		}
	}