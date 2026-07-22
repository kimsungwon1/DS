
#include "DSCharacterStatsWidget.h"
#include "DSComboBoxWidget.h"
#include "DSPrimaryAttributesWidget.h"
#include "DSSkillsWidget.h"
#include "DSAttributesWidget.h"
#include "DSPortraitChooserWidget.h"
#include "DSSpellAddWidget.h"
#include "DSSpellData.h"
#include "Spell.h"
#include "DSMessageBoxWidget.h"
#include "DSMessageBoxYesNoWidget.h"
#include "DSSaveGameSubsystem.h"
#include "Components/TextBlock.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/EditableText.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"
#include "CharacterClass.h"
#include "DSRaceData.h"
#include "CharacterClassRegistry.h"
#include "DSRaceDataRegistry.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

namespace
{
	const FString GenderMale = TEXT("Male");
	const FString GenderFemale = TEXT("Female");

	const FText DescriptionText_StatsPage = FText::FromString(TEXT("Choose your gender, race, profession. Then distribute your remain bonus attributes."));
	const FText DescriptionText_SkillsPage = FText::FromString(TEXT("Distribute your skill points."));
	const FText DescriptionText_SpellAddPage = FText::FromString(TEXT("Choose your Spell."));
	const FText DescriptionText_PortraitPage = FText::FromString(TEXT("Choose your portrait."));

	const TArray<FName> WeaponSkillNames = { TEXT("Sword"), TEXT("HandsAndFeet"), TEXT("Axe"), TEXT("Bow"), TEXT("MaceAndFlail"), TEXT("PoleAndStaff"), TEXT("Shield"), TEXT("Sling"), TEXT("Throw"), TEXT("WandAndDagger") };
	const TArray<FName> PhysicalSkillNames = { TEXT("Stealth"), TEXT("Oratory"), TEXT("Skulduggery") };
	const TArray<FName> AcademiaSkillNames = { TEXT("Artifacts"), TEXT("Kirijutsu"), TEXT("Thaumaturgy"), TEXT("Theology") };

	bool FindProfessionByDisplayName(const FString& Name, EProfession& OutValue)
	{
		const UEnum* EnumPtr = StaticEnum<EProfession>();
		if (!EnumPtr) return false;
		for (int32 i = 0; i < EnumPtr->NumEnums(); ++i)
		{
			if (EnumPtr->GetDisplayNameTextByIndex(i).ToString() == Name)
			{
				OutValue = static_cast<EProfession>(EnumPtr->GetValueByIndex(i));
				return true;
			}
		}
		return false;
	}

	FString GetProfessionDisplayName(EProfession Value)
	{
		const UEnum* EnumPtr = StaticEnum<EProfession>();
		return EnumPtr ? EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(Value)).ToString() : FString();
	}
}

void UDSCharacterStatsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
	{
		return;
	}

	// 실제 텍스처 씌우기 전, 디자이너에 세팅해둔 기본 브러시(해골 등)를 캐싱 - 선택 없음으로 돌아갈 때 이걸로 복원
	if (!bCachedDefaultPortraitBrush && IsValid(Portrait))
	{
		DefaultPortraitBrush = Portrait->GetBrush();
		bCachedDefaultPortraitBrush = true;
	}

	if (IsValid(ComboBox_Pro))
	{
		ComboBox_Pro->OnSelectionChangedEvent.RemoveDynamic(this, &UDSCharacterStatsWidget::OnProfessionChanged);
		ComboBox_Pro->OnSelectionChangedEvent.AddDynamic(this, &UDSCharacterStatsWidget::OnProfessionChanged);
	}
	if (IsValid(ComboBox_Race))
	{
		ComboBox_Race->OnSelectionChangedEvent.RemoveDynamic(this, &UDSCharacterStatsWidget::OnRaceChanged);
		ComboBox_Race->OnSelectionChangedEvent.AddDynamic(this, &UDSCharacterStatsWidget::OnRaceChanged);
	}
	if (IsValid(ComboBox_Gender))
	{
		ComboBox_Gender->OnSelectionChangedEvent.RemoveDynamic(this, &UDSCharacterStatsWidget::OnGenderChanged);
		ComboBox_Gender->OnSelectionChangedEvent.AddDynamic(this, &UDSCharacterStatsWidget::OnGenderChanged);
	}
	if (IsValid(DSG_PrimaryAttributes))
	{
		DSG_PrimaryAttributes->OnStatButtonClicked.RemoveDynamic(this, &UDSCharacterStatsWidget::OnStatButtonClicked);
		DSG_PrimaryAttributes->OnStatButtonClicked.AddDynamic(this, &UDSCharacterStatsWidget::OnStatButtonClicked);
	}
	if (IsValid(Btn_Cancel))
	{
		Btn_Cancel->OnClicked.RemoveDynamic(this, &UDSCharacterStatsWidget::OnCancelClicked);
		Btn_Cancel->OnClicked.AddDynamic(this, &UDSCharacterStatsWidget::OnCancelClicked);
	}
	if (IsValid(Btn_Back))
	{
		Btn_Back->OnClicked.RemoveDynamic(this, &UDSCharacterStatsWidget::OnBackClicked);
		Btn_Back->OnClicked.AddDynamic(this, &UDSCharacterStatsWidget::OnBackClicked);
	}
	if (IsValid(Btn_Confirm))
	{
		Btn_Confirm->OnClicked.RemoveDynamic(this, &UDSCharacterStatsWidget::OnConfirmClicked);
		Btn_Confirm->OnClicked.AddDynamic(this, &UDSCharacterStatsWidget::OnConfirmClicked);
	}
	if (IsValid(DSG_Skills))
	{
		if (IsValid(DSG_Skills->DSG_WeaponarySkillAttributes))
		{
			DSG_Skills->DSG_WeaponarySkillAttributes->OnAttributeButtonClicked.RemoveDynamic(this, &UDSCharacterStatsWidget::OnSkillRowClicked);
			DSG_Skills->DSG_WeaponarySkillAttributes->OnAttributeButtonClicked.AddDynamic(this, &UDSCharacterStatsWidget::OnSkillRowClicked);
		}
		if (IsValid(DSG_Skills->DSG_PhysicalSkillAttributes))
		{
			DSG_Skills->DSG_PhysicalSkillAttributes->OnAttributeButtonClicked.RemoveDynamic(this, &UDSCharacterStatsWidget::OnSkillRowClicked);
			DSG_Skills->DSG_PhysicalSkillAttributes->OnAttributeButtonClicked.AddDynamic(this, &UDSCharacterStatsWidget::OnSkillRowClicked);
		}
		if (IsValid(DSG_Skills->DSG_AcademiaSkillAttributes))
		{
			DSG_Skills->DSG_AcademiaSkillAttributes->OnAttributeButtonClicked.RemoveDynamic(this, &UDSCharacterStatsWidget::OnSkillRowClicked);
			DSG_Skills->DSG_AcademiaSkillAttributes->OnAttributeButtonClicked.AddDynamic(this, &UDSCharacterStatsWidget::OnSkillRowClicked);
		}
	}
	if (IsValid(DSG_PortraitChooser))
	{
		DSG_PortraitChooser->OnPortraitChanged.RemoveDynamic(this, &UDSCharacterStatsWidget::OnPortraitChanged);
		DSG_PortraitChooser->OnPortraitChanged.AddDynamic(this, &UDSCharacterStatsWidget::OnPortraitChanged);

		// DSG_PortraitChooser는 자식이라 부모(이 위젯)보다 먼저 NativeConstruct되면서 첫 초상화를
		// 이미 골라 OnPortraitChanged를 쐈을 수 있음 - 그땐 아직 바인딩 전이라 놓치니 여기서 한 번 더 동기화
		WorkingCharacter.Portrait = DSG_PortraitChooser->GetSelectedPortrait();
	}
	if (IsValid(KarmaSlider))
	{
		KarmaSlider->OnValueChanged.RemoveDynamic(this, &UDSCharacterStatsWidget::OnKarmaSliderChanged);
		KarmaSlider->OnValueChanged.AddDynamic(this, &UDSCharacterStatsWidget::OnKarmaSliderChanged);
	}
	if (IsValid(DSG_SpellAdd))
	{
		DSG_SpellAdd->OnSpellEntryClicked.RemoveDynamic(this, &UDSCharacterStatsWidget::OnSpellAddEntryClicked);
		DSG_SpellAdd->OnSpellEntryClicked.AddDynamic(this, &UDSCharacterStatsWidget::OnSpellAddEntryClicked);
	}

	PopulateComboBoxes();

	RecalculateBaseStats();
	RefreshResistanceDisplay();
	RefreshDisplay();
	UpdateConfirmButtonLabel();
	UpdateKarmaFromSlider();
}

