
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DSStats.h"
#include "DSRaceData.generated.h"

class AEffectBase;
class AProjectileBase;
class USpell;
class ASelector;

UCLASS(BlueprintType)
class DS_API UDSRaceData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Info")
	FName RaceName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Info")
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Info")
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat")
	ERace eRace;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat")
	FCharacterStat DefaultStat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat")
	FResistance DefaultResistance;

#if WITH_EDITOR
	// SpellClass의 Targeter 인터페이스와 SelectorClass의 ProducedTargetType 모순 검증
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif
};
