
#include "Spell_Buff.h"
#include "CharacterInstanceComponent.h"
#include "DSSpellData.h"

bool USpell_Buff::IsTargetValid_character_Implementation(UCharacterInstanceComponent* targetCharacter)
{
	return targetCharacter && !targetCharacter->IsDead();
}

void USpell_Buff::Cast_Success_Implementation()
{
	if (!Target || !Caster) return;

	if (SpellData)
	{
		if (Caster->GetOwner())
			SpawnEffectAt(SpellData->CasterEffect, Caster->GetOwner()->GetActorLocation());
		if (Target->GetOwner())
			SpawnEffectAt(SpellData->TargetEffect, Target->GetOwner()->GetActorLocation());
	}

	ApplyBuff(Target);
}
