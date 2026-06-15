

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "Targeter_PositionVector.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTargeter_PositionVector : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DS_API ITargeter_PositionVector
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsTargetValid_position(FVector targetPosition);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetTarget_position(FVector targetPosition);
};
