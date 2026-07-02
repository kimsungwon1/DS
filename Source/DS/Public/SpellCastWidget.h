
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Spell.h"
#include "SpellCastWidget.generated.h"

class UDSSpellData;
class USpellCast;
class USpellEntryWidget;
class UButton;
class UScrollBox;

UCLASS()
class DS_API USpellCastWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 위젯 열릴 때 호출 — 해당 캐릭터의 KnownSpells 세팅
	UFUNCTION(BlueprintCallable)
	void InitializeWidget(USpellCast* InSpellCast, const TArray<UDSSpellData*>& InKnownSpells);

	// 원소 버튼 클릭 시 호출
	UFUNCTION(BlueprintCallable)
	void OnElementSelected(ESpellElement Element);

	// 스펠 목록에서 항목 클릭 시 호출
	UFUNCTION(BlueprintCallable)
	void OnSpellSelected(UDSSpellData* SpellData);

	// 스펠 레벨(1~6) 변경 시 호출
	UFUNCTION(BlueprintCallable)
	void OnLevelChanged(int32 NewLevel);

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
	UFUNCTION() void HandleCancelClicked();
	UFUNCTION() void HandleSpellEntryClicked(UDSSpellData* InSpellData);
};
