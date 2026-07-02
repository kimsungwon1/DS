
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SpellRegistryConfig.generated.h"

class UDSSpellData;

UCLASS(BlueprintType)
class DS_API USpellRegistryConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spells")
	TArray<TObjectPtr<UDSSpellData>> RegisteredSpells;
};
