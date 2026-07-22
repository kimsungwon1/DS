
#pragma once

#include "CoreMinimal.h"
#include "DSUserWidget.h"
#include "DSMessageBoxWidget.generated.h"

class UMultiLineEditableTextBox;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMessageBoxClosed);

// DSG_MessageBox_Close 전용. 메시지 하나 + 닫기 버튼만 있는 단순 알림창.
// UDSMessageBoxWidget::Show(...)로 뷰포트에 바로 띄워서 씀 (Qt의 QMessageBox::information처럼)
UCLASS()
class DS_API UDSMessageBoxWidget : public UDSUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// 메시지 창을 만들어서 바로 뷰포트에 띄움
	UFUNCTION(BlueprintCallable, Category = "MessageBox")
	static UDSMessageBoxWidget* Show(UObject* WorldContextObject, TSubclassOf<UDSMessageBoxWidget> MessageBoxClass, const FText& Message, int32 ZOrder = 1000);

	UFUNCTION(BlueprintCallable, Category = "MessageBox")
	void SetMessage(const FText& Message);

	// 닫기 버튼 눌렸을 때 (알림 후 자기 자신을 뷰포트에서 제거함)
	UPROPERTY(BlueprintAssignable)
	FOnMessageBoxClosed OnClosed;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UMultiLineEditableTextBox> Text;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

private:
	UFUNCTION()
	void OnCloseButtonClicked();
};
