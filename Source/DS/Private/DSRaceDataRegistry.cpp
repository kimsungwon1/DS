
#include "DSRaceDataRegistry.h"
#include "DSRaceDataConfig.h"
#include "DSRaceData.h"

void UDSRaceDataRegistry::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UDSRaceDataConfig* Config = LoadObject<UDSRaceDataConfig>(nullptr, ConfigAssetPath);
	if (!Config)
	{
		UE_LOG(LogTemp, Warning, TEXT("DSRaceDataRegistry: Config 에셋을 찾을 수 없음 — 경로: %s"), ConfigAssetPath);
		return;
	}

	AllRaces = Config->RegisteredRaces;
	UE_LOG(LogTemp, Log, TEXT("DSRaceDataRegistry: 종족 %d개 로드됨"), AllRaces.Num());
}

UDSRaceData* UDSRaceDataRegistry::GetRaceDataByRace(ERace Race) const
{
	for (UDSRaceData* Data : AllRaces)
	{
		if (Data && Data->eRace == Race)
			return Data;
	}
	return nullptr;
}
