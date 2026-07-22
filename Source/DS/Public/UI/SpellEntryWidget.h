
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpellEntryWidget.generated.h"

class UDSSpellData;
class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpellEntryClicked, UDSSpellData*, SpellData);

UCLASS()
class DS_API USpellEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetSpellData(UDSSpellData* InSpellData);

	// 선택 상태 설정 — 선택되면 Cyan, 해제되면 White로 복귀
	UFUNCTION(BlueprintCallable)
	void SetSelected(bool bInSelected);

	UFUNCTION(BlueprintCallable)
	bool IsSelected() const { return bSelected; }

	UFUNCTION(BlueprintCallable)
	UDSSpellData* GetSpellData() const { return SpellData; }

	UPROPERTY(BlueprintAssignable)
	FOnSpellEntryClicked OnSpellEntryClicked;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleClicked();

	UFUNCTION()
	void HandleHovered();

	UFUNCTION()
	void HandleUnhovered();

	void ApplyTextColor(const FLinearColor& Color);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> EntryButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> SpellNameText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> MPCostText;

private:
	UPROPERTY()
	TObjectPtr<UDSSpellData> SpellData;

	bool bSelected = false;
};
