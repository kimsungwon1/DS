
#pragma once

#include "CoreMinimal.h"
#include "DSUserWidget.h"
#include "DSAttributeRowWidget.generated.h"

class UTextBlock;
class UButton;
class UDSAttributeRowWidget;

// Row, Direction(-1 또는 1). Row 자기 자신을 실어보내서, 상위 위젯이 RowName으로 어떤 행인지 구분함
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeRowClicked, UDSAttributeRowWidget*, Row, int32, Direction);

// DSG_AttributeRow 전용. 스탯 하나(혹은 스킬 하나)의 라벨/결과값/유저추가값/-+버튼을 담은 재사용 가능한 행 위젯.
// 상위 위젯이 이 클래스를 필요한 개수만큼 CreateWidget해서 VerticalBox 등에 쌓아 쓰면 됨.
UCLASS()
class DS_API UDSAttributeRowWidget : public UDSUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// 상위 위젯이 이 행을 만든 뒤 세팅하는 식별자 (어떤 스탯/스킬인지)
	UPROPERTY(BlueprintReadWrite)
	FName RowName;

	UFUNCTION(BlueprintCallable)
	void SetLabel(const FText& Label);

	UFUNCTION(BlueprintCallable)
	void SetResult(int32 Value);

	UFUNCTION(BlueprintCallable)
	void SetRemain(int32 Value);

	// -/+ 버튼이 눌리면 알림. 실제 값 반영 여부는 상위(예: DSG_CharacterStats)가 판단
	UPROPERTY(BlueprintAssignable)
	FOnAttributeRowClicked OnAttributeRowClicked;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> StrengthResult;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Strengthminus;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> Strengthremain;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Strengthadd;

private:
	UFUNCTION() void HandleMinusClicked();
	UFUNCTION() void HandlePlusClicked();
};
