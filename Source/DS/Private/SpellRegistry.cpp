
#include "SpellRegistry.h"
#include "SpellRegistryConfig.h"
#include "DSSpellData.h"

void USpellRegistry::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	USpellRegistryConfig* Config = LoadObject<USpellRegistryConfig>(nullptr, ConfigAssetPath);
	if (!Config)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpellRegistry: Config 에셋을 찾을 수 없음 — 경로: %s"), ConfigAssetPath);
		return;
	}

	AllSpells = Config->RegisteredSpells;
	UE_LOG(LogTemp, Log, TEXT("SpellRegistry: 스펠 %d개 로드됨"), AllSpells.Num());
}

UDSSpellData* USpellRegistry::GetSpellByName(FName SpellName) const
{
	for (UDSSpellData* Spell : AllSpells)
	{
		if (Spell && Spell->SpellName == SpellName)
			return Spell;
	}
	return nullptr;
}
