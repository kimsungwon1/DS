
#include "DSMessageBoxWidget.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"

void UDSMessageBoxWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
	{
		return;
	}

	if (IsValid(CloseButton))
	{
		CloseButton->OnClicked.RemoveDynamic(this, &UDSMessageBoxWidget::OnCloseButtonClicked);
		CloseButton->OnClicked.AddDynamic(this, &UDSMessageBoxWidget::OnCloseButtonClicked);
	}
}

void UDSMessageBoxWidget::NativeDestruct()
{
	if (IsValid(CloseButton))
	{
		CloseButton->OnClicked.RemoveDynamic(this, &UDSMessageBoxWidget::OnCloseButtonClicked);
	}

	Super::NativeDestruct();
}

UDSMessageBoxWidget* UDSMessageBoxWidget::Show(UObject* WorldContextObject, TSubclassOf<UDSMessageBoxWidget> MessageBoxClass, const FText& Message, int32 ZOrder)
{
	if (!WorldContextObject || !MessageBoxClass) return nullptr;

	UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	if (!World) return nullptr;

	UDSMessageBoxWidget* MsgBox = CreateWidget<UDSMessageBoxWidget>(World, MessageBoxClass);
	if (!MsgBox) return nullptr;

	MsgBox->SetMessage(Message);
	MsgBox->AddToViewport(ZOrder);
	return MsgBox;
}

void UDSMessageBoxWidget::SetMessage(const FText& Message)
{
	if (Text)
	{
		Text->SetText(Message);
	}
}

void UDSMessageBoxWidget::OnCloseButtonClicked()
{
	OnClosed.Broadcast();
	RemoveFromParent();
}
