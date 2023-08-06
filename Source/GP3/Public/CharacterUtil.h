#pragma once

UENUM(BlueprintType)
enum class EAbility : uint8
{
	Ability_HorizontalDash	UMETA(DisplayName="Horizontal Dash"),
	Ability_VerticalDash	UMETA(DisplayName="Vertical Dash"),
	Ability_DashCharges		UMETA(DisplayName="Dash Charges"),
};
