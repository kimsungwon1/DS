
#pragma once

#include "CoreMinimal.h"
#include "DSUserWidget.h"
#include "DSAttributeRowWidget.h"
#include "DSAttributesWidget.generated.h"

class UVerticalBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeButtonClicked, FName, RowName, int32, Direction);

// 이름-값 목록(스킬, 스탯, 뭐든)을 UDSAttributeRowWidget(DSG_AttributeRow) 인스턴스를 세로로 쌓아서 채우는 위젯.
// DSG_PrimaryAttributes가 DSG_CharacterStats에서 쓰이는 것과 같은 방식으로 쓸 수 있음.
UCLASS()
class DS_API UDSAttributesWidget : public UDSUserWidget
{
	GENERATED_BODY()

public:
	UDSAttributesWidget();

	// 기존 행을 전부 지우고, 주어진 이름들로 새로 행을 만듦
	UFUNCTION(BlueprintCallable)
	void SetupRows(const TArray<FName>& RowNames);

	// 행 하나의 결과값/유저추가값 갱신
	UFUNCTION(BlueprintCallable)
	void SetRowDisplay(FName RowName, int32 FinalValue, int32 UserAddedValue);

	// 이름으로 행 위젯 찾기 (없으면 nullptr). 직업이 못 쓰는 스킬 숨기기 등 직접 조작할 때 사용
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UDSAttributeRowWidget* FindRow(FName RowName) const;

	// -/+ 버튼이 눌리면 알림 (Direction: -1 또는 +1). 실제 값 반영 여부는 상위(예: DSG_CharacterStats)가 판단
	UPROPERTY(BlueprintAssignable)
	FOnAttributeButtonClicked OnAttributeButtonClicked;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UVerticalBox> verticalBox;

	// DSG_AttributeRow(UDSAttributeRowWidget 상속) 클래스. 기본값은 생성자에서 자동으로 잡아줌
	UPROPERTY(EditDefaultsOnly, Category = "Row")
	TSubclassOf<UDSAttributeRowWidget> RowWidgetClass;

private:
	UFUNCTION()
	void HandleRowClicked(UDSAttributeRowWidget* Row, int32 Direction);

	UPROPERTY()
	TMap<FName, TObjectPtr<UDSAttributeRowWidget>> Rows;
};
