
#include "CharacterClassRegistry.h"
#include "CharacterClassConfig.h"
#include "CharacterClass.h"

void UCharacterClassRegistry::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UCharacterClassConfig* Config = LoadObject<UCharacterClassConfig>(nullptr, ConfigAssetPath);
	if (!Config)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterClassRegistry: Config 에셋을 찾을 수 없음 — 경로: %s"), ConfigAssetPath);
		return;
	}

	AllCharacterClasses = Config->RegisteredCharacterClasses;
	UE_LOG(LogTemp, Log, TEXT("CharacterClassRegistry: 직업 %d개 로드됨"), AllCharacterClasses.Num());
}

UCharacterClass* UCharacterClassRegistry::GetCharacterClassByProfession(EProfession Profession) const
{
	for (UCharacterClass* Class : AllCharacterClasses)
	{
		if (Class && Class->classType == Profession)
			return Class;
	}
	return nullptr;
}
