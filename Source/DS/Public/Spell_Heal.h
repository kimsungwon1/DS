
#pragma once

#include "CoreMinimal.h"
#include "Spell.h"
#include "Targeter_DSCharacter.h"
#include "Spell_Heal.generated.h"

// 단일 대상 또는 파티 회복 마법 (투사체 없음)
UCLASS(Blueprintable)
class DS_API USpell_Heal : public USpell, public ITargeter_DSCharacter
{
	GENERATED_BODY()

public:
	bool IsTargetValid_character_Implementation(UCharacterInstanceComponent* targetCharacter) override;
	void SetTarget_character_Implementation(UCharacterInstanceComponent* targetCharacter) override { Target = targetCharacter; }

protected:
	virtual void Cast_Success_Implementation() override;

	UFUNCTION(BlueprintNativeEvent)
	void ApplyHeal(UCharacterInstanceComponent* HealTarget);
	virtual void ApplyHeal_Implementation(UCharacterInstanceComponent* HealTarget);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Heal")
	bool bTargetParty = false;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCharacterInstanceComponent> Target;
};
