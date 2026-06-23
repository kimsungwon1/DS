// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPartyMover.h"
#include "PlayerCharacterInstanceComponent.h"
#include "PlayerPartyManagerComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "DSPlayerController.h"
#include "DSPlayerParty.h"

// 캡슐별 상하좌우 위치 (로컬 좌표, Z=0)
static const FVector CapsuleLocalPositions[5] =
{
	{   0.f,    0.f, 0.f },  // 0: 중앙
	{ 100.f,    0.f, 0.f },  // 1: 앞
	{   0.f,  100.f, 0.f },  // 2: 오른쪽
	{-100.f,    0.f, 0.f },  // 3: 뒤
	{   0.f, -100.f, 0.f },  // 4: 왼쪽
};

// 대각선 충돌 방지 캡슐 위치 (로컬 좌표)
static const FVector BlockerLocalPositions[4] =
{
	{  70.f,  70.f, 0.f },
	{ -70.f,  70.f, 0.f },
	{ -70.f, -70.f, 0.f },
	{  70.f, -70.f, 0.f },
};

APlayerPartyMover::APlayerPartyMover()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;

	MyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	MyCamera->SetupAttachment(SpringArm);

	playerCharacterManager = CreateDefaultSubobject<UPlayerPartyManagerComponent>(TEXT("PlayerCharacters"));
	timelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));

	// 캐릭터 캡슐 5개 생성
	// [0]은 ACharacter 기본 루트 캡슐을 재사용
	UCapsuleComponent* RootCap = Cast<UCapsuleComponent>(RootComponent);
	CapsulesForCharacters.Add(RootCap);

	for (int32 i = 1; i < NumCapsules; i++)
	{
		UCapsuleComponent* Cap = CreateDefaultSubobject<UCapsuleComponent>(
			*FString::Printf(TEXT("CharCapsule%d"), i));
		Cap->SetupAttachment(RootComponent);
		Cap->SetCapsuleSize(34.f, 88.f);
		Cap->SetRelativeLocation(CapsuleLocalPositions[i]);
		CapsulesForCharacters.Add(Cap);
	}

	// 대각선 충돌 방지 캡슐 4개 생성
	for (int32 i = 0; i < 4; i++)
	{
		UCapsuleComponent* Cap = CreateDefaultSubobject<UCapsuleComponent>(
			*FString::Printf(TEXT("BlockerCapsule%d"), i));
		Cap->SetupAttachment(RootComponent);
		Cap->SetCapsuleSize(34.f, 88.f);
		Cap->SetRelativeLocation(BlockerLocalPositions[i]);
		CapsulesForBlocking.Add(Cap);
	}

	// 슬롯 배열 초기화 (15칸, 전부 nullptr)
	CapsuleSlots.SetNum(NumCapsules * PlayersPerCapsule);
}

void APlayerPartyMover::BeginPlay()
{
	Super::BeginPlay();

	for (UCapsuleComponent* Cap : CapsulesForCharacters)
		Cap->SetCollisionProfileName(TEXT("Pawn"));

	for (UCapsuleComponent* Cap : CapsulesForBlocking)
		Cap->SetCollisionProfileName(TEXT("Pawn"));
}

void APlayerPartyMover::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

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
		if (AController* Controller = GetController())
			dsPlayerController = Cast<ADSPlayerController>(Controller);
	}
	return dsPlayerController.Get();
}

int32 APlayerPartyMover::FindBestCapsule() const
{
	int32 BestIndex = -1;
	int32 MinCount = PlayersPerCapsule; // 꽉 찬 캡슐은 제외

	for (int32 Ci = 0; Ci < NumCapsules; Ci++)
	{
		int32 Count = 0;
		for (int32 Si = 0; Si < PlayersPerCapsule; Si++)
		{
			if (CapsuleSlots[Ci * PlayersPerCapsule + Si] != nullptr)
				Count++;
		}
		if (Count < MinCount)
		{
			MinCount = Count;
			BestIndex = Ci;
		}
	}
	return BestIndex;
}

bool APlayerPartyMover::AllocatePlayerCapsule(UPlayerCharacterInstanceComponent* Player, int32 CapsuleIndex)
{
	if (!Player) return false;

	// 이미 할당돼 있으면 먼저 해제
	if (PlayerToSlot.Contains(Player))
		DeallocatePlayerCapsule(Player);

	// -1이면 자동으로 가장 비어있는 캡슐 선택
	if (CapsuleIndex < 0)
		CapsuleIndex = FindBestCapsule();

	if (CapsuleIndex < 0 || CapsuleIndex >= NumCapsules)
		return false;

	// 빈 슬롯 탐색
	for (int32 Si = 0; Si < PlayersPerCapsule; Si++)
	{
		int32 FlatIndex = CapsuleIndex * PlayersPerCapsule + Si;
		if (CapsuleSlots[FlatIndex] == nullptr)
		{
			CapsuleSlots[FlatIndex] = Player;
			PlayerToSlot.Add(Player, FlatIndex);
			return true;
		}
	}

	return false; // 해당 캡슐이 꽉 참
}

void APlayerPartyMover::DeallocatePlayerCapsule(UPlayerCharacterInstanceComponent* Player)
{
	if (!Player) return;

	if (const int32* FlatIndex = PlayerToSlot.Find(Player))
	{
		CapsuleSlots[*FlatIndex] = nullptr;
		PlayerToSlot.Remove(Player);
	}
}

FVector APlayerPartyMover::GetPlayerLocation(const UPlayerCharacterInstanceComponent* Player) const
{
	if (!Player) return GetActorLocation();

	const int32* FlatIndex = PlayerToSlot.Find(Player);
	if (!FlatIndex) return GetActorLocation();

	const int32 CapsuleIndex = *FlatIndex / PlayersPerCapsule;

	return CapsulesForCharacters[CapsuleIndex]->GetComponentLocation();
}

int32 APlayerPartyMover::GetPlayerCapsuleIndex(const UPlayerCharacterInstanceComponent* Player) const
{
	if (!Player) return -1;
	const int32* FlatIndex = PlayerToSlot.Find(Player);
	return FlatIndex ? (*FlatIndex / PlayersPerCapsule) : -1;
}

ADSParty* APlayerPartyMover::GetParty()
{
	return playerCharacterManager->partyObject;
}
