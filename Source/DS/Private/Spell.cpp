


#include "Spell.h"
#include "CharacterInstanceComponent.h"
#include "DSSpellData.h"
#include "EffectBase.h"
#include "ProjectileBase.h"
#include "DSGameMode.h"
#include "DSPlayerController.h"
#include "PlayerPartyMover.h"
#include "DSParty.h"
#include "DSNPCParty.h"
#include "DSGameMode.h"

USpell::USpell()
{
}

void USpell::Initialize(UCharacterInstanceComponent* InCaster, int32 InLevel)
{
	Caster = InCaster;
	CastLevel = FMath::Clamp(InLevel, 1, 7);
}

void USpell::SpawnEffectAt(TSoftClassPtr<AEffectBase> EffectClass, FVector Location)
{
	if (EffectClass.IsNull()) return;
	UClass* Loaded = EffectClass.LoadSynchronous();
	if (!Loaded) return;
	if (UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->SpawnActor<AEffectBase>(Loaded, Location, FRotator::ZeroRotator);
		//나중에 프로젝타일처럼 PushFocus 해야하나?
	}
}

void USpell::SpawnProjectileToward(TSoftClassPtr<AProjectileBase> ProjectileClass, FVector From, FVector To)
{
	if (ProjectileClass.IsNull()) return;
	UClass* Loaded = ProjectileClass.LoadSynchronous();
	if (!Loaded) return;
	if (UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull))
	{
		FRotator Dir = (To - From).Rotation();
		if (AProjectileBase* Projectile = World->SpawnActor<AProjectileBase>(Loaded, From, Dir))
		{
			Projectile->Initialize(Caster, SpellData);
			Caster->GetDSGameMode()->PushFocus(Projectile);
		}
	}
}

bool USpell::WantsHostileTarget() const
{
	if (!SpellData) return false;
	return SpellData->spellStat.targetType == EDSTargetType::Opponent || SpellData->spellStat.targetType == EDSTargetType::OpponentParty;
}

UCharacterInstanceComponent* USpell::FindReplacementTarget()
{
	if (!Caster) return nullptr;

	ADSGameMode* GameMode = Caster->GetDSGameMode();

	TArray<ADSParty*> AllParties;
	if (ADSPlayerController* PC = GameMode->GetDSPlayerController())
	{
		if (APlayerPartyMover* Mover = PC->GetPlayerParty())
			AllParties.Add(Mover->GetParty());
	}
	for (ADSNPCParty* NpcParty : GameMode->GetNpcParties())
		AllParties.Add(NpcParty);

	for (ADSParty* Party : AllParties)
	{
		if (!Party) continue;
		for (UCharacterInstanceComponent* Candidate : Party->GetCharacters())
		{
			if (!Candidate || Candidate->IsDead()) continue;
			if (Caster->IsHostileForParam(Candidate))
			{
				return Candidate;
			}
		}
	}
	return nullptr;
}

ESpellResult USpell::CheckResult()
{

	return ESpellResult::Success;
}

void USpell::CastSpell()
{
	// StartAction 단계에서 재타겟팅했더라도 타이머 대기 중 다시 죽었을 수 있음 — 최종 안전장치
	if (!IsTargetValid())
	{
		Fizzle();
		return;
	}

	auto result = CheckResult();

	switch (result)
	{
	case ESpellResult::Success:
		Cast_Success();
		return;
	case ESpellResult::Fizzle:
		Fizzle();
		return;
	case ESpellResult::Fizzle_SparkToParty:
		Fizzle_SparkToParty();
		return;
	}
}