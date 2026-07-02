
#pragma once

#include "CoreMinimal.h"
#include "Spell.h"
#include "Targeter_PositionVector.h"
#include "Spell_AreaDamage.generated.h"

// 위치 지정 범위 피해 마법
UCLASS(Blueprintable)
class DS_API USpell_AreaDamage : public USpell, public ITargeter_PositionVector
{
	GENERATED_BODY()

public:
	bool IsTargetValid_position_Implementation(FVector targetPosition) override { return true; }
	void SetTarget_position_Implementation(FVector targetPosition) override { TargetPosition = targetPosition; }

protected:
	virtual void Cast_Success_Implementation() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Area")
	float Radius = 300.f;

	UPROPERTY(BlueprintReadOnly)
	FVector TargetPosition = FVector::ZeroVector;
};
