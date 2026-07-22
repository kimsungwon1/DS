

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
class ADSPlayerParty;
class UPlayerPartyManagerComponent;

const int32 NoCycleNumber = -1;

// 델리게이트 선언 (bool 인자를 하나 받는 방송국)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBattleStateChanged, bool, bNewIsBattle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCycleStarted, bool, bCycleStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHomeBaseChanged, bool, bNewIsInHomeBase);

UCLASS()
class DS_API ADSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADSGameMode();

protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintAssignable, Category = "Battle")
	FOnBattleStateChanged OnBattleChanged;

	UPROPERTY(BlueprintAssignable, Category = "Battle")
	FOnCycleStarted OnCycleStarted;

	UPROPERTY(BlueprintAssignable, Category = "HomeBase")
	FOnHomeBaseChanged OnHomeBaseChanged;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsBattle() const { return bIsBattle; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsCycle() const { return bIsCycle; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsInHomeBase() const { return bIsInHomeBase; }

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
	void EnterHomeBase();
	virtual void EnterHomeBase_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ExitHomeBase();
	virtual void ExitHomeBase_Implementation();

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
	void PushFocus(UObject* focusee);

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

	// 위젯 등에서 GetOwningPlayer()/GetPlayerController()가 상황에 따라 null이 나오는 문제를 피하려고
	// GameMode를 통해 캐싱된 레퍼런스를 직접 받아가게 함
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ADSPlayerParty* GetPartyObject();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UPlayerPartyManagerComponent* GetPartyManager();

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

	UPROPERTY(BlueprintReadWrite)
	bool bIsInHomeBase = false;

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

	UPROPERTY()
	TObjectPtr<ADSPlayerParty> cachedPartyObject;

	UPROPERTY()
	TObjectPtr<UPlayerPartyManagerComponent> cachedPartyManager;

	UPROPERTY(EditAnywhere, EditFixedSize, BlueprintReadWrite)
	TArray<FColor> pcMemberColors;
};
