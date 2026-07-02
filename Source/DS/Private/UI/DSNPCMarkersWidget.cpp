#include "UI/DSNPCMarkersWidget.h"
#include "Components/Image.h"

void UDSNPCMarkersWidget::SetSlot(int32 Index, bool bShow, FLinearColor Color)
{
	UImage* iSlot = GetSlot(Index);
	if (!iSlot) return;

	iSlot->SetVisibility(bShow ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	iSlot->SetColorAndOpacity(Color);
}

UImage* UDSNPCMarkersWidget::GetSlot(int32 Index) const
{
	static const FName SlotNames[] = {
		TEXT("Slot0"), TEXT("Slot1"), TEXT("Slot2"),
		TEXT("Slot3"), TEXT("Slot4"), TEXT("Slot5")
	};
	if (Index < 0 || Index > 5) return nullptr;
	return Cast<UImage>(GetWidgetFromName(SlotNames[Index]));
}
