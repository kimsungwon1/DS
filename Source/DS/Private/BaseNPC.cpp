


#include "BaseNPC.h"
#include "NPCCharacterInstanceComponent.h"

// Sets default values
ABaseNPC::ABaseNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	npcCharacterComponent = CreateDefaultSubobject<UNPCCharacterInstanceComponent>(TEXT("NPCCharacterComponent"));
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


