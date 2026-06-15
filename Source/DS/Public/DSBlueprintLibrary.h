

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DSBlueprintLibrary.generated.h"

/**
 * 
 */

class UDSAction;

UENUM(BlueprintType)
enum class EWeaponRangeType : uint8
{
	Short UMETA(DisplayName = "Short"),
	Extended UMETA(DisplayName = "Extended"),
	Long UMETA(DisplayName = "Long")
};

UCLASS()
class DS_API UDSBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "Combat")
	static float GetDefaultReachDistance() { return 100.0f; }

	UFUNCTION(BlueprintPure, Category = "Combat")
	static float GetCombatStartDistance() { return 600.0f; }

	UFUNCTION(BlueprintPure, Category = "Combat")
	static float GetAttackRange(EWeaponRangeType RangeType)
	{
		switch (RangeType)
		{
		case EWeaponRangeType::Short:    return 200.0f;
		case EWeaponRangeType::Extended: return 350.0f;
		case EWeaponRangeType::Long:     return 1000.0f;
		default:                         return 0.0f;
		}
	}
};
