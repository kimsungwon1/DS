
#include "Spell_Damage.h"
#include "CharacterInstanceComponent.h"
#include "DSSpellData.h"

bool USpell_Damage::IsTargetValid_character_Implementation(UCharacterInstanceComponent* targetCharacter)
{
	return targetCharacter && !targetCharacter->IsDead();
}

void USpell_Damage::Cast_Success_Implementation()
{
	if (!Target || !Caster) return;

	if (SpellData)
	{
		// 시전자 이펙트
		if (Caster->GetOwner())
			SpawnEffectAt(SpellData->CasterEffect, Caster->GetOwner()->GetActorLocation());

		if (!SpellData->Projectile.IsNull())
		{
			// 투사체 발사 — 투사체가 명중 처리
			FVector From = Caster->GetOwner() ? Caster->GetOwner()->GetActorLocation() : FVector::ZeroVector;
			FVector To   = Target->GetOwner()  ? Target->GetOwner()->GetActorLocation()  : FVector::ZeroVector;
			SpawnProjectileToward(SpellData->Projectile, From, To);
		}
		else
		{
			// 즉발 — 바로 피해 적용 + 대상 이펙트
			Target->ReceiveSpellDamage(Caster, SpellData);
			if (Target->GetOwner())
				SpawnEffectAt(SpellData->TargetEffect, Target->GetOwner()->GetActorLocation());
		}
	}
}
