#include "DSNPCParty.h"
#include "NPCCharacterInstanceComponent.h"
#include "BaseNPC.h"
#include "NavigationSystem.h"
#include "Components/CapsuleComponent.h"

// ZOffset = capsule half-height so the actor root lands at the right height above the floor.
// NavMesh projection returns the floor surface Z; we must lift by ZOffset to place the capsule centre correctly.
static bool FindValidSpawnLocation(UWorld* World, const FVector& Origin, const FVector& Candidate, float Radius, float ZOffset, FVector& OutLocation)
{
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
    if (NavSys)
    {
        FNavLocation NavLoc;
        const FVector Extent(60.f, 60.f, 200.f);
        if (NavSys->ProjectPointToNavigation(Candidate, NavLoc, Extent))
        {
            OutLocation = NavLoc.Location + FVector(0.f, 0.f, ZOffset);
            return true;
        }
        // Try a handful of random points within Radius
        for (int32 Attempt = 0; Attempt < 6; Attempt++)
        {
            const float A = FMath::FRandRange(0.f, 2.f * PI);
            const float D = FMath::FRandRange(30.f, Radius * 0.6f);
            const FVector Rnd = Origin + FVector(FMath::Cos(A) * D, FMath::Sin(A) * D, 0.f);
            if (NavSys->ProjectPointToNavigation(Rnd, NavLoc, Extent))
            {
                OutLocation = NavLoc.Location + FVector(0.f, 0.f, ZOffset);
                return true;
            }
        }
    }

    // Editor fallback: floor trace then capsule wall check
    const FCollisionQueryParams TraceParams(NAME_None, false);
    FHitResult Hit;
    const FVector Up(0, 0, 200.f), Down(0, 0, -500.f);
    if (World->LineTraceSingleByChannel(Hit, Candidate + Up, Candidate + Down, ECC_WorldStatic, TraceParams))
    {
        const FVector Centre = Hit.Location + FVector(0.f, 0.f, ZOffset);
        const FCollisionShape Cap = FCollisionShape::MakeCapsule(34.f, ZOffset);
        if (!World->OverlapBlockingTestByChannel(Centre, FQuat::Identity, ECC_WorldStatic, Cap, TraceParams))
        {
            OutLocation = Centre;
            return true;
        }
    }

    return false;
}

TArray<class UCharacterInstanceComponent*> ADSNPCParty::GetCharacters()
{
    TArray<class UCharacterInstanceComponent*> ret;
    for (UNPCCharacterInstanceComponent* nc : characters)
    {
        ret.Add(nc);
    }
    return ret;
}

void ADSNPCParty::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    if (HasAnyFlags(RF_ClassDefaultObject) || !NPCClass || Count <= 0)
        return;

    UWorld* World = GetWorld();
    if (!World || World->IsGameWorld())
        return; // Preview only in editor; BeginPlay handles in-game spawning

    // Remove previous preview actors
    TArray<AActor*> Attached;
    GetAttachedActors(Attached);
    for (AActor* A : Attached)
    {
        if (A && A->Tags.Contains(FName("DSEncounterPreview")))
            A->Destroy();
    }

    FActorSpawnParameters Params;
    Params.ObjectFlags |= RF_Transient;

    const FVector Origin = GetActorLocation();
    const float RingRadius = Count > 1 ? SpawnRadius : 0.f;

    // Read capsule half-height from CDO so the spawn Z lands the character on the floor, not buried in it.
    float ZOffset = 88.f;
    if (ACharacter* CDO = NPCClass->GetDefaultObject<ACharacter>())
        ZOffset = CDO->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

    for (int32 i = 0; i < Count; i++)
    {
        const float Angle = (i / float(Count)) * 2.f * PI;
        const FVector Candidate = Origin + FVector(FMath::Cos(Angle) * RingRadius, FMath::Sin(Angle) * RingRadius, 0.f);

        FVector SpawnLoc;
        if (!FindValidSpawnLocation(World, Origin, Candidate, SpawnRadius, ZOffset, SpawnLoc))
            SpawnLoc = Candidate + FVector(0.f, 0.f, ZOffset); // best-effort for editor preview

        AActor* Spawned = World->SpawnActor<AActor>(NPCClass, SpawnLoc, FRotator::ZeroRotator, Params);
        if (Spawned)
        {
            Spawned->Tags.Add(FName("DSEncounterPreview"));
            Spawned->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
        }
    }
}

void ADSNPCParty::BeginPlay()
{
    Super::BeginPlay();

    // Destroy editor preview actors
    TArray<AActor*> Attached;
    GetAttachedActors(Attached);
    for (AActor* A : Attached)
    {
        if (A && A->Tags.Contains(FName("DSEncounterPreview")))
            A->Destroy();
    }

    if (!NPCClass || Count <= 0)
        return;

    UWorld* World = GetWorld();
    if (!World)
        return;

    const FVector Origin = GetActorLocation();
    const float RingRadius = Count > 1 ? SpawnRadius : 0.f;

    float ZOffset = 88.f;
    if (ACharacter* CDO = NPCClass->GetDefaultObject<ACharacter>())
        ZOffset = CDO->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

    for (int32 i = 0; i < Count; i++)
    {
        const float Angle = (i / float(Count)) * 2.f * PI;
        const FVector Candidate = Origin + FVector(FMath::Cos(Angle) * RingRadius, FMath::Sin(Angle) * RingRadius, 0.f);

        FVector SpawnLoc;
        if (!FindValidSpawnLocation(World, Origin, Candidate, SpawnRadius, ZOffset, SpawnLoc))
            continue; // skip if no valid floor found

        const FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLoc);
        ABaseNPC* NPC = World->SpawnActorDeferred<ABaseNPC>(NPCClass, SpawnTransform);
        if (!NPC) continue;

        UNPCCharacterInstanceComponent* Comp = NPC->FindComponentByClass<UNPCCharacterInstanceComponent>();
        if (Comp)
        {
            Comp->SetNPCParty(this);
            characters.Add(Comp);
        }

        NPC->FinishSpawning(SpawnTransform);
    }
}
