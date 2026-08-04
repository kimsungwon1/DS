

#include "DSDoorBase.h"
#include "DSKeyRingComponent.h"
#include "DSBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerPartyMover.h"

ADSDoorBase::ADSDoorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	DoorLeafRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DoorLeafRoot"));
	DoorLeafRoot->SetupAttachment(RootComponent);
}

void ADSDoorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!DoorLeafRoot)
	{
		return;
	}

	FRotator CurrentRot = DoorLeafRoot->GetRelativeRotation();
	if (FMath::IsNearlyEqual(CurrentRot.Yaw, TargetYaw, 0.05f))
	{
		return;
	}

	CurrentRot.Yaw = FMath::FInterpTo(CurrentRot.Yaw, TargetYaw, DeltaTime, RotationInterpSpeed);
	DoorLeafRoot->SetRelativeRotation(CurrentRot);
}

bool ADSDoorBase::CanOpen(AActor* Interactor) const
{
	if (bIsUnlocked)
	{
		return true;
	}

	// 방향 제한이 걸려있으면 다른 조건 볼 것도 없이 먼저 걸러냄
	const EDoorSide Side = GetInteractorSide(Interactor);
	if (AllowedSide != EDoorSide::Either && Side != AllowedSide)
	{
		return false;
	}

	// 아무 조건도 안 걸려있으면 그냥 열리는 문
	if (RequiredKeyIDs.Num() == 0 && !bRequiresButton)
	{
		return true;
	}

	if (bRequiresButton && bButtonPressed)
	{
		return true;
	}

	if (RequiredKeyIDs.Num() > 0 && Interactor)
	{
		if (UDSKeyRingComponent* KeyRing = Interactor->FindComponentByClass<UDSKeyRingComponent>())
		{
			for (const FName& KeyID : RequiredKeyIDs)
			{
				if (KeyRing->HasKey(KeyID))
				{
					return true;
				}
			}
		}
	}

	return CheckSkillOrSpellCondition(Interactor);
}

bool ADSDoorBase::TryOpen(AActor* Interactor)
{
	if (!CanOpen(Interactor))
	{
		return false;
	}

	// 열쇠로 열린 거면 그 열쇠의 충전을 하나 소모 (0 되면 KeyRing에서 알아서 사라짐)
	if (!bIsUnlocked && RequiredKeyIDs.Num() > 0 && Interactor)
	{
		if (UDSKeyRingComponent* KeyRing = Interactor->FindComponentByClass<UDSKeyRingComponent>())
		{
			for (const FName& KeyID : RequiredKeyIDs)
			{
				if (KeyRing->ConsumeKey(KeyID))
				{
					break;
				}
			}
		}
	}

	bIsUnlocked = true;
	Open();

	return true;
}

void ADSDoorBase::Open_Implementation()
{
	bIsOpen = true;
	TargetYaw = OpenYaw;
}

void ADSDoorBase::Close_Implementation()
{
	if (bStaysOpenForever)
	{
		return;
	}

	bIsOpen = false;
	TargetYaw = 0.0f;
}

bool ADSDoorBase::IsInteractable_Implementation(APlayerPartyMover* ppm, float distance)
{
	return distance <= UDSBlueprintLibrary::GetDefaultReachDistance();
}

bool ADSDoorBase::Interact_Implementation(APlayerPartyMover* ppm, FText& textToExpress)
{
	if (!ppm)
	{
		return false;
	}

	// 열려있으면 닫기 시도(bStaysOpenForever면 Close 내부에서 무시됨), 닫혀있으면 열기 시도
	if (bIsOpen)
	{
		Close();
	}
	else
	{
		TryOpen(ppm);
	}
	return true;
}
