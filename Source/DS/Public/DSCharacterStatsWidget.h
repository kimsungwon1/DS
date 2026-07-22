
#pragma once

#include "CoreMinimal.h"
#include "DSUserWidget.h"
#include "DSStats.h"
#include "SaveGameData.h"
#include "Styling/SlateBrush.h"
#include "DSCharacterStatsWidget.generated.h"

class UDSComboBoxWidget;
class UDSPrimaryAttributesWidget;
class UDSSkillsWidget;
class UDSAttributesWidget;
class UDSPortraitChooserWidget;
class UDSSpellAddWidget;
class UDSSpellData;
class UTextBlock;
class UMultiLineEditableTextBox;
class UEditableText;
class UCharacterClass;
class UDSRaceData;
class UWidgetSwitcher;
class UButton;
class UTexture2D;
class UImage;
class USlider;
class UDSMessageBoxWidget;
class UDSMessageBoxYesNoWidget;

// 취소(X) 눌렸을 때 상위(예: WBP_GatherParty)에 알림 - 실제로 GatherParty로 돌아가는 건 상위가 처리
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterCreationCancelled);
// 캐릭터 등록(저장)까지 끝났을 때 상위에 알림
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterCreated);

UCLASS()
class DS_API UDSCharacterStatsWidget : public UDSUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// BonusPointsRemaining == 0 이어야 다음 단계로 진행 가능
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanProceed() const { return BonusRemaining == 0; }

	// 지금까지 편집한 캐릭터 데이터를 최종본으로 가져옴 (저장할 때 사용)
	UFUNCTION(BlueprintCallable, BlueprintPure)
	const FPlayerCharacterSaveData& GetWorkingCharacter() const { return WorkingCharacter; }

	// X 눌러서 취소하면 알림 (GatherParty로 돌아가는 실제 처리는 상위가 담당)
	UPROPERTY(BlueprintAssignable)
	FOnCharacterCreationCancelled OnCharacterCreationCancelled;

	// 캐릭터 등록(저장) 완료 시 알림 - 이후 GatherParty로 돌아가는 처리는 상위가 담당
	UPROPERTY(BlueprintAssignable)
	FOnCharacterCreated OnCharacterCreated;

	// 이미 저장된 캐릭터를 읽기 전용으로 보여줌 (Review Character 버튼용) - 편집/보너스/저장 로직 전부 비활성화됨
	UFUNCTION(BlueprintCallable)
	void EnterReviewMode(const FPlayerCharacterSaveData& CharacterData);

	// 리뷰 모드에서 X 또는 Confirm(마지막 페이지) 눌러서 닫을 때 알림 - 저장/취소 이벤트는 안 터짐
	UPROPERTY(BlueprintAssignable)
	FOnCharacterCreationCancelled OnReviewClosed;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UWidgetSwitcher> WidgetSwitcher_0;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UButton> Btn_Cancel;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UButton> Btn_Back;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UButton> Btn_Confirm;
	// Btn_Confirm 안의 라벨 - 마지막 페이지가 아니면 "→", 마지막 페이지면 "V"
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UTextBlock> Btn_Confirm_Label_1;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UDSComboBoxWidget> ComboBox_Pro;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UDSComboBoxWidget> ComboBox_Race;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UDSComboBoxWidget> ComboBox_Gender;

	// 캐릭터 이름 입력란
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UEditableText> EditableText_48;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UDSPrimaryAttributesWidget> DSG_PrimaryAttributes;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UDSSkillsWidget> DSG_Skills;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UDSSpellAddWidget> DSG_SpellAdd;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UDSPortraitChooserWidget> DSG_PortraitChooser;

	// 왼쪽 위 캐릭터 초상화 (DSG_PortraitChooser에서 고른 사진, 혹은 리뷰 모드일 땐 저장된 캐릭터의 초상화)
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UImage> Portrait;

	// 종족+젠더 / 직업 요약 텍스트 (왼쪽 정보란)
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UTextBlock> GenderRace;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UTextBlock> Profession;

	// 페이지별로 바뀌는 설명 텍스트
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UMultiLineEditableTextBox> MultiLineEditableTextBox_111;

	// "Bonus Points Remaining" 값 표시용 (라벨 아니고 "0/0" 형태의 값 텍스트) - 페이지에 따라 스탯/스킬 보너스를 번갈아 표시
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UTextBlock> TextBlock_13;

	// 종족 마법 저항력 5개
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UTextBlock> Resistance_Eather;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UTextBlock> Resistance_Sun;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UTextBlock> TextBlock_Air;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UTextBlock> Resistance_Water;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UTextBlock> Resistance_Earth;

	// 카르마 - 드래그로 값을 정하는 슬라이더. Value(0~1) * 18이 실제 카르마 점수
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<USlider> KarmaSlider;
	// 카르마 숫자 표시용 텍스트박스 (원래 이름 KarmaPoint로 바뀜)
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UTextBlock> KarmaPoint;
	// 카르마 구간 이름 표시 (Doom/Evil/Neutral/Good/Saint)
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UTextBlock> karmaResult;

	// 종족/직업/스탯/스킬에 따라 자동 계산되는 파생 스탯 표시
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UTextBlock> HitPoints;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UTextBlock> SpellPoints;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UTextBlock> ArmorPoints;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UTextBlock> CarryCapacityPoints;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterCreation")
	int32 BonusPointsTotal = 10;

	// 캐릭터 생성 단계에서 스탯이 오를 수 있는 최댓값 (레벨업으로는 더 오를 수 있음)
	UPROPERTY(EditDefaultsOnly, Category = "CharacterCreation")
	int32 MaxStatAtCreation = 18;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterCreation")
	int32 SkillBonusPointsTotal = 15;

	// 스펠 추가 페이지 진입할 때마다(레벨업 1회당) 새로 배울 수 있는 스펠 개수
	UPROPERTY(EditDefaultsOnly, Category = "CharacterCreation")
	int32 SpellPointsTotal = 1;

	// 이름 미입력/중복 시 띄울 메시지박스 (DSG_MessageBox_Close 계열)
	UPROPERTY(EditDefaultsOnly, Category = "CharacterCreation")
	TSubclassOf<UDSMessageBoxWidget> MessageBoxClass;

	// 마지막 확인("정말로 -이름-으로 정하시겠습니까?")용 Yes/No 메시지박스 (DSG_MessageBox_YesNo)
	UPROPERTY(EditDefaultsOnly, Category = "CharacterCreation")
	TSubclassOf<UDSMessageBoxYesNoWidget> MessageBoxYesNoClass;

