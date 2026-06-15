

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "Targeter_DSCharacter.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTargeter_DSCharacter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DS_API ITargeter_DSCharacter
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsTargetValid_character(class UCharacterInstanceComponent* targetCharacter);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetTarget_character(class UCharacterInstanceComponent* targetCharacter);
};
