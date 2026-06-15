

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DSTarget.h"
#include "ActionData.generated.h"

/**
 * 
 */


UCLASS(BlueprintType, Blueprintable)
class DS_API UActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName actionName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EDSTargetType targetType;
};
