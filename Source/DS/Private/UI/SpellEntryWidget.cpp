
#include "UI/SpellEntryWidget.h"
#include "DSSpellData.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

namespace
{
	const FLinearColor EntryColor_Normal   = FLinearColor::White;
	const FLinearColor EntryColor_Hovered  = FLinearColor::Yellow;
	const FLinearColor EntryColor_Selected = FLinearColor(0.f, 1.f, 1.f);
}

void USpellEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (EntryButton)
	{
		EntryButton->OnClicked.AddDynamic(this, &USpellEntryWidget::HandleClicked);
		EntryButton->OnHovered.AddDynamic(this, &USpellEntryWidget::HandleHovered);
		EntryButton->OnUnhovered.AddDynamic(this, &USpellEntryWidget::HandleUnhovered);
	}

	ApplyTextColor(EntryColor_Normal);
}

void USpellEntryWidget::SetSpellData(UDSSpellData* InSpellData)
{
	SpellData = InSpellData;
	if (!InSpellData) return;

	if (SpellNameText)
		SpellNameText->SetText(FText::FromName(InSpellData->SpellName));

	if (MPCostText)
		MPCostText->SetText(FText::AsNumber(InSpellData->spellStat.MpCost));
}

void USpellEntryWidget::SetSelected(bool bInSelected)
{
	bSelected = bInSelected;
	if (bSelected)
		ApplyTextColor(EntryColor_Selected);
	else
		ApplyTextColor(EntryButton && EntryButton->IsHovered() ? EntryColor_Hovered : EntryColor_Normal);
}

void USpellEntryWidget::HandleClicked()
{
	// 여기서 SetSelected를 미리 확정하면 안 됨 - 실제 선택 상태는 이 클릭 브로드캐스트를 받는 쪽
	// (DSSpellAddWidget 다중 선택 토글, SpellCastWidget 단일 선택 등)이 각자 정책대로 결정해서 SetSelected를 부름
	if (SpellData)
		OnSpellEntryClicked.Broadcast(SpellData);
}

void USpellEntryWidget::HandleHovered()
{
	if (!bSelected)
		ApplyTextColor(EntryColor_Hovered);
}

void USpellEntryWidget::HandleUnhovered()
{
	if (!bSelected)
		ApplyTextColor(EntryColor_Normal);
}

void USpellEntryWidget::ApplyTextColor(const FLinearColor& Color)
{
	const FSlateColor SlateColor(Color);
	if (SpellNameText)
		SpellNameText->SetColorAndOpacity(SlateColor);
	if (MPCostText)
		MPCostText->SetColorAndOpacity(SlateColor);
}
