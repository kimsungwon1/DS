

#include "DSDoorBase.h"
#include "DSKeyRingComponent.h"

ADSDoorBase::ADSDoorBase()
{
	PrimaryActorTick.bCanEverTick = false;
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
}

void ADSDoorBase::Close_Implementation()
{
	if (bStaysOpenForever)
	{
		return;
	}

	bIsOpen = false;
}
