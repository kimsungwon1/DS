#include "BTService_DSDetectPlayer.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UBTService_DSDetectPlayer::UBTService_DSDetectPlayer()
{
	NodeName = TEXT("DS Detect Player");
	Interval = 0.25f;       // check 4× per second
	RandomDeviation = 0.05f;

	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_DSDetectPlayer, TargetActorKey), AActor::StaticClass());
	IsWanderingKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_DSDetectPlayer, IsWanderingKey));
}

void UBTService_DSDetectPlayer::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	if (UBlackboardData* BBData = GetBlackboardAsset())
	{
		TargetActorKey.ResolveSelectedKey(*BBData);
		IsWanderingKey.ResolveSelectedKey(*BBData);
	}
}

void UBTService_DSDetectPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC) return;

	APawn* Pawn = AIC->GetPawn();
	if (!Pawn) return;

	UWorld* World = Pawn->GetWorld();
	if (!World) return;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return;

	const FVector PawnLoc = Pawn->GetActorLocation();

	// Gather all player pawns
	AActor* BestTarget = nullptr;
	float BestDistSq = FMath::Square(DetectionRadius);

	for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (!PC) continue;
		APawn* PlayerPawn = PC->GetPawn();
		if (!PlayerPawn || PlayerPawn == Pawn) continue;

		const float DistSq = FVector::DistSquared(PawnLoc, PlayerPawn->GetActorLocation());
		if (DistSq > BestDistSq) continue;

		if (bRequireLineOfSight)
		{
			FHitResult Hit;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(Pawn);
			Params.AddIgnoredActor(PlayerPawn);
			const bool bBlocked = World->LineTraceSingleByChannel(Hit, PawnLoc, PlayerPawn->GetActorLocation(), ECC_Visibility, Params);
			if (bBlocked) continue; // something between them
		}

		BestDistSq = DistSq;
		BestTarget = PlayerPawn;
	}

	if (BestTarget)
	{
		BB->SetValueAsObject(TargetActorKey.SelectedKeyName, BestTarget);
		BB->SetValueAsBool(IsWanderingKey.SelectedKeyName, false);
	}
	else
	{
		// SeenPlayer 클리어는 기존 전투 종료 로직에 맡김 — 여기선 IsWandering만 업데이트
		BB->SetValueAsBool(IsWanderingKey.SelectedKeyName, true);
	}
}