private:
	void PopulateComboBoxes();

	UFUNCTION() void OnProfessionChanged(FString SelectedItem);
	UFUNCTION() void OnRaceChanged(FString SelectedItem);
	UFUNCTION() void OnGenderChanged(FString SelectedItem);
	UFUNCTION() void OnStatButtonClicked(EPrimaryStatType Stat, int32 Direction);

	UFUNCTION() void OnCancelClicked();
	UFUNCTION() void OnBackClicked();
	UFUNCTION() void OnConfirmClicked();

	UFUNCTION() void OnSkillRowClicked(FName RowName, int32 Direction);
	UFUNCTION() void OnPortraitChanged(UTexture2D* NewPortrait);

	void UpdateConfirmButtonLabel();

	// 스킬 페이지를 벗어날 때 스펠 추가 페이지로 갈지, 건너뛰고 바로 초상화로 갈지 결정
	void GoToSpellAddOrSkip();
	// 스펠 목록에서 항목 클릭 시 - 여기서 바로 토글(선택/해제)하고 SpellBonusRemaining 증감
	UFUNCTION() void OnSpellAddEntryClicked(UDSSpellData* Spell);
	// 최종 Yes 확정 시 그동안 선택(Cyan)해둔 스펠들을 WorkingCharacter.KnownSpells에 실제로 반영
	void CommitSelectedSpells();
	// 지정한 페이지로 전환 + 라벨/설명/보너스 표시 갱신까지 한번에
	void GoToPage(int32 PageIndex);

	// 종족/직업이 바뀔 때: 종족 기본값 + 직업 요구량 자동 반영(보너스 초과 시 마이너스 허용) 재계산
	void RecalculateBaseStats();
	// 화면(Result/remain/BonusPointsRemaining 텍스트) 갱신 + WorkingCharacter 동기화
	void RefreshDisplay();
	void RefreshResistanceDisplay();

	int32 GetBaseValue(EPrimaryStatType Stat) const;
	int32 GetUserAddedValue(EPrimaryStatType Stat) const;
	void SetUserAddedValue(EPrimaryStatType Stat, int32 Value);
	int32 GetRaceFloor(EPrimaryStatType Stat) const;

	// 직업이 바뀔 때 StartingSkillStat 기준으로 스킬 페이지(행 구성 + 보너스) 초기화
	void InitializeSkillPage();
	// BaseSkillStat 기준으로 행 구성 + 표시만 갱신 (BaseSkillStat/SkillBonusRemaining은 안 건드림) - InitializeSkillPage/EnterReviewMode 공용
	void RefreshSkillRows();
	void RefreshSkillDisplay(FName RowName);
	UDSAttributesWidget* FindSkillGroup(FName RowName) const;

	int32 GetSkillBaseValue(FName RowName) const;
	int32 GetSkillUserAddedValue(FName RowName) const;
	void SetSkillUserAddedValue(FName RowName, int32 Value);

	// 왼쪽 정보란(GenderRace/Profession)과 페이지별 설명 텍스트, 보너스 표시 텍스트 갱신
	void RefreshGenderRaceText();
	void UpdateDescriptionText();
	void RefreshBonusPointsDisplay();

	// 이름이 비었거나 이미 존재하면 메시지박스 띄우고 false 리턴 (진행 막음)
	bool ValidateCharacterName();
	void ShowMessageBox(const FText& Message);

	UFUNCTION() void OnFinalizeYesNoResult(bool bYes);

	// 취소/완료 후 다음에 다시 새 캐릭터 만들 때 백지 상태로 보이도록 전체 초기화
	void ResetCreationState();

	UFUNCTION() void OnKarmaSliderChanged(float Value);
	void UpdateKarmaFromSlider();

	// HP/SpellPoints/CarryCapacity 등 공식으로 계산되는 파생 스탯 갱신
	void RefreshDerivedStats(const FCharacterStat& FinalStat);

	TObjectPtr<UCharacterClass> SelectedProfession;
	TObjectPtr<UDSRaceData> SelectedRace;

	// 종족 기본값 + 직업 요구량 자동 top-up까지 반영된 값 (유저 조정 전)
	FCharacterStat BaseComputedStat;
	// 유저가 +/- 버튼으로 직접 더한/뺀 값 (remain 표시값)
	FCharacterStat UserAddedStat;

	int32 BonusRemaining = 0;

	// 직업의 StartingSkillStat (스킬 페이지 기준선, -1인 스킬은 아예 행 자체가 없음)
	FSkillStat BaseSkillStat;
	// 유저가 스킬 페이지에서 +/- 버튼으로 직접 배분한 값
	FSkillStat UserAddedSkillStat;

	int32 SkillBonusRemaining = 0;

	// 스펠 추가 페이지에서 아직 안 쓴 포인트 - 배울 스펠이 없어서 못 쓰면 다음 진입 때까지 이월됨
	int32 SpellBonusRemaining = 0;
	// 스펠 추가 페이지를 건너뛰었는지 (전사/도둑류이거나 배울 수 있는 스펠이 하나도 없었을 때) - Back 눌렀을 때 같이 건너뛰기 위함
	bool bSkippedSpellAdd = false;

	static constexpr int32 PageStats = 0;
	static constexpr int32 PageSkills = 1;
	static constexpr int32 PageSpellAdd = 2;
	static constexpr int32 PagePortrait = 3;

	// true면 읽기 전용 - 스탯/스킬/스펠/카르마/이름/직업 등 모든 편집 핸들러가 no-op됨
	bool bReviewMode = false;

	// Portrait에 아직 아무 텍스처도 안 씌운 시점(NativeConstruct 초입)의 기본 브러시(해골 등) - 선택 없음으로 돌아갈 때 복원용
	FSlateBrush DefaultPortraitBrush;
	bool bCachedDefaultPortraitBrush = false;

	// Profession/Race/Gender/최종 스탯을 전부 모아두는 단일 진실 공급원. 저장할 때 이걸 그대로 씀
	FPlayerCharacterSaveData WorkingCharacter;
};
