

#pragma once

#include "CoreMinimal.h"
#include "DSTarget.h"
#include "ActionData.h"
#include "DSActionData.generated.h"

/**
 * 
 */
class UDSCharacterBaseData;
class UDSCharacterBaseInstance;

UCLASS(BlueprintType, Blueprintable)
class DS_API UDSActionData : public UActionData
{
	GENERATED_BODY()

public:
	UDSActionData();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FPrimaryAssetType actionType = TEXT("DSActionData");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* actionIcon = nullptr;
};
