
#pragma once

#include "CoreMinimal.h"
#include "DSUserWidget.h"
#include "Spell.h"
#include "DSStats.h"
#include "DSSpellAddWidget.generated.h"

class UScrollBox;
class UDSSpellData;
class USpellEntryWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpellAddSelectionChanged, UDSSpellData*, SpellData);

// DSG_SpellAdd 전용. 5개 마법 영역(Aether/Water/Air/Earth/Sun)별로 배울 수 있는 스펠 목록을 보여줌.
// 목록 채우는 방식은 SpellCastWidget과 동일 (USpellEntryWidget 재사용).
// 여러 개를 토글(Cyan 선택/White 해제)로 고를 수 있고, 실제 포인트 차감/최종 확정은 상위(DSCharacterStatsWidget)가 담당.
UCLASS()
class DS_API UDSSpellAddWidget : public UDSUserWidget
{
	GENERATED_BODY()

public:
	// Skill(Thaumaturgy/Theology)과 이미 아는 스펠 기준으로 5개 리스트를 채움 (페이지 처음 들어올 때 1번만 호출)
	UFUNCTION(BlueprintCallable)
	void PopulateSpells(const FSkillStat& Skill, const FKnownSpells& AlreadyKnown);

	// 마지막 PopulateSpells 결과, 배울 수 있는 스펠이 하나라도 있었는지
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasAnyLearnableSpell() const { return bHasAnyLearnable; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsSpellSelected(UDSSpellData* Spell) const;

	// Cyan(선택)/White(해제) 하이라이트만 바꿈 - 포인트 증감은 상위가 알아서 처리
	UFUNCTION(BlueprintCallable)
	void SetSpellSelected(UDSSpellData* Spell, bool bSelected);

	// 지금 Cyan으로 선택되어있는 스펠 전부 (최종 확정 시 상위가 이걸 읽어서 실제로 배움 처리)
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UDSSpellData*> GetSelectedSpells() const;

	// 선택 전부 해제 (취소/리셋 시 사용)
	UFUNCTION(BlueprintCallable)
	void ClearAllSelections();

	// 목록에서 항목 클릭 시 알림 (선택/해제 여부는 상위가 IsSpellSelected로 판단해서 처리)
	UPROPERTY(BlueprintAssignable)
	FOnSpellAddSelectionChanged OnSpellEntryClicked;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UScrollBox> SpellListBox_Aether;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UScrollBox> SpellListBox_Water;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UScrollBox> SpellListBox_Air;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UScrollBox> SpellListBox_Earth;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UScrollBox> SpellListBox_Sun;

	// 스펠 항목 위젯 클래스 (SpellCastWidget과 동일하게 WBP_SpellEntry 지정)
	UPROPERTY(EditDefaultsOnly, Category = "Spell")
	TSubclassOf<USpellEntryWidget> SpellEntryClass;

private:
	void PopulateRealmList(UScrollBox* ListBox, ESpellElement Element, const FSkillStat& Skill, const TArray<TSoftObjectPtr<UDSSpellData>>& AlreadyKnown);

	UFUNCTION() void HandleSpellEntryClicked(UDSSpellData* SpellData);

	USpellEntryWidget* FindEntry(UDSSpellData* Spell) const;

	bool bHasAnyLearnable = false;
};
