
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DSStats.h"
#include "DSRaceDataRegistry.generated.h"

class UDSRaceData;
class UDSRaceDataConfig;

UCLASS(BlueprintType)
class DS_API UDSRaceDataRegistry : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Races")
	UDSRaceData* GetRaceDataByRace(ERace Race) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Races")
	const TArray<UDSRaceData*>& GetAllRaces() const { return AllRaces; }

private:
	TArray<TObjectPtr<UDSRaceData>> AllRaces;

	// 에셋 경로 — 에디터에서 Config 에셋 만든 뒤 경로 맞춰줘라
	static constexpr const TCHAR* ConfigAssetPath = TEXT("/Game/Made/Players/Races/RaceDataConfig");
};
