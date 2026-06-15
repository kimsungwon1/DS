// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPartyMover.h"
#include "PlayerCharacterInstanceComponent.h"
#include "PlayerPartyManagerComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "DSPlayerController.h"
#include "DSPlayerParty.h"

// Sets default values
APlayerPartyMover::APlayerPartyMover() : capsuleAllocatedPlayersNum{0,0,0,0,0,0}
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	MyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	MyCamera->SetupAttachment(SpringArm);

	playerCharacterManager = CreateDefaultSubobject<UPlayerPartyManagerComponent>(TEXT("PlayerCharacters"));
	playerCharacterManager->RegisterComponent();

	timelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));
	timelineComp->RegisterComponent();

	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;

	UCapsuleComponent* cap = Cast<UCapsuleComponent>(RootComponent);
	capsulesForCharacters.Add(cap);

	for (int i = 0; i < 4; i++)
	{
		UCapsuleComponent* newCap = CreateDefaultSubobject<UCapsuleComponent>(*FString::Printf(TEXT("Capsule%d"), i + 1));

		newCap->SetupAttachment(RootComponent);

		// newCap->RegisterComponent(); 
		
		newCap->SetCapsuleSize(34, 88);

		// newCap->SetCollisionProfileName(TEXT("Pawn"));

		capsulesForCharacters.Add(newCap);
	}

	capsulesForCharacters[1]->SetRelativeLocation(FVector(100.0, 0.0, 0.0));
	capsulesForCharacters[2]->SetRelativeLocation(FVector(0.0, 100.0, 0.0));
	capsulesForCharacters[3]->SetRelativeLocation(FVector(-100.0, 0.0, 0.0));
	capsulesForCharacters[4]->SetRelativeLocation(FVector(0.0, -100.0, 0.0));

	TArray<FVector> vCap{ {70.0, 70.0, 0.0}, {-70.0, 70.0, 0.0}, {-70.0, -70.0, 0.0}, {70.0, -70.0, 0.0}, };

	for (int i = 0; i < 4; i++)
	{
		UCapsuleComponent* newCap = CreateDefaultSubobject<UCapsuleComponent>(*FString::Printf(TEXT("BlockerCapsule%d"), i));

		newCap->SetupAttachment(RootComponent);

		// newCap->RegisterComponent();

		newCap->SetCapsuleSize(34, 88);

		// newCap->SetCollisionProfileName(TEXT("Pawn"));

		newCap->SetRelativeLocation(vCap[i]);

		capsulesForJustBlocking.Add(newCap);
	}
}

// Called when the game starts or when spawned
void APlayerPartyMover::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeCapsulesAndPlayers();

	for (UCapsuleComponent* cp : capsulesForJustBlocking)
	{
		cp->SetCollisionProfileName(TEXT("Pawn"));
	}

	for (UCapsuleComponent* cp : capsulesForCharacters)
	{
		cp->SetCollisionProfileName(TEXT("Pawn"));
	}
}

// Called every frame
void APlayerPartyMover::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerPartyMover::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerPartyMover::TurnAround(const FVector2D& value)
{
	AddControllerYawInput(value.Y);
	AddControllerPitchInput(value.X);
}

ADSPlayerController* APlayerPartyMover::GetDSController()
{
	if (!dsPlayerController)
	{
		auto* controller = GetController();
		if (controller) {
			dsPlayerController = Cast<ADSPlayerController>(controller);
		}
	}
	return dsPlayerController;
}

void APlayerPartyMover::InitializeCapsulesAndPlayers()
{
}

bool APlayerPartyMover::AllocatePlayerCapsule(UPlayerCharacterInstanceComponent* player, int capsuleIndex)
{
	if (capsuleAllocatedPlayersNum[capsuleIndex] == 3)
	{
		return false;
	}

	if (capsuleIndexAllocatedToPlayers.Contains(player))
	{
		int capsuleIndexPast = capsuleIndexAllocatedToPlayers[player];

		capsuleAllocatedPlayersNum[capsuleIndexPast]--;

		capsuleIndexAllocatedToPlayers.Remove(player);
	}
	capsuleAllocatedPlayersNum[capsuleIndex]++;

	UCapsuleComponent* capsule = capsulesForCharacters[capsuleIndex];

	capsuleIndexAllocatedToPlayers.Add(player, capsuleIndex);

	return true;
}

FVector APlayerPartyMover::GetPlayerLocation(const UPlayerCharacterInstanceComponent* player) const
{
	FVector retLocation;
	if (capsuleIndexAllocatedToPlayers.Contains(player))
	{
		int capIndex = capsuleIndexAllocatedToPlayers[player];

		retLocation = capsulesForCharacters[capIndex]->GetComponentLocation();
	}
	else 
	{
		retLocation = GetActorLocation();
	}

	return retLocation;
}

ADSParty* APlayerPartyMover::GetParty()
{
	return playerCharacterManager->partyObject;
}