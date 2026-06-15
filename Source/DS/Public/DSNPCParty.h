

#pragma once

#include "CoreMinimal.h"
#include "DSParty.h"
#include "DSNPCParty.generated.h"
/**
 * 
 */
class UNPCCharacterInstanceComponent;

UCLASS(Blueprintable, BlueprintType)
class DS_API ADSNPCParty : public ADSParty
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UNPCCharacterInstanceComponent>> characters;

	bool bIsHostile = true;

	TArray<class UCharacterInstanceComponent*> GetCharacters() override;
};
