

#pragma once

#include "CoreMinimal.h"
#include "DSCharacterBaseData.h"
#include "DSNPCCharacterData.generated.h"
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class DS_API UDSNPCCharacterData : public UDSCharacterBaseData
{
	GENERATED_BODY()
public:
	UDSNPCCharacterData();
	~UDSNPCCharacterData();
};