void UDSCharacterStatsWidget::NativeDestruct()
{
	if (IsValid(ComboBox_Pro))
	{
		ComboBox_Pro->OnSelectionChangedEvent.RemoveDynamic(this, &UDSCharacterStatsWidget::OnProfessionChanged);
	}
	if (IsValid(ComboBox_Race))
	{
		ComboBox_Race->OnSelectionChangedEvent.RemoveDynamic(this, &UDSCharacterStatsWidget::OnRaceChanged);
	}
	if (IsValid(ComboBox_Gender))
	{
		ComboBox_Gender->OnSelectionChangedEvent.RemoveDynamic(this, &UDSCharacterStatsWidget::OnGenderChanged);
	}
	if (IsValid(DSG_PrimaryAttributes))
	{
		DSG_PrimaryAttributes->OnStatButtonClicked.RemoveDynamic(this, &UDSCharacterStatsWidget::OnStatButtonClicked);
	}
	if (IsValid(Btn_Cancel))
	{
		Btn_Cancel->OnClicked.RemoveDynamic(this, &UDSCharacterStatsWidget::OnCancelClicked);
	}
	if (IsValid(Btn_Back))
	{
		Btn_Back->OnClicked.RemoveDynamic(this, &UDSCharacterStatsWidget::OnBackClicked);
	}
	if (IsValid(Btn_Confirm))
	{
		Btn_Confirm->OnClicked.RemoveDynamic(this, &UDSCharacterStatsWidget::OnConfirmClicked);
	}
	if (IsValid(DSG_Skills))
	{
		if (IsValid(DSG_Skills->DSG_WeaponarySkillAttributes))
		{
			DSG_Skills->DSG_WeaponarySkillAttributes->OnAttributeButtonClicked.RemoveDynamic(this, &UDSCharacterStatsWidget::OnSkillRowClicked);
		}
		if (IsValid(DSG_Skills->DSG_PhysicalSkillAttributes))
		{
			DSG_Skills->DSG_PhysicalSkillAttributes->OnAttributeButtonClicked.RemoveDynamic(this, &UDSCharacterStatsWidget::OnSkillRowClicked);
		}
		if (IsValid(DSG_Skills->DSG_AcademiaSkillAttributes))
		{
			DSG_Skills->DSG_AcademiaSkillAttributes->OnAttributeButtonClicked.RemoveDynamic(this, &UDSCharacterStatsWidget::OnSkillRowClicked);
		}
	}
	if (IsValid(DSG_PortraitChooser))
	{
		DSG_PortraitChooser->OnPortraitChanged.RemoveDynamic(this, &UDSCharacterStatsWidget::OnPortraitChanged);
	}
	if (IsValid(KarmaSlider))
	{
		KarmaSlider->OnValueChanged.RemoveDynamic(this, &UDSCharacterStatsWidget::OnKarmaSliderChanged);
	}
	if (IsValid(DSG_SpellAdd))
	{
		DSG_SpellAdd->OnSpellEntryClicked.RemoveDynamic(this, &UDSCharacterStatsWidget::OnSpellAddEntryClicked);
	}

	Super::NativeDestruct();
}

void UDSCharacterStatsWidget::OnKarmaSliderChanged(float Value)
{
	if (bReviewMode) return;
	UpdateKarmaFromSlider();
}

void UDSCharacterStatsWidget::UpdateKarmaFromSlider()
{
	if (!IsValid(KarmaSlider)) return;

	const int32 KarmaValue = FMath::RoundToInt(KarmaSlider->GetValue() * 18.f);

	WorkingCharacter.characterData.BaseFaceStat.Karma = KarmaValue;
	WorkingCharacter.characterData.FinalFaceStat.Karma = KarmaValue;

	if (KarmaPoint)
	{
		KarmaPoint->SetText(FText::AsNumber(KarmaValue));
	}

	if (karmaResult)
	{
		FString ResultText;
		FLinearColor ResultColor;
		if (KarmaValue <= 2) { ResultText = TEXT("Doom");    ResultColor = FLinearColor(0.273f, 0.f, 0.f); }
		else if (KarmaValue <= 5) { ResultText = TEXT("Evil");    ResultColor = FLinearColor(1.f, 0.f, 0.f); }
		else if (KarmaValue <= 11) { ResultText = TEXT("Neutral"); ResultColor = FLinearColor(1.f, 1.f, 0.f); }
		else if (KarmaValue <= 15) { ResultText = TEXT("Good");    ResultColor = FLinearColor(0.f, 0.35f, 1.f); }
		else { ResultText = TEXT("Saint");   ResultColor = FLinearColor(0.f, 1.f, 1.f); }

		karmaResult->SetText(FText::FromString(ResultText));
		karmaResult->SetColorAndOpacity(FSlateColor(ResultColor));
	}
}

void UDSCharacterStatsWidget::OnCancelClicked()
{
	if (bReviewMode)
	{
		ResetCreationState();
		OnReviewClosed.Broadcast();
		return;
	}

	OnCharacterCreationCancelled.Broadcast();
	ResetCreationState();
}

void UDSCharacterStatsWidget::OnBackClicked()
{
	if (!IsValid(WidgetSwitcher_0)) return;

	const int32 CurrentIndex = WidgetSwitcher_0->GetActiveWidgetIndex();
	int32 NewIndex = FMath::Max(CurrentIndex - 1, 0);

	// 스펠 추가 페이지를 건너뛰고 왔으면 되돌아갈 때도 건너뜀
	if (CurrentIndex == PagePortrait && bSkippedSpellAdd)
	{
		NewIndex = PageSkills;
	}

	GoToPage(NewIndex);
}

