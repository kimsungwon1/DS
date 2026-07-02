#include "UI/DSActionButtonWidget.h"
#include "DSAction.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UDSActionButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ActionButton)
		ActionButton->OnClicked.AddDynamic(this, &UDSActionButtonWidget::HandleClicked);
}

void UDSActionButtonWidget::SetAction(UDSAction* InAction)
{
	Action = InAction;
	if (!InAction) return;

	if (ActionNameText)
		ActionNameText->SetText(FText::FromName(InAction->GetActionName()));

	if (ActionCostText)
		ActionCostText->SetText(FText::GetEmpty());
}

void UDSActionButtonWidget::HandleClicked()
{
	if (Action)
		OnActionButtonClicked.Broadcast(Action);
}
