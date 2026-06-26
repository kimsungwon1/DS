

#pragma once

#include "CoreMinimal.h"
#include "DSParty.h"
#include "DSNPCParty.generated.h"

class UNPCCharacterInstanceComponent;
class ABaseNPC;

UCLASS(Blueprintable, BlueprintType)
class DS_API ADSNPCParty : public ADSParty
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UNPCCharacterInstanceComponent>> characters;

	UPROPERTY(EditAnywhere)
	bool bIsHostile = true;

	TArray<class UCharacterInstanceComponent*> GetCharacters() override;

	// --- Encounter Spawner ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
	TSubclassOf<ABaseNPC> NPCClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter", meta = (ClampMin = 1, ClampMax = 20))
	int32 Count = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter", meta = (ClampMin = 30.f))
	float SpawnRadius = 120.f;

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
};
