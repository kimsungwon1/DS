
#include "Spell_Damage.h"
#include "CharacterInstanceComponent.h"
#include "DSSpellData.h"
#include "DSNPCParty.h"
#include "DSGameMode.h"
#include "PlayerCharacterInstanceComponent.h"

bool USpell_Damage::IsTargetValid_character_Implementation(UCharacterInstanceComponent* targetCharacter)
{
	return targetCharacter && !targetCharacter->IsDead();
}

void USpell_Damage::SetTarget_character_Implementation(UCharacterInstanceComponent* targetCharacter)
{
	Target = targetCharacter;

	if (Caster->IsA<UPlayerCharacterInstanceComponent>())
	{
		UPlayerCharacterInstanceComponent* pcic = ::Cast<UPlayerCharacterInstanceComponent>(Caster);
		
		if (Target)
		{
			Target->BeTarget(pcic->GetPartyIndex(), false);
		}
		if (targetCharacter)
		{
			targetCharacter->BeTarget(pcic->GetPartyIndex(), true);
		}
	}
}

bool USpell_Damage::IsTargetValid() const
{
	if (SpellData && SpellData->spellStat.targetType == EDSTargetType::OpponentParty && Target)
	{
		return !Target->GetParty()->IsWipedOut();
	}
	return Target && !Target->IsDead();
}

UCharacterInstanceComponent* USpell_Damage::FindReplacementTarget()
{
	if (SpellData && SpellData->spellStat.targetType == EDSTargetType::OpponentParty && Target && Target->GetParty()->IsWipedOut())
	{
		for (ADSNPCParty* npcParty : Caster->GetDSGameMode()->GetNpcParties())
		{
			if (npcParty != Target->GetParty() && !npcParty->IsWipedOut())
			{
				Target = npcParty->GetCharacters()[0];
				return Target;
			}
		}
	}
	else if (Target)
	{
		for (UCharacterInstanceComponent* cic : Target->GetParty()->GetCharacters())
		{
			if (!cic->IsDead())
			{
				Target = cic;
				return cic;
			}
		}
	}
	
	return nullptr;
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
