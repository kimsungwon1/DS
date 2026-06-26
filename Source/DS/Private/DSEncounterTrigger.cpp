#include "DSEncounterTrigger.h"
#include "DSNPCParty.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "NavigationSystem.h"

ADSEncounterTrigger::ADSEncounterTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetBoxExtent(FVector(200.f, 200.f, 100.f));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = TriggerBox;
}

void ADSEncounterTrigger::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ADSEncounterTrigger::OnTriggerBeginOverlap);
}

void ADSEncounterTrigger::OnTriggerBeginOverlap(UPrimitiveComponent*, AActor* OtherActor,
	UPrimitiveComponent*, int32, bool, const FHitResult&)
{
	if (bTriggered || !OtherActor) return;

	// Only react to locally controlled player character
	ACharacter* AsChar = Cast<ACharacter>(OtherActor);
	if (!AsChar) return;
	APlayerController* PC = Cast<APlayerController>(AsChar->GetController());
	if (!PC || !PC->IsLocalController()) return;

	TrySpawnParty(OtherActor);
}

void ADSEncounterTrigger::TrySpawnParty(AActor* InstigatorActor)
{
	if (!PartyClass) return;

	UWorld* World = GetWorld();
	if (!World) return;

	bTriggered = true;

	FVector SpawnOrigin = GetActorLocation();

	if (bSpawnOutOfSight)
	{
		SpawnOrigin = FindOutOfSightSpawnOrigin(InstigatorActor);
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ADSNPCParty* Party = World->SpawnActor<ADSNPCParty>(PartyClass, SpawnOrigin, FRotator::ZeroRotator, Params);
	(void)Party;
}

FVector ADSEncounterTrigger::FindOutOfSightSpawnOrigin(AActor* PlayerActor) const
{
	UWorld* World = GetWorld();
	if (!World || !PlayerActor) return GetActorLocation();

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	const FVector TriggerCenter = GetActorLocation();
	const FVector PlayerLoc = PlayerActor->GetActorLocation();

	// Try random points around trigger; pick first one the player can't see
	constexpr int32 MaxAttempts = 12;
	for (int32 i = 0; i < MaxAttempts; i++)
	{
		const float Angle = (i / float(MaxAttempts)) * 2.f * PI + FMath::FRandRange(0.f, 0.3f);
		const float Dist = FMath::FRandRange(OutOfSightSearchRadius * 0.4f, OutOfSightSearchRadius);
		const FVector Candidate = TriggerCenter + FVector(FMath::Cos(Angle) * Dist, FMath::Sin(Angle) * Dist, 0.f);

		// Check that NavMesh reaches this point
		if (NavSys)
		{
			FNavLocation NavLoc;
			if (!NavSys->ProjectPointToNavigation(Candidate, NavLoc, FVector(100.f, 100.f, 200.f)))
				continue;
		}

		// Line-of-sight check: if trace hits something between player and candidate → out of sight
		FHitResult Hit;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(PlayerActor);
		const bool bBlocked = World->LineTraceSingleByChannel(Hit, PlayerLoc, Candidate, ECC_Visibility, QueryParams);
		if (bBlocked)
			return Candidate;
	}

	// Fallback: spawn directly behind the player relative to trigger centre
	const FVector DirAwayFromPlayer = (TriggerCenter - PlayerLoc).GetSafeNormal();
	return TriggerCenter + DirAwayFromPlayer * OutOfSightSearchRadius * 0.5f;
}
