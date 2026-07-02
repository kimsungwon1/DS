#pragma once

#include "CoreMinimal.h"
#include "DSUserWidget.h"
#include "DSActionButtonWidget.generated.h"

class UDSAction;
class UButton;
class UTextBlock;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionButtonClicked, UDSAction*, Action);

/**
 * 행동 목록의 단일 버튼
 */
UCLASS()
class DS_API UDSActionButtonWidget : public UDSUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetAction(UDSAction* InAction);

	UPROPERTY(BlueprintAssignable)
	FOnActionButtonClicked OnActionButtonClicked;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleClicked();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> ActionButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ActionNameText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ActionCostText;

private:
	UPROPERTY()
	TObjectPtr<UDSAction> Action;
};
