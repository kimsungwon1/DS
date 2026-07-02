
#include "UI/SpellEntryWidget.h"
#include "DSSpellData.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void USpellEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (EntryButton)
		EntryButton->OnClicked.AddDynamic(this, &USpellEntryWidget::HandleClicked);
}

void USpellEntryWidget::SetSpellData(UDSSpellData* InSpellData)
{
	SpellData = InSpellData;
	if (!InSpellData) return;

	if (SpellNameText)
		SpellNameText->SetText(FText::FromName(InSpellData->SpellName));

	if (MPCostText)
		MPCostText->SetText(FText::Format(FText::FromString(TEXT("MP {0}")), InSpellData->MPCost));
}

void USpellEntryWidget::HandleClicked()
{
	if (SpellData)
		OnSpellEntryClicked.Broadcast(SpellData);
}
