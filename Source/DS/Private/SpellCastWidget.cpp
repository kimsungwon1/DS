
#include "SpellCastWidget.h"
#include "UI/SpellEntryWidget.h"
#include "DSSpellData.h"
#include "SpellCast.h"
#include "CharacterInstanceComponent.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "DSPlayerController.h"

void USpellCastWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Aether)  Btn_Aether->OnClicked.AddDynamic(this, &USpellCastWidget::HandleAetherClicked);
	if (Btn_Sun)     Btn_Sun->OnClicked.AddDynamic(this, &USpellCastWidget::HandleSunClicked);
	if (Btn_Earth)   Btn_Earth->OnClicked.AddDynamic(this, &USpellCastWidget::HandleEarthClicked);
	if (Btn_Water)   Btn_Water->OnClicked.AddDynamic(this, &USpellCastWidget::HandleWaterClicked);
	if (Btn_Air)     Btn_Air->OnClicked.AddDynamic(this, &USpellCastWidget::HandleAirClicked);

	if (DiceBtn_1)   DiceBtn_1->OnClicked.AddDynamic(this, &USpellCastWidget::HandleDice1Clicked);
	if (DiceBtn_2)   DiceBtn_2->OnClicked.AddDynamic(this, &USpellCastWidget::HandleDice2Clicked);
	if (DiceBtn_3)   DiceBtn_3->OnClicked.AddDynamic(this, &USpellCastWidget::HandleDice3Clicked);
	if (DiceBtn_4)   DiceBtn_4->OnClicked.AddDynamic(this, &USpellCastWidget::HandleDice4Clicked);
	if (DiceBtn_5)   DiceBtn_5->OnClicked.AddDynamic(this, &USpellCastWidget::HandleDice5Clicked);
	if (DiceBtn_6)   DiceBtn_6->OnClicked.AddDynamic(this, &USpellCastWidget::HandleDice6Clicked);
	if (DiceBtn_7)   DiceBtn_7->OnClicked.AddDynamic(this, &USpellCastWidget::HandleDice7Clicked);

	// 시작 시 전부 검정 (스펠 미선택 상태)
	RefreshDiceDisplay();
}

void USpellCastWidget::InitializeWidget(USpellCast* InSpellCast)
{
	SpellCastAction = InSpellCast;
	KnownSpells.Reset();

	if (InSpellCast && InSpellCast->GetActor())
	{
		const FKnownSpells& Known = InSpellCast->GetActor()->GetKnownSpells();
		auto AppendRealm = [this](const TArray<TSoftObjectPtr<UDSSpellData>>& RealmSpells)
		{
			for (const TSoftObjectPtr<UDSSpellData>& Soft : RealmSpells)
			{
				if (UDSSpellData* Spell = Soft.LoadSynchronous())
				{
					KnownSpells.Add(Spell);
				}
			}
		};
		AppendRealm(Known.KnownSpells_Aether);
		AppendRealm(Known.KnownSpells_Water);
		AppendRealm(Known.KnownSpells_Air);
		AppendRealm(Known.KnownSpells_Earth);
		AppendRealm(Known.KnownSpells_Sun);
	}

	if (InSpellCast->GetSpell_const())
	{
		SelectedSpell = InSpellCast->GetSpell_const()->SpellData;
	}
	else 
	{
		SelectedSpell = nullptr;
	}
	SelectedLevel = 0;

	if (SelectedSpell == nullptr)
	{
		SpellListBox->ClearChildren();
	}
	else
	{
		SelectedElement = SelectedSpell->spellStat.element;   // OnElementSelected 대신 직접 세팅 — 초기화 안 됨
		RefreshSpellList();
		SelectedLevel = SpellCastAction->GetSpellLevel();
		UpdateEntrySelection();
		RefreshDiceDisplay();
		RefreshSpellInfo();
	}
}

void USpellCastWidget::OnElementSelected(ESpellElement Element)
{
	SelectedElement = Element;
	SelectedSpell = nullptr;
	SelectedLevel = 0;
	RefreshSpellList();
	RefreshDiceDisplay();
	RefreshSpellInfo();

	GetDsController()->DisableTargetSelection(false);
}

void USpellCastWidget::OnSpellSelected(UDSSpellData* SpellData)
{
	SelectedSpell = SpellData;
	SelectedLevel = 0;
	UpdateEntrySelection();
	RefreshDiceDisplay();
	RefreshSpellInfo();

	GetDsController()->DisableTargetSelection(false);
}

// 선택된 스펠만 Cyan, 나머지는 해제
void USpellCastWidget::UpdateEntrySelection()
{
	if (!SpellListBox) return;
	for (int32 i = 0; i < SpellListBox->GetChildrenCount(); ++i)
	{
		if (USpellEntryWidget* Entry = Cast<USpellEntryWidget>(SpellListBox->GetChildAt(i)))
			Entry->SetSelected(SelectedSpell && Entry->GetSpellData() == SelectedSpell);
	}
}

void USpellCastWidget::DiceClicked(int32 NewLevel)
{
	SelectedLevel = FMath::Clamp(NewLevel, 1, 7);
	RefreshDiceDisplay();

	if (SelectedSpell)
	{
		if (SpellCastAction)
		{
			SpellCastAction->SetSpellFromData(SelectedSpell, SelectedLevel);
		}

		auto* cont = Cast<ADSPlayerController>(GetOwningPlayer());

		cont->EnableSelect(SpellCastAction, SelectedSpell->SelectorClass);
	}
}

