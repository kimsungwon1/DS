

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DSWeaponRange.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EWeaponRange : uint8
{
	Melee_Short UMETA(DisplayName = "Melee Short", ToolTip = "100cm"),
	Melee_Long UMETA(DisplayName = "Melee Long", ToolTip = "200cm"),
	Shoot_Short UMETA(DisplayName = "Shoot Short", ToolTip = "5000cm"),
	Shoot_Long UMETA(DisplayName = "Shoot Long", ToolTip = "10000cm")
};

UCLASS()
class DS_API UDSWeaponRange : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static int GetWeaponRange(EWeaponRange eRangeType) 
	{
		switch (eRangeType)
		{
		case EWeaponRange::Melee_Short:
			return 100;
			break;
		case EWeaponRange::Melee_Long:
			return 200;
			break;
		case EWeaponRange::Shoot_Short:
			return 5000;
			break;
		case EWeaponRange::Shoot_Long:
			return 10000;
			break;
		default:
			return 100;
			break;
		}

	}
};
