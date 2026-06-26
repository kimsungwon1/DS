

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DSGameMode.generated.h"

/**
 * 
 */
class ADSNPCParty;
class UCharacterInstanceComponent;
class APlayerPartyMover;

const int32 NoCycleNumber = -1;

// 델리게이트 선언 (bool 인자를 하나 받는 방송국)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBattleStateChanged, bool, bNewIsBattle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCycleStarted, bool, bCycleStarted);

UCLASS()
class DS_API ADSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADSGameMode();

	UPROPERTY(BlueprintAssignable, Category = "Battle")
	FOnBattleStateChanged OnBattleChanged;

	UPROPERTY(BlueprintAssignable, Category = "Battle")
	FOnCycleStarted OnCycleStarted;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsBattle() const { return bIsBattle; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsCycle() const { return bIsCycle; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartBattle();
	virtual void StartBattle_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EndBattle();
	virtual void EndBattle_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartCycle();
	virtual void StartCycle_Implementation();

	void SortCharacters();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EndCycle();
	virtual void EndCycle_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void JoinNpcToBattle(class UNPCCharacterInstanceComponent* npc, AActor* actor);
	void JoinNpcToBattle_Implementation(class UNPCCharacterInstanceComponent* npc, AActor* actor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RemoveNpcFromBattle(class UNPCCharacterInstanceComponent* npc);
	void RemoveNpcFromBattle_Implementation(class UNPCCharacterInstanceComponent* npc);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SwitchTurn();
	void SwitchTurn_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlayerDefeated();
	void PlayerDefeated_Implementation();

	UFUNCTION(BlueprintCallable)
	void PushFocus(UObject* forcusee);

	UFUNCTION(BlueprintCallable)
	void PopFocus(UObject* focusee);

	UFUNCTION(BlueprintCallable)
	void SwitchFocus(UObject* top, UObject* switchee);

	UFUNCTION(BlueprintCallable)
	void PushForTurn(UCharacterInstanceComponent* character);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UCharacterInstanceComponent* GetCurrentTurnGetter();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsThisTop(UObject* ob) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ADSPlayerController* GetDSPlayerController();

	TArray<ADSNPCParty*> GetNpcParties() { return arrPartiesInBattle; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	APlayerPartyMover* GetPartyMover();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCycleProgress() const;

protected:
	void DecideCharactersAction();
	bool IsBattleEnded();

protected:
	UPROPERTY(BlueprintReadWrite)
	bool bIsBattle = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsCycle = false;

	UPROPERTY()
	TObjectPtr<ADSPlayerController> dsPlayerController;

	UPROPERTY(BlueprintReadWrite)
	TArray<ADSNPCParty*> arrPartiesInBattle;

	UPROPERTY(BlueprintReadWrite)
	TArray< UCharacterInstanceComponent*> arrCharactersInTurn;

	UPROPERTY(BlueprintReadWrite)
	TArray< UCharacterInstanceComponent*> arrCharactersUnable;

	UPROPERTY(BlueprintReadOnly)
	int32 currentTurnGetterIndex = NoCycleNumber;

	UPROPERTY(BlueprintReadOnly)
	TArray<UObject*> focuseeStack;

	UPROPERTY()
	TObjectPtr<APlayerPartyMover> partyMover;

	UPROPERTY(EditAnywhere, EditFixedSize, BlueprintReadWrite)
	TArray<FColor> pcMemberColors;
};
