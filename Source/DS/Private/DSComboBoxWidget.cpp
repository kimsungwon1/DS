
#include "DSComboBoxWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/Button.h"

void UDSComboBoxWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
	{
		return;
	}

	if (IsValid(comboBox))
	{
		comboBox->OnSelectionChanged.RemoveDynamic(this, &UDSComboBoxWidget::HandleSelectionChanged);
		comboBox->OnSelectionChanged.AddDynamic(this, &UDSComboBoxWidget::HandleSelectionChanged);
	}
	if (IsValid(ChangeButton_left))
	{
		ChangeButton_left->OnClicked.RemoveDynamic(this, &UDSComboBoxWidget::OnComboLeftClicked);
		ChangeButton_left->OnClicked.AddDynamic(this, &UDSComboBoxWidget::OnComboLeftClicked);
	}
	if (IsValid(ChangeButton_right))
	{
		ChangeButton_right->OnClicked.RemoveDynamic(this, &UDSComboBoxWidget::OnComboRightClicked);
		ChangeButton_right->OnClicked.AddDynamic(this, &UDSComboBoxWidget::OnComboRightClicked);
	}
}

void UDSComboBoxWidget::NativeDestruct()
{
	if (IsValid(comboBox))
	{
		comboBox->OnSelectionChanged.RemoveDynamic(this, &UDSComboBoxWidget::HandleSelectionChanged);
	}
	if (IsValid(ChangeButton_left))
	{
		ChangeButton_left->OnClicked.RemoveDynamic(this, &UDSComboBoxWidget::OnComboLeftClicked);
	}
	if (IsValid(ChangeButton_right))
	{
		ChangeButton_right->OnClicked.RemoveDynamic(this, &UDSComboBoxWidget::OnComboRightClicked);
	}

	Super::NativeDestruct();
}

void UDSComboBoxWidget::ClearOptions()
{
	if (IsValid(comboBox))
	{
		comboBox->ClearOptions();
	}
}

void UDSComboBoxWidget::AddOption(const FString& Option)
{
	if (IsValid(comboBox))
	{
		comboBox->AddOption(Option);
	}
}

void UDSComboBoxWidget::SetSelectedOption(const FString& Option)
{
	if (IsValid(comboBox))
	{
		comboBox->SetSelectedOption(Option);
	}
}

void UDSComboBoxWidget::SetInteractable(bool bInteractable)
{
	if (IsValid(ChangeButton_left))
	{
		ChangeButton_left->SetIsEnabled(bInteractable);
	}
	if (IsValid(ChangeButton_right))
	{
		ChangeButton_right->SetIsEnabled(bInteractable);
	}
}

void UDSComboBoxWidget::OnComboLeftClicked()
{
	CycleCombo(-1);
}

void UDSComboBoxWidget::OnComboRightClicked()
{
	CycleCombo(1);
}

FString UDSComboBoxWidget::GetSelectedOption() const
{
	return comboBox ? comboBox->GetSelectedOption() : FString();
}

void UDSComboBoxWidget::CycleCombo(int32 Direction)
{
	if (!comboBox) return;

	const int32 Count = comboBox->GetOptionCount();
	if (Count <= 0) return;

	int32 CurrentIndex = comboBox->FindOptionIndex(comboBox->GetSelectedOption());
	if (CurrentIndex == INDEX_NONE)
	{
		CurrentIndex = 0;
	}

	const int32 NewIndex = (CurrentIndex + Direction + Count) % Count;
	comboBox->SetSelectedIndex(NewIndex);
}

void UDSComboBoxWidget::HandleSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	OnSelectionChangedEvent.Broadcast(SelectedItem);
}
