
#pragma once

#include "CoreMinimal.h"
#include "DSUserWidget.h"
#include "DSMessageBoxYesNoWidget.generated.h"

class UMultiLineEditableTextBox;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageBoxYesNoResult, bool, bYes);

// DSG_MessageBox_YesNo 전용. 메시지 + Yes/No 버튼 두 개.
// UDSMessageBoxYesNoWidget::Show(...)로 뷰포트에 띄우고 OnResult 바인딩해서 선택 받으면 됨
UCLASS()
class DS_API UDSMessageBoxYesNoWidget : public UDSUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = "MessageBox")
	static UDSMessageBoxYesNoWidget* Show(UObject* WorldContextObject, TSubclassOf<UDSMessageBoxYesNoWidget> MessageBoxClass, const FText& Message, int32 ZOrder = 1000);

	UFUNCTION(BlueprintCallable, Category = "MessageBox")
	void SetMessage(const FText& Message);

	// Yes/No 버튼 눌렸을 때 (선택 후 자기 자신을 뷰포트에서 제거함)
	UPROPERTY(BlueprintAssignable)
	FOnMessageBoxYesNoResult OnResult;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UMultiLineEditableTextBox> Text;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> YesButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> NoButton;

private:
	UFUNCTION() void OnYesClicked();
	UFUNCTION() void OnNoClicked();
};
