
#include "GP3GameUserSettings.h"


#define GAME_CONFIG_PATH_NAME = TEXT("GameSettings.ini")
UGP3GameUserSettings::UGP3GameUserSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MasterVolumeLevel = 1.f;
	SFXVolumeLevel = 1.f;
	MusicVolumeLevel = 1.f;
	
}

void UGP3GameUserSettings::SetMasterVolumeLevel(float NewValue)
{
		MasterVolumeLevel = NewValue;
		SaveConfig();

}

void UGP3GameUserSettings::SetSFXVolumeLevel(float NewValue)
{
	SFXVolumeLevel = NewValue;
	SaveConfig();
}

void UGP3GameUserSettings::SetMusicVolumeLevel(float NewValue)
{
	MusicVolumeLevel = NewValue;
	SaveConfig();
}

float UGP3GameUserSettings::GetMasterVolumeLevel() const
{
	return MasterVolumeLevel;
}

float UGP3GameUserSettings::GetSFXVolumeLevel() const
{
	return SFXVolumeLevel;
}

float UGP3GameUserSettings::GetMusicVolumeLevel() const
{
	return MusicVolumeLevel;
}

UGP3GameUserSettings* UGP3GameUserSettings::GetCustomUserSettings()
{
	return Cast<UGP3GameUserSettings>(UGameUserSettings::GetGameUserSettings());
}
