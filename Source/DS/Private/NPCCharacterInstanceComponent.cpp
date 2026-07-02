// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCCharacterInstanceComponent.h"
#include "DSCharacterBaseData.h"
#include "ActionList.h"
#include "Spell.h"
#include "DSGameMode.h"
#include "DSNPCParty.h"
#include "DSPlayerController.h"
#include "PlayerPartyMover.h"
#include "DSPlayerParty.h"
#include "PlayerCharacterInstanceComponent.h"

UNPCCharacterInstanceComponent::UNPCCharacterInstanceComponent()
{
}

void UNPCCharacterInstanceComponent::BeginPlay()
{
	Super::BeginPlay();

	for(TSoftClassPtr<USpell> spellClass : ableSpellsCandidates)
	{
		if (spellClass)
		{
			auto* uclass = spellClass.LoadSynchronous();
			auto* newSpell = NewObject<USpell>(this, uclass);

			ableSpells.Add(newSpell);
		}
	}

	if (npcParty == nullptr)
	{
		FActorSpawnParameters param;

		ADSNPCParty* party = GetWorld()->SpawnActor<ADSNPCParty>(ADSNPCParty::StaticClass(), param);

		party->characters.Add(this);

		npcParty = party;
	}
}

void UNPCCharacterInstanceComponent::InitializeCharacter_Implementation()
{
	// 캐릭터 클래스, 종족에 따라 할수 있는 일들(ableActions)을 초기화
	ableActions.Push(NewObject<UAttack>(this));
	ableActions.Push(NewObject<USpellCast>(this));
	ableActions.Push(NewObject<UMove>(this));

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

void UNPCCharacterInstanceComponent::JoinToBattle(AActor* aactor)
{
	auto* gm = GetDSGameMode();

	if (gm)
	{
		gm->JoinNpcToBattle(this, aactor);
	}
}

void UNPCCharacterInstanceComponent::DecideAction(ADSGameMode* md)
{
	int sum = actionPriority.ToAttackNormal + actionPriority.ToSpell + actionPriority.ToRun;

	int randValue = FMath::RandRange(1, sum);

	if (randValue <= actionPriority.ToAttackNormal)
	{
		currentAction = ableActions[0];
		DecideAttack(md);
	}
	else if (randValue <= actionPriority.ToAttackNormal + actionPriority.ToSpell)
	{
		currentAction = ableActions[1];
	}
	else
	{
		currentAction = ableActions[2];
	}
}

void UNPCCharacterInstanceComponent::DecideAttack(ADSGameMode* md)
{
	TArray<ADSParty*> parties;
	TArray<UCharacterInstanceComponent*> candidates;

	currentAction = ableActions[0];

	auto* at = Cast<UAttack>(currentAction);

	// 현재 타겟이 살아있으면 바꾸지 않는다
	if (at && !at->IsTargetLost())
	{
		return;
	}

	if (npcParty->bIsHostile)
	{
		auto* playerParty = md->GetDSPlayerController()->GetPlayerParty()->GetParty();

		parties.Add(playerParty);

		for (auto* np : md->GetNpcParties())
		{
			if (!np->bIsHostile)
			{
				parties.Add(np);
			}
		}
	}
	else
	{
		for (auto* np : md->GetNpcParties())
		{
			if (np->bIsHostile)
			{
				parties.Add(np);
			}
		}
	}

	for (auto* party : parties)
	{
		if (party->IsA(ADSNPCParty::StaticClass()))
		{
			auto* np = Cast<ADSNPCParty>(party);
			for (UNPCCharacterInstanceComponent* c : np->characters)
			{
				if (!c->IsDead())
				{
					candidates.Add(c);
				}
			}
		}
		if (party->IsA(ADSPlayerParty::StaticClass()))
		{
			auto* pp = Cast<ADSPlayerParty>(party);
			for (UPlayerCharacterInstanceComponent* c : pp->characters)
			{
				if (c != nullptr && !c->IsDead())
				{
					candidates.Add(c);
				}
			}
		}
	}

	if (candidates.Num() > 0)
	{
		int32 randIdx = FMath::RandRange(0, candidates.Num() - 1);
		ITargeter_DSCharacter::Execute_SetTarget_character(at, candidates[randIdx]);
	}
}

void UNPCCharacterInstanceComponent::ReceiveTurn_Implementation()
{
	if (IsDead())
	{
		actor->GetDSGameMode()->PopFocus(this);
		return;
	}

	currentAction->ReadyAction();

	bIsGotTurn = true;
}

void UNPCCharacterInstanceComponent::SwitchTurn()
{
	bIsGotTurn = false;

	moveCount = moveCountMax;

	GetDSGameMode()->SwitchTurn();
}

bool UNPCCharacterInstanceComponent::IsGotTurn()
{
	return Super::IsGotTurn(); // bIsGotTurn;
}

bool UNPCCharacterInstanceComponent::IsHostileForParam(UCharacterInstanceComponent* character)
{
	if (character->IsA<UPlayerCharacterInstanceComponent>())
	{
		return npcParty->bIsHostile;
	}
	else
	{
		ADSNPCParty* oppPtr = Cast<ADSNPCParty>(character->GetParty());

		return npcParty->bIsHostile ^ oppPtr->bIsHostile;
	}
}

ADSParty* UNPCCharacterInstanceComponent::GetParty()
{
	return npcParty;
}
