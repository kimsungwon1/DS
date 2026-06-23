#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTService_DSDetectPlayer.generated.h"

/**
 * Each tick: finds the nearest player character within DetectionRadius.
 * - If found: sets TargetActorKey to that player, sets IsWanderingKey = false
 * - If lost: clears TargetActorKey, sets IsWanderingKey = true
 *
 * Wire this as a service on the root or on a combat branch in BT_UnitAI.
 */
UCLASS()
class DS_API UBTService_DSDetectPlayer : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_DSDetectPlayer();

	// BB key (Actor Object) to store the detected player
	UPROPERTY(EditAnywhere, Category = "Detection")
	FBlackboardKeySelector TargetActorKey;

	// BB key (Bool) — true while wandering (no player seen), false when player detected
	UPROPERTY(EditAnywhere, Category = "Detection")
	FBlackboardKeySelector IsWanderingKey;

	// Sight radius; player must be within this distance to be detected
	UPROPERTY(EditAnywhere, Category = "Detection", meta = (ClampMin = 100.f))
	float DetectionRadius = 1200.f;

	// If true, performs a line-of-sight trace so obstacles block detection
	UPROPERTY(EditAnywhere, Category = "Detection")
	bool bRequireLineOfSight = true;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
};
