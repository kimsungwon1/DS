
#pragma once

#include "CoreMinimal.h"
#include "DSUserWidget.h"
#include "DSStats.h"
#include "DSPrimaryAttributesWidget.generated.h"

class UTextBlock;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatButtonClicked, EPrimaryStatType, Stat, int32, Direction);

// 주요 스탯(Strength~Charisma) 표시 + -/+ 버튼을 담당. 실제 값 계산은 DSG_CharacterStats 쪽에서 하고
// 여기는 표시(Result/remain 텍스트)와 버튼 클릭 알림(델리게이트)만 담당한다.
UCLASS()
class DS_API UDSPrimaryAttributesWidget : public UDSUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// Stat 하나의 최종값(Result)과 유저가 직접 더한 값(remain)을 동시 갱신
	UFUNCTION(BlueprintCallable)
	void SetStatDisplay(EPrimaryStatType Stat, int32 FinalValue, int32 UserAddedValue);

	UFUNCTION(BlueprintCallable)
	void SetAllStats(const FCharacterStat& FinalStats, const FCharacterStat& UserAddedStats);

	// 버튼(-/+)이 눌리면 이 델리게이트로 알림 (Direction: -1 또는 +1). 실제 반영 여부는 상위(DSG_CharacterStats)가 판단
	UPROPERTY(BlueprintAssignable)
	FOnStatButtonClicked OnStatButtonClicked;

protected:
	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UTextBlock> StrengthResult;
	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UTextBlock> Strengthremain;
	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UButton> Strengthminus;
	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UButton> Strengthadd;

	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UTextBlock> IntelligenceResult;
	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UTextBlock> Intelligenceremain;
	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UButton> Intelligenceminus;
	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UButton> Intelligenceadd;

	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UTextBlock> PietyResult;
	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UTextBlock> Pietyremain;
	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UButton> Pietyminus;
	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UButton> Pietyadd;

	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UTextBlock> VitalityResult;
	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UTextBlock> Vitalityremain;
	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UButton> Vitalityminus;
	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UButton> Vitalityadd;

	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UTextBlock> DexterityResult;
	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UTextBlock> Dexterityremain;
	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UButton> Dexterityminus;
	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UButton> Dexterityadd;

	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UTextBlock> SpeedResult;
	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UTextBlock> Speedremain;
	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UButton> Speedminus;
	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UButton> Speedadd;

	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UTextBlock> CharismaResult;
	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UTextBlock> Charismaremain;
	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UButton> Charismaminus;
	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UButton> Charismaadd;

private:
	UFUNCTION() void OnStrengthMinus();
	UFUNCTION() void OnStrengthPlus();
	UFUNCTION() void OnIntelligenceMinus();
	UFUNCTION() void OnIntelligencePlus();
	UFUNCTION() void OnPietyMinus();
	UFUNCTION() void OnPietyPlus();
	UFUNCTION() void OnVitalityMinus();
	UFUNCTION() void OnVitalityPlus();
	UFUNCTION() void OnDexterityMinus();
	UFUNCTION() void OnDexterityPlus();
	UFUNCTION() void OnSpeedMinus();
	UFUNCTION() void OnSpeedPlus();
	UFUNCTION() void OnCharismaMinus();
	UFUNCTION() void OnCharismaPlus();
};
