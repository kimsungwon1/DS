
#pragma once

#include "CoreMinimal.h"
#include "DSUserWidget.h"
#include "DSComboBoxWidget.generated.h"

class UComboBoxString;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComboSelectionChanged, FString, SelectedItem);

// 좌우 버튼으로 콤보박스 옵션을 순환시키는 위젯 (DSG_ComboBox 전용)
UCLASS()
class DS_API UDSComboBoxWidget : public UDSUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable)
	void OnComboLeftClicked();

	UFUNCTION(BlueprintCallable)
	void OnComboRightClicked();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FString GetSelectedOption() const;

	UFUNCTION(BlueprintCallable)
	void ClearOptions();

	UFUNCTION(BlueprintCallable)
	void AddOption(const FString& Option);

	UFUNCTION(BlueprintCallable)
	void SetSelectedOption(const FString& Option);

	// 좌우 버튼을 눌러도 반응 안 하게 (읽기 전용 화면 등에서 사용)
	UFUNCTION(BlueprintCallable)
	void SetInteractable(bool bInteractable);

	// 부모 위젯(DSG_CharacterStats 등)이 선택값 변화를 알아채기 위한 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnComboSelectionChanged OnSelectionChangedEvent;

protected:
	// 위젯 블루프린트에 정확히 "comboBox"라는 이름의 ComboBoxString이 있어야 함 (없으면 컴파일 에러로 바로 알림)
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString> comboBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> ChangeButton_left;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> ChangeButton_right;

private:
	void CycleCombo(int32 Direction);

	UFUNCTION()
	void HandleSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
};
