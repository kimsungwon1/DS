

#pragma once

#include "CoreMinimal.h"
#include "DSParty.h"
#include "DSPlayerParty.generated.h"
/**
 * 
 */

class UPlayerCharacterInstanceComponent;

UCLASS(Blueprintable, BlueprintType)
class DS_API ADSPlayerParty : public ADSParty
{
	GENERATED_BODY()
public:
	ADSPlayerParty();

	TArray<class UCharacterInstanceComponent*> GetCharacters() override;
public:
	UPROPERTY(EditAnywhere, EditFixedSize, BlueprintReadWrite, Category = "RPG")
	TArray<TObjectPtr<UPlayerCharacterInstanceComponent>> characters;
};
