#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DSEncounterTrigger.generated.h"

class UBoxComponent;
class ADSNPCParty;

UCLASS(Blueprintable)
class DS_API ADSEncounterTrigger : public AActor
{
	GENERATED_BODY()

public:
	ADSEncounterTrigger();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

private:
	void TrySpawnParty(AActor* InstigatorActor);
	FVector FindOutOfSightSpawnOrigin(AActor* PlayerActor) const;

public:
	// Party class to spawn when triggered
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
	TSubclassOf<ADSNPCParty> PartyClass;

	// Spawn search radius passed to the party
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter", meta = (ClampMin = 60.f))
	float SpawnSearchRadius = 500.f;

	// If true, try to spawn at a location not in the player's line of sight
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
	bool bSpawnOutOfSight = true;

	// Max distance from trigger centre to look for an out-of-sight spawn origin
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter", meta = (ClampMin = 100.f))
	float OutOfSightSearchRadius = 800.f;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> TriggerBox;

	bool bTriggered = false;
};
