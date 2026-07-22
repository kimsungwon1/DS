
#include "DSMessageBoxYesNoWidget.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/Button.h"
#include "Engine/World.h"

void UDSMessageBoxYesNoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
	{
		return;
	}

	if (IsValid(YesButton))
	{
		YesButton->OnClicked.RemoveDynamic(this, &UDSMessageBoxYesNoWidget::OnYesClicked);
		YesButton->OnClicked.AddDynamic(this, &UDSMessageBoxYesNoWidget::OnYesClicked);
	}
	if (IsValid(NoButton))
	{
		NoButton->OnClicked.RemoveDynamic(this, &UDSMessageBoxYesNoWidget::OnNoClicked);
		NoButton->OnClicked.AddDynamic(this, &UDSMessageBoxYesNoWidget::OnNoClicked);
	}
}

void UDSMessageBoxYesNoWidget::NativeDestruct()
{
	if (IsValid(YesButton))
	{
		YesButton->OnClicked.RemoveDynamic(this, &UDSMessageBoxYesNoWidget::OnYesClicked);
	}
	if (IsValid(NoButton))
	{
		NoButton->OnClicked.RemoveDynamic(this, &UDSMessageBoxYesNoWidget::OnNoClicked);
	}

	Super::NativeDestruct();
}

UDSMessageBoxYesNoWidget* UDSMessageBoxYesNoWidget::Show(UObject* WorldContextObject, TSubclassOf<UDSMessageBoxYesNoWidget> MessageBoxClass, const FText& Message, int32 ZOrder)
{
	if (!WorldContextObject || !MessageBoxClass) return nullptr;

	UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	if (!World) return nullptr;

	UDSMessageBoxYesNoWidget* MsgBox = CreateWidget<UDSMessageBoxYesNoWidget>(World, MessageBoxClass);
	if (!MsgBox) return nullptr;

	MsgBox->SetMessage(Message);
	MsgBox->AddToViewport(ZOrder);
	return MsgBox;
}

void UDSMessageBoxYesNoWidget::SetMessage(const FText& Message)
{
	if (Text)
	{
		Text->SetText(Message);
	}
}

void UDSMessageBoxYesNoWidget::OnYesClicked()
{
	OnResult.Broadcast(true);
	RemoveFromParent();
}

void UDSMessageBoxYesNoWidget::OnNoClicked()
{
	OnResult.Broadcast(false);
	RemoveFromParent();
}
