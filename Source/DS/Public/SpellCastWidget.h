
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Spell.h"
#include "DSUserWidget.h"
#include "SpellCastWidget.generated.h"

class UDSSpellData;
class USpellCast;
class USpellEntryWidget;
class UButton;
class UScrollBox;
class UDSUserWidget;

UCLASS()
class DS_API USpellCastWidget : public UDSUserWidget
{
	GENERATED_BODY()

public:
	// 위젯 열릴 때 호출 — InSpellCast의 시전자(GetActor())가 실제로 배운 스펠만 목록에 넣음
	UFUNCTION(BlueprintCallable)
	void InitializeWidget(USpellCast* InSpellCast);

	// 원소 버튼 클릭 시 호출
	UFUNCTION(BlueprintCallable)
	void OnElementSelected(ESpellElement Element);

	// 스펠 목록에서 항목 클릭 시 호출
	UFUNCTION(BlueprintCallable)
	void OnSpellSelected(UDSSpellData* SpellData);

	// 스펠 레벨(1~7) 변경 시 호출
	UFUNCTION(BlueprintCallable)
	void DiceClicked(int32 NewLevel);

	// Cancel 버튼
	UFUNCTION(BlueprintCallable)
	void OnCancelClicked();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UDSSpellData*> GetFilteredSpells() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetSelectedLevel() const { return SelectedLevel; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UDSSpellData* GetSelectedSpell() const { return SelectedSpell; }

protected:
	virtual void NativeConstruct() override;

	void RefreshSpellList();
	void RefreshDiceDisplay();
	void RefreshSpellInfo();

	// 주사위 선택 상태가 바뀔 때(스펠 선택/취소, 세기 변경) 한 번만 호출됨.
	// BP에서 이 이벤트를 구현해 세기별 글로우 이미지 Visibility를 여기서 Set (매틱 바인딩 대체).
	UFUNCTION(BlueprintImplementableEvent, Category = "Dice")
	void OnDiceStateChanged(int32 InSelectedLevel, bool bHasSpellSelected);

protected:
	// ===== 원소 버튼 (Blueprint에서 5각형 배치) =====
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Aether;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Sun;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Earth;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Water;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Air;

	// ===== 주사위 레벨 버튼 (1~6) =====
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> DiceBtn_1;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> DiceBtn_2;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> DiceBtn_3;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> DiceBtn_4;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> DiceBtn_5;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> DiceBtn_6;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UButton> DiceBtn_7;

	// ===== 스펠 목록 =====
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UScrollBox> SpellListBox;

	// ===== CANCEL =====
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> CancelButton;

	// 스펠 항목 위젯 클래스 (BP에서 WBP_SpellEntry 지정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<USpellEntryWidget> SpellEntryClass;


private:
	UPROPERTY()
	TObjectPtr<USpellCast> SpellCastAction;

	UPROPERTY()
	TArray<TObjectPtr<UDSSpellData>> KnownSpells;

	UPROPERTY()
	TObjectPtr<UDSSpellData> SelectedSpell;

	ESpellElement SelectedElement = ESpellElement::Aether;
	int32 SelectedLevel = 1;

	UFUNCTION() void HandleAetherClicked();
	UFUNCTION() void HandleSunClicked();
	UFUNCTION() void HandleEarthClicked();
	UFUNCTION() void HandleWaterClicked();
	UFUNCTION() void HandleAirClicked();
	UFUNCTION() void HandleDice1Clicked();
	UFUNCTION() void HandleDice2Clicked();
	UFUNCTION() void HandleDice3Clicked();
	UFUNCTION() void HandleDice4Clicked();
	UFUNCTION() void HandleDice5Clicked();
	UFUNCTION() void HandleDice6Clicked();
	UFUNCTION() void HandleDice7Clicked();
	UFUNCTION() void HandleCancelClicked();
	UFUNCTION() void HandleSpellEntryClicked(UDSSpellData* InSpellData);

	void UpdateEntrySelection();

	// ===== 주사위 그래프 색 =====
	// 성공 확률에 따른 그래프 색. 확률 계산식은 기획 확정 전까지 placeholder.
	enum class EDiceGraphColor : uint8 { Black, Red, Orange, Yellow, Green };

	// 해당 레벨로 시전 가능한 MP인지. MP 시스템 미구현 — placeholder.
	bool CanAffordLevel(int32 Level) const;

	// 해당 레벨의 성공 확률(0~1). 캐릭터 실력 기반 계산식은 기획 후 교체.
	float GetSuccessChance(int32 Level) const;

	EDiceGraphColor GetDiceGraphColor(int32 Level) const;
	void SetDiceButtonGraph(UButton* Button, int32 Level, EDiceGraphColor Color);
};
