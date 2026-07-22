

#pragma once

#include "CoreMinimal.h"
#include "DSUserWidget.h"
#include "DSMainWidget.generated.h"
/**
 * 
 */

class UPlayerCharacterInstanceComponent;
class UCharacterInstanceComponent;
class UDSAction;

UCLASS()
class DS_API UDSMainWidget : public UDSUserWidget
{
	GENERATED_BODY()
public:
	UDSMainWidget();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetCharacter(int32 index, UPlayerCharacterInstanceComponent* playerInstance);
	void SetCharacter_Implementation(int32 index, UPlayerCharacterInstanceComponent* playerInstance);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BeginBattle();
	void BeginBattle_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EndBattle();
	void EndBattle_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EnterHomeBase();
	void EnterHomeBase_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ExitHomeBase();
	void ExitHomeBase_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool SelectActionTarget(UDSAction* action);
	bool SelectActionTarget_Implementation(UDSAction* action) { return true; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OpenSpellList(UCharacterInstanceComponent* actorInstance, class USpellCast* castAction);
	void OpenSpellList_Implementation(UCharacterInstanceComponent* actorInstance, class USpellCast* castAction) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CloseSpellList();
	void CloseSpellList_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ActionSelectDone();
	void ActionSelectDone_Implementation() { CloseSpellList(); }

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, EditFixedSize)
	TArray<TObjectPtr<UPlayerCharacterInstanceComponent>> characters;
};
