

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "DSTarget.h"
#include "TurnSwitcher.h"
#include "DSAction.generated.h"

/**
 * 
 */

class UDSActionData;
class UDSCharacterBaseData;
class UDSCharacterBaseInstance;
class ABaseCharacter;

UCLASS(Abstract)
class DS_API UDSAction : public UAction, public ITurnSwitcher
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDSActionData* baseData = nullptr;

	// ableActions로부터 고를 때마다 Initialize호출한다.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Initialize(UCharacterInstanceComponent* actorInstance);
	virtual void Initialize_Implementation(UCharacterInstanceComponent* actorInstance);

	// 행동의 상대를 고르거나 한다. 이걸 블루프린트에서 재정의해 ui따위에서 직접 고르도록 한다. 아마 this를 넘겨줘서 상대를 직접 지정하게 할 것이다.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool SelectAction();
	virtual bool SelectAction_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReadyAction();
	virtual void ReadyAction_Implementation() {}

	// 예시 : 캐릭터가 칼을 휘두름 - 애니메이션 시작/함성 지르기 등
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartAction();
	virtual void StartAction_Implementation();

	// 예시 : 캐릭터가 칼을 휘두름 - 휘두르는 중 = 휘두르는 딜레이/애니메이션 중
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ExecuteAction();
	virtual void ExecuteAction_Implementation();

	// 예시 : 애니메이션 끝-전환 + 더 할게 없음 = 호출
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EndAction();
	virtual void EndAction_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsTargetLost() const;
	virtual bool IsTargetLost_Implementation() const { return false; }

	virtual void ReceiveTurn_Implementation(ADSGameMode* mode, UObject* passer);

	virtual void TurnFinished_Implementation(ADSGameMode* mode, float fDelay);

	virtual void PassTurn_Implementation(ADSGameMode* mode, UObject* switcher);

	int GetActionCount() const { return actionCount; }
	UCharacterInstanceComponent* GetActor() const { return actor; }

	EDSTargetType GetType() const { return targetType; }

protected:
	UPROPERTY(BlueprintReadOnly)
	int actionCount = 1;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UCharacterInstanceComponent> actor;
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr< ABaseCharacter> actorChar;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	EDSTargetType targetType;
};
