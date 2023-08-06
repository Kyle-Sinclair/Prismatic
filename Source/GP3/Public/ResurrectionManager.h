#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResurrectionManager.generated.h"

UCLASS()
class GP3_API AResurrectionManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AResurrectionManager();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite)
	bool bProcessingDeath;

	UFUNCTION(BlueprintImplementableEvent)
	void OnKillPlayer();

public:	
	virtual void Tick(float DeltaTime) override;
	void HandleResurrection();
	void HandlePlayerAtGameContinue();

	UFUNCTION(BlueprintCallable)
	bool TryKillPlayer();

};
