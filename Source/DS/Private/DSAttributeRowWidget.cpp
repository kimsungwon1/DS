
#include "DSAttributeRowWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UDSAttributeRowWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
	{
		return;
	}

	if (IsValid(Strengthminus))
	{
		Strengthminus->OnClicked.RemoveDynamic(this, &UDSAttributeRowWidget::HandleMinusClicked);
		Strengthminus->OnClicked.AddDynamic(this, &UDSAttributeRowWidget::HandleMinusClicked);
	}
	if (IsValid(Strengthadd))
	{
		Strengthadd->OnClicked.RemoveDynamic(this, &UDSAttributeRowWidget::HandlePlusClicked);
		Strengthadd->OnClicked.AddDynamic(this, &UDSAttributeRowWidget::HandlePlusClicked);
	}
}

void UDSAttributeRowWidget::NativeDestruct()
{
	if (IsValid(Strengthminus))
	{
		Strengthminus->OnClicked.RemoveDynamic(this, &UDSAttributeRowWidget::HandleMinusClicked);
	}
	if (IsValid(Strengthadd))
	{
		Strengthadd->OnClicked.RemoveDynamic(this, &UDSAttributeRowWidget::HandlePlusClicked);
	}

	Super::NativeDestruct();
}

void UDSAttributeRowWidget::SetLabel(const FText& Label)
{
	if (TextBox) TextBox->SetText(Label);
}

void UDSAttributeRowWidget::SetResult(int32 Value)
{
	if (StrengthResult) StrengthResult->SetText(FText::AsNumber(Value));
}

void UDSAttributeRowWidget::SetRemain(int32 Value)
{
	if (Strengthremain) Strengthremain->SetText(FText::AsNumber(Value));
}

void UDSAttributeRowWidget::HandleMinusClicked()
{
	OnAttributeRowClicked.Broadcast(this, -1);
}

void UDSAttributeRowWidget::HandlePlusClicked()
{
	OnAttributeRowClicked.Broadcast(this, 1);
}
