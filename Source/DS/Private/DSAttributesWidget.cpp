
#include "DSAttributesWidget.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "UObject/ConstructorHelpers.h"

UDSAttributesWidget::UDSAttributesWidget()
{
	static ConstructorHelpers::FClassFinder<UDSAttributeRowWidget> RowClassFinder(TEXT("/Game/Made/UI/CharacterUi/DSG_AttributeRow"));
	if (RowClassFinder.Succeeded())
	{
		RowWidgetClass = RowClassFinder.Class;
	}
}

void UDSAttributesWidget::SetupRows(const TArray<FName>& RowNames)
{
	if (!verticalBox || !RowWidgetClass) return;

	verticalBox->ClearChildren();
	Rows.Empty();

	if (RowNames.Num() == 0)
	{
		if (UTextBlock* EmptyLabel = NewObject<UTextBlock>(this))
		{
			EmptyLabel->SetText(FText::FromString(TEXT("-")));
			verticalBox->AddChild(EmptyLabel);
		}
		return;
	}

	for (const FName& RowName : RowNames)
	{
		UDSAttributeRowWidget* Row = CreateWidget<UDSAttributeRowWidget>(this, RowWidgetClass);
		if (!Row) continue;

		Row->RowName = RowName;
		Row->SetLabel(FText::FromName(RowName));
		Row->SetResult(0);
		Row->SetRemain(0);
		Row->OnAttributeRowClicked.AddDynamic(this, &UDSAttributesWidget::HandleRowClicked);

		verticalBox->AddChild(Row);
		Rows.Add(RowName, Row);
	}
}

void UDSAttributesWidget::SetRowDisplay(FName RowName, int32 FinalValue, int32 UserAddedValue)
{
	if (TObjectPtr<UDSAttributeRowWidget>* Found = Rows.Find(RowName))
	{
		if (*Found)
		{
			(*Found)->SetResult(FinalValue);
			(*Found)->SetRemain(UserAddedValue);
		}
	}
}

UDSAttributeRowWidget* UDSAttributesWidget::FindRow(FName RowName) const
{
	if (const TObjectPtr<UDSAttributeRowWidget>* Found = Rows.Find(RowName))
	{
		return *Found;
	}
	return nullptr;
}

void UDSAttributesWidget::HandleRowClicked(UDSAttributeRowWidget* Row, int32 Direction)
{
	if (Row)
	{
		OnAttributeButtonClicked.Broadcast(Row->RowName, Direction);
	}
}
