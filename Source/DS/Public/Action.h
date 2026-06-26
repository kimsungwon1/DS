

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "Action.generated.h"

/**
 * 
 */

UCLASS(Abstract, BlueprintType, Blueprintable)
class DS_API UAction : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FName GetActionName() const { return actionName; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName actionName;
};
