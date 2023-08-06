
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "GP3GameUserSettings.generated.h"

UCLASS(/*Config=Game*/)
class GP3_API UGP3GameUserSettings : public UGameUserSettings
{
	GENERATED_UCLASS_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	void SetMasterVolumeLevel(float NewValue);
	UFUNCTION(BlueprintCallable)
	void SetSFXVolumeLevel(float NewValue);
	UFUNCTION(BlueprintCallable)
	void SetMusicVolumeLevel(float NewValue);
	
	UFUNCTION(BlueprintPure)
	float GetMasterVolumeLevel () const;
	UFUNCTION(BlueprintPure)
	float GetSFXVolumeLevel () const;
	UFUNCTION(BlueprintPure)
	float GetMusicVolumeLevel () const;

	UFUNCTION(BlueprintCallable)
	static UGP3GameUserSettings* GetCustomUserSettings();

protected:
	UPROPERTY(Config) // Will be saved in the config file
	FString CustomSettingVar1;
	UPROPERTY(Config)
	float MasterVolumeLevel;
	UPROPERTY(Config)
	float SFXVolumeLevel;
	UPROPERTY(Config)
	float MusicVolumeLevel;
	
	
	
};
