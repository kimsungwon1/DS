


#include "Spell.h"
#include "CharacterInstanceComponent.h"
#include "DSSpellData.h"
#include "EffectBase.h"
#include "ProjectileBase.h"

USpell::USpell()
{
}

void USpell::Initialize(UCharacterInstanceComponent* InCaster, int32 InLevel)
{
	Caster = InCaster;
	CastLevel = FMath::Clamp(InLevel, 1, 6);
}

void USpell::SpawnEffectAt(TSoftClassPtr<AEffectBase> EffectClass, FVector Location)
{
	if (EffectClass.IsNull()) return;
	UClass* Loaded = EffectClass.LoadSynchronous();
	if (!Loaded) return;
	if (UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->SpawnActor<AEffectBase>(Loaded, Location, FRotator::ZeroRotator);
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
		World->SpawnActor<AProjectileBase>(Loaded, From, Dir);
	}
}

ESpellResult USpell::CheckResult()
{

	return ESpellResult::Success;
}

void USpell::Cast()
{
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