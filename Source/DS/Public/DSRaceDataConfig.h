
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DSRaceDataConfig.generated.h"

class UDSRaceData;

UCLASS(BlueprintType)
class DS_API UDSRaceDataConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Races")
	TArray<TObjectPtr<UDSRaceData>> RegisteredRaces;
};
