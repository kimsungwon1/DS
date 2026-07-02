


#include "Attack.h"
#include "CharacterInstanceComponent.h"
#include "NPCCharacterInstanceComponent.h"
#include "DSPlayerController.h"
#include "Weapon.h"
#include "Hands.h"

UAttack::UAttack() : usingWeapon{ nullptr }
{
	actionName = "Attack";
}

UAttack::~UAttack()
{
}

bool UAttack::IsTargetValid_character_Implementation(UCharacterInstanceComponent* targetCharacter)
{
	FVector attackerPos = actor->GetCharacterLocation();
	FVector receiverPos = targetCharacter->GetCharacterLocation();
	float distance = FVector::Dist2D(attackerPos, receiverPos);

	AWeapon* wp = usingWeapon;

	if (wp)
	{
		float weaponRange = wp->GetDistanceToAttack();
		if (distance <= weaponRange)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void UAttack::ReadyAction_Implementation()
{
	// 나중에 캐릭터의 스탯에 따라 카운트 변화시키기
	actionCount = 1;

	if (UNPCCharacterInstanceComponent* NpcTarget = Cast<UNPCCharacterInstanceComponent>(target))
	{
		if (AActor* NpcActor = NpcTarget->GetOwner())
		{
			if (ADSPlayerController* PC = Cast<ADSPlayerController>(
				GetWorld()->GetFirstPlayerController()))
			{
				PC->FocusOnActor(NpcActor, 2.f, 2.f);
			}
		}
	}
}

void UAttack::StartAction_Implementation()
{
	Super::StartAction_Implementation();

	if (target && target->IsDead())
	{
		actor->DecideAttack(actor->GetDSGameMode());
	}

	if (actionCount > 0 && target && !target->IsDead())
	{
		GetWorld()->GetTimerManager().SetTimer(
			AttackTimerHandle,
			this,
			&UAttack::ExecuteAction_Implementation,
			0.8f,
			false
		);
	}
	else
	{
		EndAction_Implementation();
	}
}

void UAttack::ExecuteAction_Implementation()
{
	Super::ExecuteAction_Implementation();
	actionCount--;
	if (target)
	{
		target->ReceiveDamage(actor);
	}
}

void UAttack::Initialize_Implementation(UCharacterInstanceComponent* ci)
{
	Super::Initialize_Implementation(ci);
	usingWeapon = const_cast<AWeapon*>(actor->GetHoldingWeapon());
}

bool UAttack::IsTargetLost_Implementation() const
{
	return target == nullptr || target->IsDead();
}

