
#include "SpellCastWidget.h"
#include "UI/SpellEntryWidget.h"
#include "DSSpellData.h"
#include "SpellCast.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"

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

}

void USpellCastWidget::InitializeWidget(USpellCast* InSpellCast, const TArray<UDSSpellData*>& InKnownSpells)
{
	SpellCastAction = InSpellCast;
	KnownSpells.Reset();
	for (UDSSpellData* Spell : InKnownSpells)
	{
		if (Spell) KnownSpells.Add(Spell);
	}
	SelectedSpell = nullptr;
	SelectedLevel = 1;

	RefreshDiceDisplay();
	RefreshSpellList();
	RefreshSpellInfo();
}

void USpellCastWidget::OnElementSelected(ESpellElement Element)
{
	SelectedElement = Element;
	SelectedSpell = nullptr;
	RefreshSpellList();
	RefreshSpellInfo();
}

void USpellCastWidget::OnSpellSelected(UDSSpellData* SpellData)
{
	SelectedSpell = SpellData;
	RefreshSpellInfo();
}

void USpellCastWidget::OnLevelChanged(int32 NewLevel)
{
	SelectedLevel = FMath::Clamp(NewLevel, 1, 6);
	RefreshDiceDisplay();
}

void USpellCastWidget::OnCancelClicked()
{
}

TArray<UDSSpellData*> USpellCastWidget::GetFilteredSpells() const
{
	TArray<UDSSpellData*> Result;
	for (UDSSpellData* Spell : KnownSpells)
	{
		if (Spell && Spell->Element == SelectedElement)
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
	}
}

void USpellCastWidget::RefreshDiceDisplay()
{
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
void USpellCastWidget::HandleDice1Clicked() { OnLevelChanged(1); }
void USpellCastWidget::HandleDice2Clicked() { OnLevelChanged(2); }
void USpellCastWidget::HandleDice3Clicked() { OnLevelChanged(3); }
void USpellCastWidget::HandleDice4Clicked() { OnLevelChanged(4); }
void USpellCastWidget::HandleDice5Clicked() { OnLevelChanged(5); }
void USpellCastWidget::HandleDice6Clicked() { OnLevelChanged(6); }

// ===== Cancel =====
void USpellCastWidget::HandleCancelClicked() { OnCancelClicked(); }

// ===== Spell entry =====
void USpellCastWidget::HandleSpellEntryClicked(UDSSpellData* InSpellData) { OnSpellSelected(InSpellData); }
