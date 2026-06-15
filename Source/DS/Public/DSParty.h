

#pragma once

#include "CoreMinimal.h"
#include "Party.h"
#include "DSParty.generated.h"
/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class DS_API ADSParty : public AParty
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	virtual TArray<class UCharacterInstanceComponent*> GetCharacters() { return TArray<class UCharacterInstanceComponent*>(); }
};
