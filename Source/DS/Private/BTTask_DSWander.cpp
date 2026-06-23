#include "BTTask_DSWander.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/Pawn.h"

UBTTask_DSWander::UBTTask_DSWander()
{
	NodeName = TEXT("DS Wander");
	NextLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_DSWander, NextLocationKey));
}

void UBTTask_DSWander::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	if (UBlackboardData* BBData = GetBlackboardAsset())
		NextLocationKey.ResolveSelectedKey(*BBData);
}

EBTNodeResult::Type UBTTask_DSWander::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) return EBTNodeResult::Failed;

	APawn* Pawn = AIC->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;

	UWorld* World = Pawn->GetWorld();
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if (!NavSys) return EBTNodeResult::Failed;

	FNavLocation NavLoc;
	if (!NavSys->GetRandomReachablePointInRadius(Pawn->GetActorLocation(), WanderRadius, NavLoc))
		return EBTNodeResult::Failed;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (BB)
		BB->SetValueAsVector(NextLocationKey.SelectedKeyName, NavLoc.Location);

	return EBTNodeResult::Succeeded;
}