void UDSCharacterStatsWidget::OnConfirmClicked()
{
	if (!IsValid(WidgetSwitcher_0)) return;

	const int32 CurrentIndex = WidgetSwitcher_0->GetActiveWidgetIndex();

	if (CurrentIndex == PageStats && BonusRemaining != 0)
	{
		return;
	}
	// 스킬 페이지에서 넘어가려면 스킬 보너스를 전부 배분해야 함
	if (CurrentIndex == PageSkills && SkillBonusRemaining != 0)
	{
		return;
	}

	if (CurrentIndex == PageSkills)
	{
		GoToSpellAddOrSkip();
		return;
	}

	// 스펠 추가 페이지에서 넘어가려면 포인트를 전부 써야 함 (배울 스펠이 아예 없는 경우는 진입 시점에 걸러져서 여기 안 옴)
	if (CurrentIndex == PageSpellAdd && SpellBonusRemaining != 0)
	{
		return;
	}

	const int32 LastIndex = WidgetSwitcher_0->GetNumWidgets() - 1;

	// 리뷰 모드에서는 마지막 페이지 Confirm이 저장 대신 그냥 리뷰 닫기
	if (bReviewMode && CurrentIndex == LastIndex)
	{
		ResetCreationState();
		OnReviewClosed.Broadcast();
		return;
	}

	// 마지막 페이지(포트레이트)에서 Confirm(V)을 누르면 여기서 실제로 등록을 시도 - 이름부터 검증
	if (CurrentIndex == LastIndex)
	{
		if (!ValidateCharacterName()) return;

		if (MessageBoxYesNoClass)
		{
			const FText Message = FText::FromString(FString::Printf(TEXT("Are you sure you want to finalize this character as %s?"), *WorkingCharacter.characterName.ToString()));
			if (UDSMessageBoxYesNoWidget* MsgBox = UDSMessageBoxYesNoWidget::Show(this, MessageBoxYesNoClass, Message))
			{
				MsgBox->OnResult.AddDynamic(this, &UDSCharacterStatsWidget::OnFinalizeYesNoResult);
			}
		}
		return;
	}

	GoToPage(FMath::Min(CurrentIndex + 1, LastIndex));
}

void UDSCharacterStatsWidget::GoToPage(int32 PageIndex)
{
	if (!IsValid(WidgetSwitcher_0)) return;

	WidgetSwitcher_0->SetActiveWidgetIndex(PageIndex);
	UpdateConfirmButtonLabel();
	UpdateDescriptionText();
	RefreshBonusPointsDisplay();
}

void UDSCharacterStatsWidget::GoToSpellAddOrSkip()
{
	// 리뷰 모드는 새로 배울 게 없으니(이미 아는 스펠만 보여줄 뿐) 조건 체크/경고 메시지 없이 그냥 건너뜀
	if (bReviewMode)
	{
		bSkippedSpellAdd = true;
		GoToPage(PagePortrait);
		return;
	}

	const bool bCanLearnMagic = (WorkingCharacter.SkillStat.Thaumaturgy != -1) || (WorkingCharacter.SkillStat.Theology != -1);

	if (!bCanLearnMagic)
	{
		// 전사/도둑 등 마법 못 쓰는 직업 - 메시지 없이 그냥 초상화로
		bSkippedSpellAdd = true;
		GoToPage(PagePortrait);
		return;
	}

	if (IsValid(DSG_SpellAdd))
	{
		DSG_SpellAdd->PopulateSpells(WorkingCharacter.SkillStat, WorkingCharacter.KnownSpells);
	}

	const bool bHasLearnable = IsValid(DSG_SpellAdd) && DSG_SpellAdd->HasAnyLearnableSpell();
	if (!bHasLearnable)
	{
		// 스킬(Thaumaturgy/Theology)이 낮아서 배울 수 있는 스펠이 하나도 없음 - 점수는 안 쓴 채로 이월(누적)됨
		ShowMessageBox(FText::FromString(TEXT("No spells available to learn. Raise your Thaumaturgy or Theology skill.")));
		bSkippedSpellAdd = true;
		GoToPage(PagePortrait);
		return;
	}

	SpellBonusRemaining += SpellPointsTotal;
	bSkippedSpellAdd = false;

	// 이 페이지 처음 들어올 때 딱 한 번만 목록을 채움 - 이후엔 다시 안 채워서
	// 선택(Cyan) 해둔 항목이 페이지를 나갔다 돌아와도 그대로 남아있음
	if (IsValid(DSG_SpellAdd))
	{
		DSG_SpellAdd->PopulateSpells(WorkingCharacter.SkillStat, WorkingCharacter.KnownSpells);
	}

	GoToPage(PageSpellAdd);
}

void UDSCharacterStatsWidget::OnSpellAddEntryClicked(UDSSpellData* Spell)
{
	if (bReviewMode || !IsValid(DSG_SpellAdd) || !Spell) return;

	if (DSG_SpellAdd->IsSpellSelected(Spell))
	{
		// 이미 선택된 걸 다시 클릭 - 해제하고 포인트 돌려줌
		DSG_SpellAdd->SetSpellSelected(Spell, false);
		SpellBonusRemaining += 1;
	}
	else
	{
		if (SpellBonusRemaining <= 0) return;

		DSG_SpellAdd->SetSpellSelected(Spell, true);
		SpellBonusRemaining -= 1;
	}

	RefreshBonusPointsDisplay();
}

void UDSCharacterStatsWidget::CommitSelectedSpells()
{
	if (!IsValid(DSG_SpellAdd)) return;

	for (UDSSpellData* Spell : DSG_SpellAdd->GetSelectedSpells())
	{
		if (!Spell) continue;

		const TSoftObjectPtr<UDSSpellData> SoftSpell(Spell);
		switch (Spell->spellStat.element)
		{
		case ESpellElement::Aether: WorkingCharacter.KnownSpells.KnownSpells_Aether.Add(SoftSpell); break;
		case ESpellElement::Water:  WorkingCharacter.KnownSpells.KnownSpells_Water.Add(SoftSpell);  break;
		case ESpellElement::Air:    WorkingCharacter.KnownSpells.KnownSpells_Air.Add(SoftSpell);    break;
		case ESpellElement::Earth:  WorkingCharacter.KnownSpells.KnownSpells_Earth.Add(SoftSpell);  break;
		case ESpellElement::Sun:    WorkingCharacter.KnownSpells.KnownSpells_Sun.Add(SoftSpell);    break;
		}
	}

	// 아는 스펠 수가 늘었으니 SpellPoints(SP) 최댓값도 다시 계산
	RefreshDerivedStats(WorkingCharacter.characterData.CharStat);
}

void UDSCharacterStatsWidget::UpdateConfirmButtonLabel()
{
	if (!IsValid(WidgetSwitcher_0) || !IsValid(Btn_Confirm_Label_1)) return;

	const bool bIsLastPage = WidgetSwitcher_0->GetActiveWidgetIndex() == WidgetSwitcher_0->GetNumWidgets() - 1;
	Btn_Confirm_Label_1->SetText(bIsLastPage ? FText::FromString(TEXT("V")) : FText::FromString(TEXT("→")));
}

void UDSCharacterStatsWidget::UpdateDescriptionText()
{
	if (!IsValid(MultiLineEditableTextBox_111) || !IsValid(WidgetSwitcher_0)) return;

	switch (WidgetSwitcher_0->GetActiveWidgetIndex())
	{
	case PageSkills:   MultiLineEditableTextBox_111->SetText(DescriptionText_SkillsPage); break;
	case PageSpellAdd: MultiLineEditableTextBox_111->SetText(DescriptionText_SpellAddPage); break;
	case PagePortrait: MultiLineEditableTextBox_111->SetText(DescriptionText_PortraitPage); break;
	default:           MultiLineEditableTextBox_111->SetText(DescriptionText_StatsPage); break;
	}
}

