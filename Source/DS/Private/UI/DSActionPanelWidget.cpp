#include "UI/DSActionPanelWidget.h"
#include "UI/DSActionButtonWidget.h"
#include "PlayerCharacterInstanceComponent.h"
#include "DSAction.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

void UDSActionPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDSActionPanelWidget::SetActiveCharacter(UPlayerCharacterInstanceComponent* InCharacter)
{
	ActiveCharacter = InCharacter;
	RefreshActions();
}

void UDSActionPanelWidget::RefreshActions()
{
	if (!ActionListBox) return;

	ActionListBox->ClearChildren();

	if (!ActiveCharacter) return;

	if (ActiveCharacterNameText)
		ActiveCharacterNameText->SetText(FText::FromName(ActiveCharacter->characterName));

	if (!ActionButtonClass) return;

	for (int32 i = 0; i < ActiveCharacter->GetActionCount(); ++i)
	{
		UDSAction* Action = ActiveCharacter->GetActionByIndex(i);
		if (!Action) continue;

		UDSActionButtonWidget* Btn = CreateWidget<UDSActionButtonWidget>(this, ActionButtonClass);
		if (!Btn) continue;

		Btn->SetAction(Action);
		Btn->OnActionButtonClicked.AddDynamic(this, &UDSActionPanelWidget::HandleActionButtonClicked);
		ActionListBox->AddChild(Btn);
	}
}

void UDSActionPanelWidget::HandleActionButtonClicked(UDSAction* Action)
{
	OnActionSelected.Broadcast(Action);
}
