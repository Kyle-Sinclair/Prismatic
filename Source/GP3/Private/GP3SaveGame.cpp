#include "GP3SaveGame.h"

bool UGP3SaveGame::UpdateAbilityUnlocks(EAbility Ability)

{
	if(Ability == EAbility::Ability_HorizontalDash)
	{
		AbilityUnlocks[0] = true;
		return true;

	}
	if(Ability == EAbility::Ability_VerticalDash)
	{
		AbilityUnlocks[1] = true;
		return true;

	}
	if(Ability == EAbility::Ability_DashCharges)
	{
		AbilityUnlocks[2] = true;
		return true;
	}
	return false;
	
	
}

bool UGP3SaveGame::UpdateSpawnLocation(const FVector I_NewSpawnLocation)
{
	SpawnLocation =  I_NewSpawnLocation;
	return true;
	
}

bool UGP3SaveGame::UpdateMasterVolumeLevel(float NewMasterVolume)
{
    MasterVolumeLevel = NewMasterVolume;
	return true;
}

bool UGP3SaveGame::UpdateSFXVolumeLevel(float NewSFXVolume)
{
	SFXVolumeLevel = NewSFXVolume;
	return true;
}

bool UGP3SaveGame::UpdateMusicVolumeLevel(float NewMusicVolume)
{
	MusicVolumeLevel = NewMusicVolume;
	return true;
}

