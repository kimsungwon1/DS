
#pragma once

#include "CoreMinimal.h"
#include "DSUserWidget.h"
#include "SaveGameData.h"
#include "DSCharacterSaveEntryWidget.generated.h"

class UImage;
class UTextBlock;

class UDSCharacterSaveEntryWidget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveEntryClicked, UDSCharacterSaveEntryWidget*, Entry);

// DSG_CharacterSaveEntry 전용. RosterList 안에 세이브데이터 하나당 하나씩 들어가는 행.
UCLASS()
class DS_API UDSCharacterSaveEntryWidget : public UDSUserWidget
{
	GENERATED_BODY()

public:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	// 세이브데이터로 이 행의 초상화/이름/직업/종족 텍스트를 채움
	UFUNCTION(BlueprintCallable)
	void SetCharacterData(const FPlayerCharacterSaveData& InData);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const FPlayerCharacterSaveData& GetCharacterData() const { return CharacterData; }

	// 이 행이 클릭되면 자기 자신을 넘겨서 알림 (상위가 CharacterData를 읽어서 CenterColumn에 반영)
	UPROPERTY(BlueprintAssignable)
	FOnSaveEntryClicked OnEntryClicked;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> RosterEntryPortrait;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> RosterEntryName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> RosterEntryClass;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> RosterEntryRace;

private:
	UPROPERTY()
	FPlayerCharacterSaveData CharacterData;
};
