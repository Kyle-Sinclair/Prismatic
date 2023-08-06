
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"

#include "Temp_AliceTestPawn.generated.h"


class UInputAction;
class UBoxComponent;
class UDialogueUI;
class UDataTable;
class ASoundTrigger;
class UAudioComponent;

UCLASS()

class GP3_API ATemp_AliceTestPawn : public APawn
{
	GENERATED_BODY()

public:
	ATemp_AliceTestPawn();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);  
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* Collider;

	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComp;
/*
	UFUNCTION(BlueprintImplementableEvent, Category = DialogSystem)
	void ToggleUI();

	// UI reference
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UDialogueUI* UI;
*/
public:	
	virtual void Tick(float DeltaTime) override;

	void Movefa(const FInputActionValue& Value);
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/*
		// Generate player lines
	void GeneratePlayerLines(UDataTable& PlayerLines);

	UFUNCTION(BlueprintCallable, Category = DialogSystem) //Dont need?
	void Talk(FString Excerpt, FString Subtitles);
	
	// Enable/disable talk ability
	void SetTalkRangeStatus(bool Status) { bIsInTalkRange = Status; }

	// Get UI reference
	UDialogueUI* GetUI() { return UI; }
	*/
private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction_1;
	
	float Speed = 20.f;
	/*
	//True if player is currently talking
	bool bIsTalking;

	//True if player is inside a valid range to start talking
	bool bIsInTalkRange;
	
	// Initiates/terminates talking
	void ToggleTalking(); //Dont need?
	
	//Reference to lines
	UDataTable* AvailableLines;

	//Searches in the given row inside specified table
	FNarrativeInfo* RetrieveDialog(UDataTable* TableToSearch, FName RowName);

	// Trigger actor
	ASoundTrigger* AssociatedTrigger;
	
	 */
};
