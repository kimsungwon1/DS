


#include "BaseNPC.h"
#include "NPCCharacterInstanceComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABaseNPC::ABaseNPC()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// RVO avoidance keeps NPCs from physically walking through each other while moving
	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceWeight = 0.5f;
}

// Called when the game starts or when spawned
void ABaseNPC::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseNPC::Initialize_CharacterDefine()
{
	if (!softNpcPtr.IsNull())
	{
		UClass* loadedClass = softNpcPtr.LoadSynchronous();
		if (loadedClass)
		{
			auto* newCharacter = NewObject<UNPCCharacterInstanceComponent>(this, loadedClass);
			newCharacter->RegisterComponent();
		}
	}
}

// Called every frame
void ABaseNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


