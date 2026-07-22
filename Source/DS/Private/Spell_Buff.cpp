
#include "Spell_Buff.h"
#include "CharacterInstanceComponent.h"
#include "DSSpellData.h"
#include "DSNPCParty.h"
#include "DSGameMode.h"
#include "PlayerCharacterInstanceComponent.h"

bool USpell_Buff::IsTargetValid_character_Implementation(UCharacterInstanceComponent* targetCharacter)
{
	return targetCharacter && !targetCharacter->IsDead();
}

void USpell_Buff::SetTarget_character_Implementation(UCharacterInstanceComponent* targetCharacter)
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

bool USpell_Buff::IsTargetValid() const
{
	if (SpellData && SpellData->spellStat.targetType == EDSTargetType::OpponentParty)
	{
		if (Target->GetParty()->IsWipedOut())
		{
			return false;
		}
		else 
		{
			return true;
		}
	}
	else
	{
		return Target && !Target->IsDead();
	}
}

UCharacterInstanceComponent* USpell_Buff::FindReplacementTarget()
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

	return nullptr;
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
