

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Party.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class DS_API AParty : public AActor
{
	GENERATED_BODY()
public:
	AParty();

	UPROPERTY(BlueprintReadWrite)
	FName name;
protected:
	UPROPERTY(BlueprintReadOnly)
	FGuid characterID;

	TObjectPtr<UBillboardComponent> VisualHelper;
public:
	UFUNCTION(BlueprintCallable)
	FGuid GetID() const { return characterID; }

	void InitializeID();
	void InitializeID(const FGuid& id);
};
