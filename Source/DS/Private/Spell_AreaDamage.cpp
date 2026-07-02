
#include "Spell_AreaDamage.h"
#include "CharacterInstanceComponent.h"
#include "NPCCharacterInstanceComponent.h"
#include "DSSpellData.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"

void USpell_AreaDamage::Cast_Success_Implementation()
{
	if (!Caster) return;

	UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return;

	if (SpellData)
	{
		if (Caster->GetOwner())
			SpawnEffectAt(SpellData->CasterEffect, Caster->GetOwner()->GetActorLocation());

		if (!SpellData->Projectile.IsNull())
		{
			FVector From = Caster->GetOwner() ? Caster->GetOwner()->GetActorLocation() : FVector::ZeroVector;
			SpawnProjectileToward(SpellData->Projectile, From, TargetPosition);
		}
		else
		{
			SpawnEffectAt(SpellData->TargetEffect, TargetPosition);

			// 범위 내 NPC에게 피해
			for (TActorIterator<AActor> It(World); It; ++It)
			{
				if (UCharacterInstanceComponent* CIC = It->FindComponentByClass<UCharacterInstanceComponent>())
				{
					if (CIC->IsA<UNPCCharacterInstanceComponent>() && !CIC->IsDead())
					{
						if (FVector::Dist(It->GetActorLocation(), TargetPosition) <= Radius)
						{
							CIC->ReceiveSpellDamage(Caster, SpellData);
						}
					}
				}
			}
		}
	}
}