void UDSCharacterStatsWidget::RefreshBonusPointsDisplay()
{
	if (!IsValid(TextBlock_13)) return;

	const int32 ActiveIndex = IsValid(WidgetSwitcher_0) ? WidgetSwitcher_0->GetActiveWidgetIndex() : PageStats;
	switch (ActiveIndex)
	{
	case PageSkills:
		TextBlock_13->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), SkillBonusRemaining, SkillBonusPointsTotal)));
		break;
	case PageSpellAdd:
		TextBlock_13->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), SpellBonusRemaining, SpellPointsTotal)));
		break;
	default:
		TextBlock_13->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), BonusRemaining, BonusPointsTotal)));
		break;
	}
}

void UDSCharacterStatsWidget::PopulateComboBoxes()
{
	UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr;
	if (!GI) return;

	if (IsValid(ComboBox_Pro))
	{
		ComboBox_Pro->ClearOptions();
		FString FirstOption;
		if (UCharacterClassRegistry* CCR = GI->GetSubsystem<UCharacterClassRegistry>())
		{
			for (UCharacterClass* Class : CCR->GetAllCharacterClasses())
			{
				if (Class)
				{
					const FString OptionName = GetProfessionDisplayName(Class->classType);
					ComboBox_Pro->AddOption(OptionName);
					if (FirstOption.IsEmpty()) FirstOption = OptionName;
				}
			}
		}
		if (!FirstOption.IsEmpty())
		{
			ComboBox_Pro->SetSelectedOption(FirstOption);
		}
	}

	if (IsValid(ComboBox_Race))
	{
		ComboBox_Race->ClearOptions();
		FString FirstOption;
		if (UDSRaceDataRegistry* RDR = GI->GetSubsystem<UDSRaceDataRegistry>())
		{
			for (UDSRaceData* Race : RDR->GetAllRaces())
			{
				if (Race)
				{
					const FString OptionName = Race->RaceName.ToString();
					ComboBox_Race->AddOption(OptionName);
					if (FirstOption.IsEmpty()) FirstOption = OptionName;
				}
			}
		}
		if (!FirstOption.IsEmpty())
		{
			ComboBox_Race->SetSelectedOption(FirstOption);
		}
	}

	if (IsValid(ComboBox_Gender))
	{
		ComboBox_Gender->ClearOptions();
		ComboBox_Gender->AddOption(GenderMale);
		ComboBox_Gender->AddOption(GenderFemale);
		ComboBox_Gender->SetSelectedOption(GenderMale);
	}
}