void USpellCastWidget::OnCancelClicked()
{
	SelectedSpell = nullptr;
	UpdateEntrySelection();
	RefreshDiceDisplay();
	RefreshSpellInfo();

	GetDsController()->DisableTargetSelection(false);
}

TArray<UDSSpellData*> USpellCastWidget::GetFilteredSpells() const
{
	TArray<UDSSpellData*> Result;
	for (UDSSpellData* Spell : KnownSpells)
	{
		if (Spell && Spell->spellStat.element == SelectedElement)
			Result.Add(Spell);
	}
	return Result;
}

void USpellCastWidget::RefreshSpellList()
{
	if (!SpellListBox || !SpellEntryClass) return;

	SpellListBox->ClearChildren();

	for (UDSSpellData* Spell : GetFilteredSpells())
	{
		USpellEntryWidget* Entry = CreateWidget<USpellEntryWidget>(this, SpellEntryClass);
		if (!Entry) continue;
		Entry->SetSpellData(Spell);
		Entry->OnSpellEntryClicked.AddDynamic(this, &USpellCastWidget::HandleSpellEntryClicked);
		SpellListBox->AddChild(Entry);
		if (Spell == SelectedSpell) 
		{
			Entry->SetSelected(true);
		}
	}
}

void USpellCastWidget::RefreshDiceDisplay()
{
	UButton* Buttons[7] = { DiceBtn_1, DiceBtn_2, DiceBtn_3, DiceBtn_4, DiceBtn_5, DiceBtn_6, DiceBtn_7 };
	for (int32 i = 0; i < 7; ++i)
	{
		const int32 Level = i + 1;
		if (Buttons[i])
			SetDiceButtonGraph(Buttons[i], Level, GetDiceGraphColor(Level));
	}

	OnDiceStateChanged(SelectedLevel, SelectedSpell != nullptr);
}

bool USpellCastWidget::CanAffordLevel(int32 Level) const
{
	// TODO: MP 시스템 구현 후 실제 잔여 MP와 (MPCost × 레벨 배율) 비교로 교체
	return Level < 7;
}

float USpellCastWidget::GetSuccessChance(int32 Level) const
{
	// TODO: 캐릭터 실력 기반 확률식은 기획 확정 후 교체 (placeholder: 레벨당 선형 감소)
	return 1.f - (Level - 1) / 6.f;
}

USpellCastWidget::EDiceGraphColor USpellCastWidget::GetDiceGraphColor(int32 Level) const
{
	// 스펠 미선택 상태 — 판정할 대상이 없으니 전부 검정
	if (!SelectedSpell)
		return EDiceGraphColor::Black;

	if (!CanAffordLevel(Level))
		return EDiceGraphColor::Black;

	const float Chance = GetSuccessChance(Level);
	if (Chance >= 0.75f) return EDiceGraphColor::Green;
	if (Chance >= 0.50f) return EDiceGraphColor::Yellow;
	if (Chance >= 0.25f) return EDiceGraphColor::Orange;
	return EDiceGraphColor::Red;
}

void USpellCastWidget::SetDiceButtonGraph(UButton* Button, int32 Level, EDiceGraphColor Color)
{
	static const TCHAR* ColorNames[]  = { TEXT("black"), TEXT("red"), TEXT("orange"), TEXT("yellow"), TEXT("green") };
	static const TCHAR* FolderNames[] = { TEXT("Black"), TEXT("Red"), TEXT("Orange"), TEXT("Yellow"), TEXT("Green") };
	const int32 Idx = static_cast<int32>(Color);

	const FString Path = FString::Printf(
		TEXT("/Game/Made/UI/Images/Dice/Graphs_%s/graph_%d_%s.graph_%d_%s"),
		FolderNames[Idx], Level, ColorNames[Idx], Level, ColorNames[Idx]);

	UTexture2D* Tex = LoadObject<UTexture2D>(nullptr, *Path);
	if (!Tex) return;

	FButtonStyle Style = Button->GetStyle();
	Style.Normal.SetResourceObject(Tex);
	Style.Hovered.SetResourceObject(Tex);
	Style.Pressed.SetResourceObject(Tex);
	Button->SetStyle(Style);
}

void USpellCastWidget::RefreshSpellInfo()
{
}

// ===== Element button handlers =====
void USpellCastWidget::HandleAetherClicked() { OnElementSelected(ESpellElement::Aether); }
void USpellCastWidget::HandleSunClicked()    { OnElementSelected(ESpellElement::Sun); }
void USpellCastWidget::HandleEarthClicked()  { OnElementSelected(ESpellElement::Earth); }
void USpellCastWidget::HandleWaterClicked()  { OnElementSelected(ESpellElement::Water); }
void USpellCastWidget::HandleAirClicked()    { OnElementSelected(ESpellElement::Air); }

// ===== Dice button handlers =====
void USpellCastWidget::HandleDice1Clicked() { DiceClicked(1); }
void USpellCastWidget::HandleDice2Clicked() { DiceClicked(2); }
void USpellCastWidget::HandleDice3Clicked() { DiceClicked(3); }
void USpellCastWidget::HandleDice4Clicked() { DiceClicked(4); }
void USpellCastWidget::HandleDice5Clicked() { DiceClicked(5); }
void USpellCastWidget::HandleDice6Clicked() { DiceClicked(6); }
void USpellCastWidget::HandleDice7Clicked() { DiceClicked(7); }

// ===== Cancel =====
void USpellCastWidget::HandleCancelClicked() { OnCancelClicked(); }

// ===== Spell entry =====
void USpellCastWidget::HandleSpellEntryClicked(UDSSpellData* InSpellData) { OnSpellSelected(InSpellData); }
