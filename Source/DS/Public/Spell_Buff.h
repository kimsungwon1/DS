
#pragma once

#include "CoreMinimal.h"
#include "Spell.h"
#include "Targeter_DSCharacter.h"
#include "Spell_Buff.generated.h"

// 버프/디버프 마법 (투사체 없음)
UCLASS(Blueprintable)
class DS_API USpell_Buff : public USpell, public ITargeter_DSCharacter
{
	GENERATED_BODY()

public:
	bool IsTargetValid_character_Implementation(UCharacterInstanceComponent* targetCharacter) override;
	void SetTarget_character_Implementation(UCharacterInstanceComponent* targetCharacter) override { Target = targetCharacter; }

protected:
	virtual void Cast_Success_Implementation() override;

	// 블루프린트에서 실제 버프 로직 구현
	UFUNCTION(BlueprintNativeEvent)
	void ApplyBuff(UCharacterInstanceComponent* BuffTarget);
	virtual void ApplyBuff_Implementation(UCharacterInstanceComponent* BuffTarget) {}

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCharacterInstanceComponent> Target;
};