void UDSCharacterStatsWidget::OnProfessionChanged(FString SelectedItem)
{
	if (bReviewMode) return;

	EProfession ePro;
	if (!FindProfessionByDisplayName(SelectedItem, ePro))
	{
		SelectedProfession = nullptr;
	}
	else if (UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
	{
		if (UCharacterClassRegistry* CCR = GI->GetSubsystem<UCharacterClassRegistry>())
		{
			SelectedProfession = CCR->GetCharacterClassByProfession(ePro);
			WorkingCharacter.Profession = ePro;
		}
	}

	RecalculateBaseStats();
	RefreshDisplay();
	RefreshGenderRaceText();
	InitializeSkillPage();
}

void UDSCharacterStatsWidget::OnRaceChanged(FString SelectedItem)
{
	if (bReviewMode) return;

	SelectedRace = nullptr;

	if (UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
	{
		if (UDSRaceDataRegistry* RDR = GI->GetSubsystem<UDSRaceDataRegistry>())
		{
			for (UDSRaceData* Race : RDR->GetAllRaces())
			{
				if (Race && Race->RaceName.ToString() == SelectedItem)
				{
					SelectedRace = Race;
					WorkingCharacter.Race = Race->eRace;
					break;
				}
			}
		}
	}

	RecalculateBaseStats();
	RefreshResistanceDisplay();
	RefreshDisplay();
	RefreshGenderRaceText();
}

void UDSCharacterStatsWidget::OnGenderChanged(FString SelectedItem)
{
	if (bReviewMode) return;

	const bool bIsMale = (SelectedItem == GenderMale);
	WorkingCharacter.characterData.BaseFaceStat.Gender = bIsMale;
	WorkingCharacter.characterData.FinalFaceStat.Gender = bIsMale;

	RefreshGenderRaceText();
}

void UDSCharacterStatsWidget::RefreshGenderRaceText()
{
	const bool bIsMale = WorkingCharacter.characterData.BaseFaceStat.Gender;
	const FString GenderStr = bIsMale ? GenderMale : GenderFemale;
	const FString RaceStr = SelectedRace ? SelectedRace->RaceName.ToString() : FString();

	if (GenderRace)
	{
		GenderRace->SetText(FText::FromString(FString::Printf(TEXT("%s %s"), *GenderStr, *RaceStr)));
	}
	if (Profession)
	{
		Profession->SetText(FText::FromString(SelectedProfession ? GetProfessionDisplayName(SelectedProfession->classType) : FString()));
	}
}

void UDSCharacterStatsWidget::RecalculateBaseStats()
{
	const bool bHasRace = (SelectedRace != nullptr);
	const bool bHasProfession = (SelectedProfession != nullptr);

	if (!bHasRace && !bHasProfession)
	{
		BaseComputedStat = FCharacterStat();
	}
	else if (bHasProfession && !bHasRace)
	{
		BaseComputedStat = SelectedProfession->neededStats;
	}
	else if (bHasRace && !bHasProfession)
	{
		BaseComputedStat = SelectedRace->DefaultStat;
	}
	else
	{
		// 종족 기본값에서 시작, 직업 요구량 미달 스탯은 보너스 풀 크기와 상관없이 강제로 요구량까지 채움
		const FCharacterStat& Race = SelectedRace->DefaultStat;
		const FCharacterStat& Req = SelectedProfession->neededStats;

		BaseComputedStat = Race;
		BaseComputedStat.Strength     = FMath::Max(Race.Strength, Req.Strength);
		BaseComputedStat.Intelligence = FMath::Max(Race.Intelligence, Req.Intelligence);
		BaseComputedStat.Piety        = FMath::Max(Race.Piety, Req.Piety);
		BaseComputedStat.Vitality     = FMath::Max(Race.Vitality, Req.Vitality);
		BaseComputedStat.Dexterity    = FMath::Max(Race.Dexterity, Req.Dexterity);
		BaseComputedStat.Speed        = FMath::Max(Race.Speed, Req.Speed);
		BaseComputedStat.Charisma     = FMath::Max(Race.Charisma, Req.Charisma);
	}

	// 종족 기본값 대비 자동으로 채워넣은 만큼 보너스 풀에서 소모 (부족하면 마이너스 허용)
	int32 AutoSpent = 0;
	if (bHasRace && bHasProfession)
	{
		const FCharacterStat& Race = SelectedRace->DefaultStat;
		AutoSpent += BaseComputedStat.Strength - Race.Strength;
		AutoSpent += BaseComputedStat.Intelligence - Race.Intelligence;
		AutoSpent += BaseComputedStat.Piety - Race.Piety;
		AutoSpent += BaseComputedStat.Vitality - Race.Vitality;
		AutoSpent += BaseComputedStat.Dexterity - Race.Dexterity;
		AutoSpent += BaseComputedStat.Speed - Race.Speed;
		AutoSpent += BaseComputedStat.Charisma - Race.Charisma;
	}

	// 종족/직업이 바뀌면 유저가 직접 배분한 값은 초기화
	UserAddedStat.Strength = 0;
	UserAddedStat.Intelligence = 0;
	UserAddedStat.Piety = 0;
	UserAddedStat.Vitality = 0;
	UserAddedStat.Dexterity = 0;
	UserAddedStat.Speed = 0;
	UserAddedStat.Charisma = 0;

	BonusRemaining = BonusPointsTotal - AutoSpent;
}

void UDSCharacterStatsWidget::OnStatButtonClicked(EPrimaryStatType Stat, int32 Direction)
{
	if (bReviewMode) return;

	if (Direction > 0)
	{
		// 보너스가 이미 0(혹은 그 이하)이면 더 못 씀 - 이게 없으면 보너스가 오버스펜딩으로 마이너스가 되고,
		// 그러면 아래 minus 로직이 "종족/직업 미스매치로 인한 마이너스"로 착각해서 기본값 밑까지 깎는 걸 허용해버림
		if (BonusRemaining <= 0) return;

		// 캐릭터 생성 단계에서는 스탯 최댓값 18 캡 (20은 레벨업으로만)
		const int32 CurrentFinal = GetBaseValue(Stat) + GetUserAddedValue(Stat);
		if (CurrentFinal >= MaxStatAtCreation) return;

		SetUserAddedValue(Stat, GetUserAddedValue(Stat) + 1);
		BonusRemaining -= 1;
	}
	else
	{
		const int32 CurrentFinal = GetBaseValue(Stat) + GetUserAddedValue(Stat);

		// 평상시엔 종족 기본값 밑으로 못 깎음. 보너스가 마이너스인 특수 상황에서만 예외 허용
		int32 Floor = 1;
		if (BonusRemaining >= 0)
		{
			Floor = FMath::Max(Floor, GetRaceFloor(Stat));
		}

		if (CurrentFinal - 1 < Floor) return;

		SetUserAddedValue(Stat, GetUserAddedValue(Stat) - 1);
		BonusRemaining += 1;
	}

	RefreshDisplay();
}

void UDSCharacterStatsWidget::RefreshDisplay()
{
	FCharacterStat FinalStat = BaseComputedStat;
	FinalStat.Strength += UserAddedStat.Strength;
	FinalStat.Intelligence += UserAddedStat.Intelligence;
	FinalStat.Piety += UserAddedStat.Piety;
	FinalStat.Vitality += UserAddedStat.Vitality;
	FinalStat.Dexterity += UserAddedStat.Dexterity;
	FinalStat.Speed += UserAddedStat.Speed;
	FinalStat.Charisma += UserAddedStat.Charisma;

	// WorkingCharacter에도 최종 스탯 동기화 - 이게 저장할 때 쓰이는 단일 진실 공급원
	WorkingCharacter.characterData.CharStat = FinalStat;

	if (DSG_PrimaryAttributes)
	{
		DSG_PrimaryAttributes->SetAllStats(FinalStat, UserAddedStat);
	}

	RefreshBonusPointsDisplay();
	RefreshDerivedStats(FinalStat);
}

void UDSCharacterStatsWidget::RefreshResistanceDisplay()
{
	const FResistance R = SelectedRace ? SelectedRace->DefaultResistance : FResistance();

	WorkingCharacter.characterData.BaseFaceStat.Resistance = R;
	WorkingCharacter.characterData.FinalFaceStat.Resistance = R;

	if (Resistance_Eather) Resistance_Eather->SetText(FText::AsNumber(R.ResistAether));
	if (Resistance_Sun) Resistance_Sun->SetText(FText::AsNumber(R.ResistSun));
	if (TextBlock_Air) TextBlock_Air->SetText(FText::AsNumber(R.ResistAir));
	if (Resistance_Water) Resistance_Water->SetText(FText::AsNumber(R.ResistWater));
	if (Resistance_Earth) Resistance_Earth->SetText(FText::AsNumber(R.ResistEarth));
}

int32 UDSCharacterStatsWidget::GetBaseValue(EPrimaryStatType Stat) const
{
	switch (Stat)
	{
	case EPrimaryStatType::Strength:     return BaseComputedStat.Strength;
	case EPrimaryStatType::Intelligence: return BaseComputedStat.Intelligence;
	case EPrimaryStatType::Piety:        return BaseComputedStat.Piety;
	case EPrimaryStatType::Vitality:     return BaseComputedStat.Vitality;
	case EPrimaryStatType::Dexterity:    return BaseComputedStat.Dexterity;
	case EPrimaryStatType::Speed:        return BaseComputedStat.Speed;
	case EPrimaryStatType::Charisma:     return BaseComputedStat.Charisma;
	}
	return 0;
}

int32 UDSCharacterStatsWidget::GetUserAddedValue(EPrimaryStatType Stat) const
{
	switch (Stat)
	{
	case EPrimaryStatType::Strength:     return UserAddedStat.Strength;
	case EPrimaryStatType::Intelligence: return UserAddedStat.Intelligence;
	case EPrimaryStatType::Piety:        return UserAddedStat.Piety;
	case EPrimaryStatType::Vitality:     return UserAddedStat.Vitality;
	case EPrimaryStatType::Dexterity:    return UserAddedStat.Dexterity;
	case EPrimaryStatType::Speed:        return UserAddedStat.Speed;
	case EPrimaryStatType::Charisma:     return UserAddedStat.Charisma;
	}
	return 0;
}

void UDSCharacterStatsWidget::SetUserAddedValue(EPrimaryStatType Stat, int32 Value)
{
	switch (Stat)
	{
	case EPrimaryStatType::Strength:     UserAddedStat.Strength = Value;     break;
	case EPrimaryStatType::Intelligence: UserAddedStat.Intelligence = Value; break;
	case EPrimaryStatType::Piety:        UserAddedStat.Piety = Value;        break;
	case EPrimaryStatType::Vitality:     UserAddedStat.Vitality = Value;     break;
	case EPrimaryStatType::Dexterity:    UserAddedStat.Dexterity = Value;    break;
	case EPrimaryStatType::Speed:        UserAddedStat.Speed = Value;        break;
	case EPrimaryStatType::Charisma:     UserAddedStat.Charisma = Value;     break;
	}
}

int32 UDSCharacterStatsWidget::GetRaceFloor(EPrimaryStatType Stat) const
{
	// 평상시 floor는 "순수 종족 기본값"이 아니라 "종족+직업요구량이 반영된 현재 기준선" 자체다.
	// (직업 요구량으로 강제로 올라간 부분도 유저가 임의로 못 깎게 막아야 함 - 보너스가 마이너스인 특수 상황 제외)
	return GetBaseValue(Stat);
}

void UDSCharacterStatsWidget::InitializeSkillPage()
{
	BaseSkillStat = SelectedProfession ? SelectedProfession->StartingSkillStat : FSkillStat();
	UserAddedSkillStat = FSkillStat();
	SkillBonusRemaining = SkillBonusPointsTotal;

	RefreshSkillRows();
}

// BaseSkillStat 기준으로 행 구성 + 표시만 갱신 (BaseSkillStat/SkillBonusRemaining 자체는 안 건드림)
// - InitializeSkillPage(정상 생성 흐름)와 EnterReviewMode(저장된 스킬 값 그대로 표시) 둘 다 재사용
void UDSCharacterStatsWidget::RefreshSkillRows()
{
	auto BuildRowNames = [this](const TArray<FName>& AllNames)
	{
		TArray<FName> Result;
		for (const FName& Name : AllNames)
		{
			if (GetSkillBaseValue(Name) != -1)
			{
				Result.Add(Name);
			}
		}
		return Result;
	};

	if (IsValid(DSG_Skills))
	{
		if (IsValid(DSG_Skills->DSG_WeaponarySkillAttributes))
		{
			DSG_Skills->DSG_WeaponarySkillAttributes->SetupRows(BuildRowNames(WeaponSkillNames));
		}
		if (IsValid(DSG_Skills->DSG_PhysicalSkillAttributes))
		{
			DSG_Skills->DSG_PhysicalSkillAttributes->SetupRows(BuildRowNames(PhysicalSkillNames));
		}
		if (IsValid(DSG_Skills->DSG_AcademiaSkillAttributes))
		{
			DSG_Skills->DSG_AcademiaSkillAttributes->SetupRows(BuildRowNames(AcademiaSkillNames));
		}
	}

	for (const TArray<FName>& Group : { WeaponSkillNames, PhysicalSkillNames, AcademiaSkillNames })
	{
		for (const FName& Name : Group)
		{
			if (GetSkillBaseValue(Name) != -1)
			{
				RefreshSkillDisplay(Name);
			}
		}
	}

	RefreshBonusPointsDisplay();
}

UDSAttributesWidget* UDSCharacterStatsWidget::FindSkillGroup(FName RowName) const
{
	if (!IsValid(DSG_Skills)) return nullptr;

	if (IsValid(DSG_Skills->DSG_WeaponarySkillAttributes) && DSG_Skills->DSG_WeaponarySkillAttributes->FindRow(RowName))
	{
		return DSG_Skills->DSG_WeaponarySkillAttributes;
	}
	if (IsValid(DSG_Skills->DSG_PhysicalSkillAttributes) && DSG_Skills->DSG_PhysicalSkillAttributes->FindRow(RowName))
	{
		return DSG_Skills->DSG_PhysicalSkillAttributes;
	}
	if (IsValid(DSG_Skills->DSG_AcademiaSkillAttributes) && DSG_Skills->DSG_AcademiaSkillAttributes->FindRow(RowName))
	{
		return DSG_Skills->DSG_AcademiaSkillAttributes;
	}
	return nullptr;
}

void UDSCharacterStatsWidget::RefreshSkillDisplay(FName RowName)
{
	if (UDSAttributesWidget* Group = FindSkillGroup(RowName))
	{
		Group->SetRowDisplay(RowName, GetSkillBaseValue(RowName) + GetSkillUserAddedValue(RowName), GetSkillUserAddedValue(RowName));
	}

	// WorkingCharacter에도 최종 스킬 반영 - 저장할 때 쓰이는 단일 진실 공급원
	FSkillStat FinalSkill = BaseSkillStat;
	FinalSkill.Sword += UserAddedSkillStat.Sword;
	FinalSkill.HandsAndFeet += UserAddedSkillStat.HandsAndFeet;
	FinalSkill.Axe += UserAddedSkillStat.Axe;
	FinalSkill.Bow += UserAddedSkillStat.Bow;
	FinalSkill.MaceAndFlail += UserAddedSkillStat.MaceAndFlail;
	FinalSkill.PoleAndStaff += UserAddedSkillStat.PoleAndStaff;
	FinalSkill.Shield += UserAddedSkillStat.Shield;
	FinalSkill.Sling += UserAddedSkillStat.Sling;
	FinalSkill.Throw += UserAddedSkillStat.Throw;
	FinalSkill.WandAndDagger += UserAddedSkillStat.WandAndDagger;
	FinalSkill.Stealth += UserAddedSkillStat.Stealth;
	FinalSkill.Oratory += UserAddedSkillStat.Oratory;
	FinalSkill.Skulduggery += UserAddedSkillStat.Skulduggery;
	FinalSkill.Artifacts += UserAddedSkillStat.Artifacts;
	FinalSkill.Kirijutsu += UserAddedSkillStat.Kirijutsu;
	FinalSkill.Thaumaturgy += UserAddedSkillStat.Thaumaturgy;
	FinalSkill.Theology += UserAddedSkillStat.Theology;
	WorkingCharacter.SkillStat = FinalSkill;

	RefreshDerivedStats(WorkingCharacter.characterData.CharStat);
}

void UDSCharacterStatsWidget::RefreshDerivedStats(const FCharacterStat& FinalStat)
{
	const int32 HpMul = SelectedProfession ? SelectedProfession->Profession_HpMul : 3;
	const int32 Hp = FMath::RoundToInt((4.f + 0.8f * HpMul) * 2.f * (3.f / 8.f + FinalStat.Vitality / 36.f));
	if (HitPoints)
	{
		HitPoints->SetText(FText::AsNumber(Hp));
	}

	const FSkillStat& Skill = WorkingCharacter.SkillStat;
	const bool bCanThaumaturgy = Skill.Thaumaturgy != -1;
	const bool bCanTheology = Skill.Theology != -1;

	FSpellPoints RealmSpellPoints;
	if (bCanThaumaturgy || bCanTheology)
	{
		// 3번째/4번째 스펠북(Artifacts/Kirijutsu)은 마법이랑 무관해서 계산에서 제외. -1(사용 불가)인 쪽은 0 취급
		const float A = bCanThaumaturgy ? (float)Skill.Thaumaturgy : 0.f;
		const float B = bCanTheology ? (float)Skill.Theology : 0.f;
		const float Highest = FMath::Max(A, B);
		const float Second = FMath::Min(A, B);
		const float WeightedSkill = FMath::Min(125.f, Highest + Second * 0.5f);
		const float BaseTerm = 1.f + FinalStat.Piety / 9.f + FinalStat.Intelligence / 12.f + WeightedSkill * 0.05f;

		const int32 Level = WorkingCharacter.characterData.FinalFaceStat.Level;
		const FKnownSpells& Known = WorkingCharacter.KnownSpells;

		// 위저드리8 원 공식은 아는 스펠이 0개여도 (Lvl+1) 기본 풀이 남는데, 우리 게임은 스탯 스케일이 달라서
		// 그 기본 풀만으로도 값이 과하게 크게 나옴 - 그래서 아는 스펠이 아예 없는 영역은 그냥 0점으로 고정
		auto ComputeRealmSP = [Level, BaseTerm](int32 KnownCount) -> int32
		{
			return KnownCount > 0 ? FMath::RoundToInt((Level + 1 + KnownCount) * BaseTerm) : 0;
		};

		RealmSpellPoints.Aether = ComputeRealmSP(Known.KnownSpells_Aether.Num());
		RealmSpellPoints.Water  = ComputeRealmSP(Known.KnownSpells_Water.Num());
		RealmSpellPoints.Air    = ComputeRealmSP(Known.KnownSpells_Air.Num());
		RealmSpellPoints.Earth  = ComputeRealmSP(Known.KnownSpells_Earth.Num());
		RealmSpellPoints.Sun    = ComputeRealmSP(Known.KnownSpells_Sun.Num());
	}

	WorkingCharacter.characterData.BaseFaceStat.MaxSP = RealmSpellPoints;
	WorkingCharacter.characterData.FinalFaceStat.MaxSP = RealmSpellPoints;
	WorkingCharacter.characterData.BaseFaceStat.CurrentSP = RealmSpellPoints;
	WorkingCharacter.characterData.FinalFaceStat.CurrentSP = RealmSpellPoints;

	if (SpellPoints)
	{
		SpellPoints->SetText(FText::AsNumber(RealmSpellPoints.Total()));
	}

	// 방어구 시스템이 아직 없어서 계산 없이 현재 값만 표시
	if (ArmorPoints)
	{
		ArmorPoints->SetText(FText::AsNumber(WorkingCharacter.characterData.FinalFaceStat.Armor));
	}

	const int32 CarryCapacity = FMath::FloorToInt(FinalStat.Strength * 2.4f + FinalStat.Vitality * 1.2f);
	if (CarryCapacityPoints)
	{
		CarryCapacityPoints->SetText(FText::AsNumber(CarryCapacity));
	}
}

void UDSCharacterStatsWidget::OnSkillRowClicked(FName RowName, int32 Direction)
{
	if (bReviewMode) return;

	const int32 BaseValue = GetSkillBaseValue(RowName);
	if (BaseValue == -1) return; // 이 직업이 못 쓰는 스킬 (안전장치, 애초에 행이 안 생김)

	if (Direction > 0)
	{
		if (SkillBonusRemaining <= 0) return;

		SetSkillUserAddedValue(RowName, GetSkillUserAddedValue(RowName) + 1);
		SkillBonusRemaining -= 1;
	}
	else
	{
		// 스킬은 직업이 준 시작값 밑으로는 못 깎음 (스탯과 달리 마이너스 보너스 상황이 없음)
		if (GetSkillUserAddedValue(RowName) <= 0) return;

		SetSkillUserAddedValue(RowName, GetSkillUserAddedValue(RowName) - 1);
		SkillBonusRemaining += 1;
	}

	RefreshSkillDisplay(RowName);
	RefreshBonusPointsDisplay();
}

void UDSCharacterStatsWidget::OnPortraitChanged(UTexture2D* NewPortrait)
{
	WorkingCharacter.Portrait = NewPortrait;

	if (IsValid(Portrait))
	{
		if (NewPortrait)
		{
			Portrait->SetBrushFromTexture(NewPortrait);
		}
		else
		{
			// 선택 없음(리셋 직후 등) - 디자이너 기본 브러시(해골 등)로 복원
			Portrait->SetBrush(DefaultPortraitBrush);
		}
	}
}

int32 UDSCharacterStatsWidget::GetSkillBaseValue(FName RowName) const
{
	if (RowName == TEXT("Sword")) return BaseSkillStat.Sword;
	if (RowName == TEXT("HandsAndFeet")) return BaseSkillStat.HandsAndFeet;
	if (RowName == TEXT("Axe")) return BaseSkillStat.Axe;
	if (RowName == TEXT("Bow")) return BaseSkillStat.Bow;
	if (RowName == TEXT("MaceAndFlail")) return BaseSkillStat.MaceAndFlail;
	if (RowName == TEXT("PoleAndStaff")) return BaseSkillStat.PoleAndStaff;
	if (RowName == TEXT("Shield")) return BaseSkillStat.Shield;
	if (RowName == TEXT("Sling")) return BaseSkillStat.Sling;
	if (RowName == TEXT("Throw")) return BaseSkillStat.Throw;
	if (RowName == TEXT("WandAndDagger")) return BaseSkillStat.WandAndDagger;
	if (RowName == TEXT("Stealth")) return BaseSkillStat.Stealth;
	if (RowName == TEXT("Oratory")) return BaseSkillStat.Oratory;
	if (RowName == TEXT("Skulduggery")) return BaseSkillStat.Skulduggery;
	if (RowName == TEXT("Artifacts")) return BaseSkillStat.Artifacts;
	if (RowName == TEXT("Kirijutsu")) return BaseSkillStat.Kirijutsu;
	if (RowName == TEXT("Thaumaturgy")) return BaseSkillStat.Thaumaturgy;
	if (RowName == TEXT("Theology")) return BaseSkillStat.Theology;
	return -1;
}

int32 UDSCharacterStatsWidget::GetSkillUserAddedValue(FName RowName) const
{
	if (RowName == TEXT("Sword")) return UserAddedSkillStat.Sword;
	if (RowName == TEXT("HandsAndFeet")) return UserAddedSkillStat.HandsAndFeet;
	if (RowName == TEXT("Axe")) return UserAddedSkillStat.Axe;
	if (RowName == TEXT("Bow")) return UserAddedSkillStat.Bow;
	if (RowName == TEXT("MaceAndFlail")) return UserAddedSkillStat.MaceAndFlail;
	if (RowName == TEXT("PoleAndStaff")) return UserAddedSkillStat.PoleAndStaff;
	if (RowName == TEXT("Shield")) return UserAddedSkillStat.Shield;
	if (RowName == TEXT("Sling")) return UserAddedSkillStat.Sling;
	if (RowName == TEXT("Throw")) return UserAddedSkillStat.Throw;
	if (RowName == TEXT("WandAndDagger")) return UserAddedSkillStat.WandAndDagger;
	if (RowName == TEXT("Stealth")) return UserAddedSkillStat.Stealth;
	if (RowName == TEXT("Oratory")) return UserAddedSkillStat.Oratory;
	if (RowName == TEXT("Skulduggery")) return UserAddedSkillStat.Skulduggery;
	if (RowName == TEXT("Artifacts")) return UserAddedSkillStat.Artifacts;
	if (RowName == TEXT("Kirijutsu")) return UserAddedSkillStat.Kirijutsu;
	if (RowName == TEXT("Thaumaturgy")) return UserAddedSkillStat.Thaumaturgy;
	if (RowName == TEXT("Theology")) return UserAddedSkillStat.Theology;
	return 0;
}

void UDSCharacterStatsWidget::SetSkillUserAddedValue(FName RowName, int32 Value)
{
	if (RowName == TEXT("Sword")) UserAddedSkillStat.Sword = Value;
	else if (RowName == TEXT("HandsAndFeet")) UserAddedSkillStat.HandsAndFeet = Value;
	else if (RowName == TEXT("Axe")) UserAddedSkillStat.Axe = Value;
	else if (RowName == TEXT("Bow")) UserAddedSkillStat.Bow = Value;
	else if (RowName == TEXT("MaceAndFlail")) UserAddedSkillStat.MaceAndFlail = Value;
	else if (RowName == TEXT("PoleAndStaff")) UserAddedSkillStat.PoleAndStaff = Value;
	else if (RowName == TEXT("Shield")) UserAddedSkillStat.Shield = Value;
	else if (RowName == TEXT("Sling")) UserAddedSkillStat.Sling = Value;
	else if (RowName == TEXT("Throw")) UserAddedSkillStat.Throw = Value;
	else if (RowName == TEXT("WandAndDagger")) UserAddedSkillStat.WandAndDagger = Value;
	else if (RowName == TEXT("Stealth")) UserAddedSkillStat.Stealth = Value;
	else if (RowName == TEXT("Oratory")) UserAddedSkillStat.Oratory = Value;
	else if (RowName == TEXT("Skulduggery")) UserAddedSkillStat.Skulduggery = Value;
	else if (RowName == TEXT("Artifacts")) UserAddedSkillStat.Artifacts = Value;
	else if (RowName == TEXT("Kirijutsu")) UserAddedSkillStat.Kirijutsu = Value;
	else if (RowName == TEXT("Thaumaturgy")) UserAddedSkillStat.Thaumaturgy = Value;
	else if (RowName == TEXT("Theology")) UserAddedSkillStat.Theology = Value;
}

bool UDSCharacterStatsWidget::ValidateCharacterName()
{
	const FString Name = IsValid(EditableText_48) ? EditableText_48->GetText().ToString().TrimStartAndEnd() : FString();

	if (Name.IsEmpty())
	{
		ShowMessageBox(FText::FromString(TEXT("Please enter a name.")));
		return false;
	}

	UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr;
	if (UDSSaveGameSubsystem* SaveSubsystem = GI ? GI->GetSubsystem<UDSSaveGameSubsystem>() : nullptr)
	{
		if (SaveSubsystem->DoesCharacterNameExist(FName(*Name)))
		{
			ShowMessageBox(FText::FromString(TEXT("This name already exists.")));
			return false;
		}
	}

	WorkingCharacter.characterName = FName(*Name);
	return true;
}

void UDSCharacterStatsWidget::ShowMessageBox(const FText& Message)
{
	if (!MessageBoxClass) return;

	UDSMessageBoxWidget::Show(this, MessageBoxClass, Message);
}

void UDSCharacterStatsWidget::OnFinalizeYesNoResult(bool bYes)
{
	if (!bYes) return;

	// 스펠 추가 페이지에서 Cyan으로 선택만 해뒀던 스펠들을 여기서 최종 확정
	CommitSelectedSpells();

	UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr;
	UDSSaveGameSubsystem* SaveSubsystem = GI ? GI->GetSubsystem<UDSSaveGameSubsystem>() : nullptr;
	if (!SaveSubsystem) return;

	FText OutError;
	if (SaveSubsystem->SaveCharacter(WorkingCharacter, OutError))
	{
		OnCharacterCreated.Broadcast();
		ResetCreationState();
	}
	else
	{
		ShowMessageBox(OutError);
	}
}

void UDSCharacterStatsWidget::ResetCreationState()
{
	// 리뷰 모드에서 걸어뒀던 읽기전용 상태 원복 - PopulateComboBoxes가 아래서 On*Changed를 연쇄 호출하니 그 전에 꺼야 함
	bReviewMode = false;

	if (IsValid(WidgetSwitcher_0))
	{
		WidgetSwitcher_0->SetActiveWidgetIndex(0);
	}
	if (IsValid(EditableText_48))
	{
		EditableText_48->SetText(FText::GetEmpty());
		EditableText_48->SetIsReadOnly(false);
	}
	if (IsValid(DSG_PortraitChooser))
	{
		DSG_PortraitChooser->ResetSelection();
		DSG_PortraitChooser->SetInteractable(true);
	}
	if (IsValid(ComboBox_Pro))    ComboBox_Pro->SetInteractable(true);
	if (IsValid(ComboBox_Race))   ComboBox_Race->SetInteractable(true);
	if (IsValid(ComboBox_Gender)) ComboBox_Gender->SetInteractable(true);
	if (IsValid(KarmaSlider))
	{
		KarmaSlider->SetValue(FFaceStat().Karma / 18.f);
		KarmaSlider->SetIsEnabled(true);
	}
	if (IsValid(DSG_SpellAdd))
	{
		DSG_SpellAdd->ClearAllSelections();
	}

	SelectedProfession = nullptr;
	SelectedRace = nullptr;
	BaseComputedStat = FCharacterStat();
	UserAddedStat = FCharacterStat();
	BonusRemaining = 0;
	BaseSkillStat = FSkillStat();
	UserAddedSkillStat = FSkillStat();
	SkillBonusRemaining = 0;
	SpellBonusRemaining = 0;
	bSkippedSpellAdd = false;
	WorkingCharacter = FPlayerCharacterSaveData();

	// 콤보박스를 다시 채우면 첫 옵션이 자동 선택되면서 OnProfessionChanged/OnRaceChanged/OnGenderChanged가
	// 연쇄적으로 불려 RecalculateBaseStats/InitializeSkillPage/RefreshDisplay까지 전부 새로 계산됨
	PopulateComboBoxes();

	if (IsValid(DSG_PortraitChooser))
	{
		WorkingCharacter.Portrait = DSG_PortraitChooser->GetSelectedPortrait();
	}

	RefreshResistanceDisplay();
	RefreshDisplay();
	UpdateConfirmButtonLabel();
	UpdateDescriptionText();
	RefreshBonusPointsDisplay();
	UpdateKarmaFromSlider();
}

void UDSCharacterStatsWidget::EnterReviewMode(const FPlayerCharacterSaveData& CharacterData)
{
	bReviewMode = true;
	WorkingCharacter = CharacterData;

	// 직업/종족 데이터애셋 룩업 (RecalculateBaseStats를 안 거치니 직접 찾아줌)
	SelectedProfession = nullptr;
	SelectedRace = nullptr;
	if (UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
	{
		if (UCharacterClassRegistry* CCR = GI->GetSubsystem<UCharacterClassRegistry>())
		{
			SelectedProfession = CCR->GetCharacterClassByProfession(CharacterData.Profession);
		}
		if (UDSRaceDataRegistry* RDR = GI->GetSubsystem<UDSRaceDataRegistry>())
		{
			for (UDSRaceData* Race : RDR->GetAllRaces())
			{
				if (Race && Race->eRace == CharacterData.Race)
				{
					SelectedRace = Race;
					break;
				}
			}
		}
	}

	// 저장된 최종 스탯을 그대로 "기본값"으로 취급 - 유저 추가분 0, 남는 보너스 없음
	BaseComputedStat = CharacterData.characterData.CharStat;
	UserAddedStat = FCharacterStat();
	UserAddedStat.Strength = 0;
	UserAddedStat.Intelligence = 0;
	UserAddedStat.Piety = 0;
	UserAddedStat.Vitality = 0;
	UserAddedStat.Dexterity = 0;
	UserAddedStat.Speed = 0;
	UserAddedStat.Charisma = 0;
	BonusRemaining = 0;

	BaseSkillStat = CharacterData.SkillStat;
	UserAddedSkillStat = FSkillStat();
	SkillBonusRemaining = 0;
	RefreshSkillRows();

	SpellBonusRemaining = 0;
	bSkippedSpellAdd = false;

	// 콤보박스는 표시 텍스트만 맞춤 (bReviewMode라 On*Changed 핸들러들은 재계산 없이 그냥 리턴함)
	if (IsValid(ComboBox_Pro))
	{
		ComboBox_Pro->SetSelectedOption(GetProfessionDisplayName(CharacterData.Profession));
		ComboBox_Pro->SetInteractable(false);
	}
	if (IsValid(ComboBox_Race))
	{
		if (SelectedRace) ComboBox_Race->SetSelectedOption(SelectedRace->RaceName.ToString());
		ComboBox_Race->SetInteractable(false);
	}
	if (IsValid(ComboBox_Gender))
	{
		ComboBox_Gender->SetSelectedOption(CharacterData.characterData.FinalFaceStat.Gender ? GenderMale : GenderFemale);
		ComboBox_Gender->SetInteractable(false);
	}
	if (IsValid(EditableText_48))
	{
		EditableText_48->SetText(FText::FromName(CharacterData.characterName));
		EditableText_48->SetIsReadOnly(true);
	}
	if (IsValid(KarmaSlider))
	{
		KarmaSlider->SetValue(CharacterData.characterData.FinalFaceStat.Karma / 18.f);
		KarmaSlider->SetIsEnabled(false);
	}
	if (IsValid(DSG_PortraitChooser))
	{
		DSG_PortraitChooser->SelectPortrait(CharacterData.Portrait.LoadSynchronous());
		DSG_PortraitChooser->SetInteractable(false);
	}

	UTexture2D* newTexture = CharacterData.Portrait.LoadSynchronous();
	if (IsValid(Portrait) && newTexture)
	{
		Portrait->SetBrushFromTexture(newTexture);
	}

	RefreshGenderRaceText();
	RefreshResistanceDisplay();
	RefreshDisplay();
	UpdateKarmaFromSlider();

	GoToPage(PageStats);
}
