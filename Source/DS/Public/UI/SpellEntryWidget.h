
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

	UPROPERTY(BlueprintAssignable)
	FOnSpellEntryClicked OnSpellEntryClicked;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleClicked();

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
};
