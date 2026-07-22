
#pragma once

#include "CoreMinimal.h"
#include "DSUserWidget.h"
#include "DSSkillsWidget.generated.h"

class UDSAttributesWidget;

// DSG_Skills 전용. 무기/피지컬/아카데미아 세 스킬 그룹(각자 DSAttributesWidget)을 묶어놓은 위젯.
// 상위 위젯(DSG_CharacterStats 등)이 각 그룹에 SetupRows/SetRowDisplay를 직접 호출할 수 있게 public으로 노출.
UCLASS()
class DS_API UDSSkillsWidget : public UDSUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UDSAttributesWidget> DSG_WeaponarySkillAttributes;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UDSAttributesWidget> DSG_PhysicalSkillAttributes;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UDSAttributesWidget> DSG_AcademiaSkillAttributes;
};
