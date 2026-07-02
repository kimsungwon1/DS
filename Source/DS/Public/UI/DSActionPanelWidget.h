#pragma once

#include "CoreMinimal.h"
#include "DSUserWidget.h"
#include "DSActionPanelWidget.generated.h"

class UPlayerCharacterInstanceComponent;
class UVerticalBox;
class UScrollBox;
class UTextBlock;
class UDSActionButtonWidget;
class UDSAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionSelected, UDSAction*, SelectedAction);

/**
 * 선택된 캐릭터의 행동 목록을 표시하는 패널
 * 공격, 마법, 방어 등 행동 버튼들을 세로로 나열
 */
UCLASS()
class DS_API UDSActionPanelWidget : public UDSUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetActiveCharacter(UPlayerCharacterInstanceComponent* InCharacter);

	UFUNCTION(BlueprintCallable)
	void RefreshActions();

	UPROPERTY(BlueprintAssignable)
	FOnActionSelected OnActionSelected;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleActionButtonClicked(UDSAction* Action);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UScrollBox> ActionListBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ActiveCharacterNameText;

	// 액션 버튼 위젯 클래스 (BP에서 지정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UDSActionButtonWidget> ActionButtonClass;

private:
	UPROPERTY()
	TObjectPtr<UPlayerCharacterInstanceComponent> ActiveCharacter;
};
