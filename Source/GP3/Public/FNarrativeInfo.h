#pragma once

#include "Engine/DataTable.h"
#include "FNarrativeInfo.generated.h"
	
USTRUCT(Blueprintable)
struct FNarrativeInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Subtitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* SFX;
	
	//UPROPERTY(EditAnywhere, meta=(RowType=PlantDataRow))
	//FDataTableRowHandle RowHandle;
	
};

