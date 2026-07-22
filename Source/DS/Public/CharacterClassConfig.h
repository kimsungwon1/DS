
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassConfig.generated.h"

class UCharacterClass;

UCLASS(BlueprintType)
class DS_API UCharacterClassConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterClasses")
	TArray<TObjectPtr<UCharacterClass>> RegisteredCharacterClasses;
};
