
#include "Spell_Heal.h"
#include "CharacterInstanceComponent.h"
#include "DSPlayerParty.h"
#include "DSSpellData.h"
#include "EngineUtils.h"

void USpell_Heal::ApplyHeal_Implementation(UCharacterInstanceComponent* HealTarget)
{
	int32 Amount = SpellData ? FMath::RoundToInt(FMath::RandRange(SpellData->MinPower, SpellData->MaxPower)) : 0;
	HealTarget->ReceiveHeal(Amount);
}

bool USpell_Heal::IsTargetValid_character_Implementation(UCharacterInstanceComponent* targetCharacter)
{
	return targetCharacter && !targetCharacter->IsDead();
}

void USpell_Heal::Cast_Success_Implementation()
{
	if (!Caster) return;

	if (SpellData && Caster->GetOwner())
		SpawnEffectAt(SpellData->CasterEffect, Caster->GetOwner()->GetActorLocation());

	if (bTargetParty)
	{
		// 파티 전체 회복 — Caster의 파티를 찾아서 순회
		if (UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull))
		{
			for (TActorIterator<ADSPlayerParty> It(World); It; ++It)
			{
				for (UCharacterInstanceComponent* Member : It->GetCharacters())
				{
					if (Member && !Member->IsDead())
					{
						ApplyHeal(Member);
						if (SpellData && Member->GetOwner())
							SpawnEffectAt(SpellData->TargetEffect, Member->GetOwner()->GetActorLocation());
					}
				}
			}
		}
	}
	else
	{
		if (!Target) return;
		ApplyHeal(Target);
		if (SpellData && Target->GetOwner())
			SpawnEffectAt(SpellData->TargetEffect, Target->GetOwner()->GetActorLocation());
	}
}
