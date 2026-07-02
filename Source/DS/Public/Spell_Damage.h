
#pragma once

#include "CoreMinimal.h"
#include "Spell.h"
#include "Targeter_DSCharacter.h"
#include "Spell_Damage.generated.h"

// 단일 대상 피해 마법
UCLASS(Blueprintable)
class DS_API USpell_Damage : public USpell, public ITargeter_DSCharacter
{
	GENERATED_BODY()

public:
	bool IsTargetValid_character_Implementation(UCharacterInstanceComponent* targetCharacter) override;
	void SetTarget_character_Implementation(UCharacterInstanceComponent* targetCharacter) override { Target = targetCharacter; }

protected:
	virtual void Cast_Success_Implementation() override;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCharacterInstanceComponent> Target;
};
