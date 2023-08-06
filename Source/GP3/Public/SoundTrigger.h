
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "SubtitleManager.h"
#include "SoundTrigger.generated.h"

class UBoxComponent;

UCLASS()
class GP3_API ASoundTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	ASoundTrigger();
	virtual void Tick(float DeltaTime) override;
	

protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* Collider;

	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComp;

	UPROPERTY(EditAnywhere, Category = DialogSystem)
	UDataTable* PlayerLines;

	

private:
	
	UFUNCTION()
    void OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);    	
    UFUNCTION()
	void OnBoxEndOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndex, UPrimitiveComponent* OverlappedComp);    	

	//virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	UFUNCTION()
	void PlaySound(USoundBase* SFX, FString Subtitle);
};
