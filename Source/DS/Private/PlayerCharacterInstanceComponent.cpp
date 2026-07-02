// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterInstanceComponent.h"
#include "PlayerPartyMover.h"
#include "DSMainWidget.h"
#include "DSPlayerController.h"
#include "DSPlayerParty.h"
#include "Engine/World.h"
#include "Hands.h"
#include "Weapon.h"
#include "DSGameMode.h"
#include "ActionList.h"
#include "DSNPCParty.h"
#include "NPCCharacterInstanceComponent.h"
#include "Attack.h"
#include "Selector.h"

UPlayerCharacterInstanceComponent::UPlayerCharacterInstanceComponent()
{
	reservedBaseWeapon = AHands::StaticClass();
}

void UPlayerCharacterInstanceComponent::BeginPlay()
{
	Super::BeginPlay();

	actor = Cast<ABaseCharacter>(GetOwner());

	FinalFaceStat = BaseFaceStat;

	if (reservedWeapon)
	{
		UClass* loadedClass = reservedWeapon.LoadSynchronous();

		holdingWeapon = GetWorld()->SpawnActor<AWeapon>(loadedClass, actor->ActorToWorld());
	}

	InitializeCharacter();

	if (reservedBaseWeapon)
	{
		auto* uclass = reservedBaseWeapon.LoadSynchronous();
		baseWeapon = GetWorld()->SpawnActor<AWeapon>(uclass, actor->GetTransform());
	}
	else
	{
		baseWeapon = GetWorld()->SpawnActor<AHands>();
	}


	if (!holdingWeapon)
	{
		holdingWeapon = baseWeapon;
	}

	auto* owner = GetOwner();
	partyMover = Cast<APlayerPartyMover>(owner);
}

APlayerPartyMover* UPlayerCharacterInstanceComponent::GetPlayerPartyMover()
{
	if (!partyMover)
	{
		auto* owner = GetOwner();
		if (owner) {
			partyMover = Cast<APlayerPartyMover>(owner);
		}
	}
	return partyMover;
}
ADSPlayerController* UPlayerCharacterInstanceComponent::GetDSController()
{
	if (auto* p = GetPlayerPartyMover()) {
		return p->GetDSController();
	}
	return nullptr;
}

int UPlayerCharacterInstanceComponent::GetPartyIndex()
{
	TArray<UCharacterInstanceComponent*> members = GetParty()->GetCharacters();

	int index = 0;

	for (UCharacterInstanceComponent* m : members)
	{
		if (m == this)
		{
			return index;
		}

		index++;
	}

	return -1;
}

FVector UPlayerCharacterInstanceComponent::GetCharacterLocation() const
{
	if (const APlayerPartyMover* pm = GetPlayerPartyMover_const())
	{

		return pm->GetPlayerLocation(const_cast<const UPlayerCharacterInstanceComponent*>(this));
	}

	return FVector();
}

void UPlayerCharacterInstanceComponent::SetCharacter(int32 index)
{
	if (index < 0 || index > 6) {
		return;
	}
	pcIndex = index;
}

void UPlayerCharacterInstanceComponent::InitializeCharacter_Implementation()
{
	// 캐릭터 클래스, 종족에 따라 할수 있는 일들(ableActions)을 초기화
	ableActions.Push(NewObject<UAttack>(this));
	ableActions.Push(NewObject<USpellCast>(this));
	ableActions.Push(NewObject<UGuard>(this));
	ableActions.Push(NewObject<UGuardOther>(this));
	ableActions.Push(NewObject<UUseItem>(this));
	ableActions.Push(NewObject<UEquip>(this));
	ableActions.Push(NewObject<UHide>(this));

	for (UDSAction* ac : ableActions)
	{
		ac->Initialize(this);
	}
	// 이름 초기화
	if (characterName.IsNone())
	{
		characterName = this->GetFName();
	}
}

void UPlayerCharacterInstanceComponent::DecideAction(ADSGameMode* md)
{
	Super::DecideAction(md);
}

void UPlayerCharacterInstanceComponent::DecideAttack(ADSGameMode* md)
{
	currentAction = ableActions[0];

	auto* at = Cast<UAttack>(currentAction);

	if (at && !at->IsTargetLost())
	{
		return;
	}

	TArray<UCharacterInstanceComponent*> candidates;

	for (ADSNPCParty* np : md->GetNpcParties())
	{
		for (UNPCCharacterInstanceComponent* nc : np->characters)
		{
			if (nc == nullptr || nc->IsDead())
			{
				continue;
			}
			if (!ITargeter_DSCharacter::Execute_IsTargetValid_character(at, nc))
			{
				continue;
			}
			candidates.Add(nc);
		}
	}

	if (candidates.Num() > 0)
	{
		int32 randIdx = FMath::RandRange(0, candidates.Num() - 1);
		ITargeter_DSCharacter::Execute_SetTarget_character(at, candidates[randIdx]);
	}
	else
	{
		ITargeter_DSCharacter::Execute_SetTarget_character(at, nullptr);
	}
}

ADSParty* UPlayerCharacterInstanceComponent::GetParty()
{
	return GetPlayerPartyMover()->GetParty();
}

