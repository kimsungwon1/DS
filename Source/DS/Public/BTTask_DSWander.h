#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_DSWander.generated.h"

/**
 * Picks a random reachable point within WanderRadius of the pawn and writes it
 * to the NextLocation blackboard key, then finishes with Success.
 * Pair with a MoveTo task (NextLocation key) and a Wait task to form a wander loop.
 */
UCLASS()
class DS_API UBTTask_DSWander : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_DSWander();

	// BB key to write the chosen wander destination into
	UPROPERTY(EditAnywhere, Category = "Wander")
	FBlackboardKeySelector NextLocationKey;

	// Search radius around the pawn for a reachable wander point
	UPROPERTY(EditAnywhere, Category = "Wander", meta = (ClampMin = 100.f))
	float WanderRadius = 600.f;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
};
