// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterInstanceComponent.h"
#include "DSSpellData.h"
#include "BaseCharacter.h"
#include "DSAction.h"
#include "SpellCast.h"
#include "Spell.h"
#include "Hands.h"
#include "DSGameMode.h"
#include "Targeter_DSCharacter.h"
#include "Targeter_PositionVector.h"
#include "ActionList.h"
#include "DSNPCParty.h"
#include "NPCCharacterInstanceComponent.h"

// Sets default values for this component's properties
UCharacterInstanceComponent::UCharacterInstanceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UCharacterInstanceComponent::BeginPlay()
{
	Super::BeginPlay();

	actor = Cast<ABaseCharacter>(GetOwner());

	FinalFaceStat = BaseFaceStat;

	if (reservedWeapon && actor)
	{
		UClass* loadedClass = reservedWeapon.LoadSynchronous();

		holdingWeapon = GetWorld()->SpawnActor<AWeapon>(loadedClass, actor->ActorToWorld());
	}
	else if (!reservedWeapon)
	{
		holdingWeapon = GetWorld()->SpawnActor<AHands>();
	}

	InitializeCharacter();
}

FVector UCharacterInstanceComponent::GetCharacterLocation() const
{
	if (actor)
	{
		return actor->GetActorLocation();
	}
	return FVector();
}

USkeletalMeshComponent* UCharacterInstanceComponent::GetMesh() const
{
	return actor->GetMesh();
}

bool UCharacterInstanceComponent::IsIdle() const
{
	return currentAction == nullptr;
}

ADSGameMode* UCharacterInstanceComponent::GetDSGameMode()
{
	if (mode == nullptr)
	{
		mode = GetCharacter()->GetDSGameMode();
	}
	return mode;
}

ABaseCharacter* UCharacterInstanceComponent::GetCharacter()
{
	if (actor == nullptr)
	{
		actor = Cast<ABaseCharacter>(GetOwner());
	}

	return actor;
}

bool UCharacterInstanceComponent::IsGotTurn()
{
	return this == GetDSGameMode()->GetCurrentTurnGetter();
}

bool UCharacterInstanceComponent::Forgottable()
{
	ABaseCharacter* character = GetCharacter();

	return character->IsForgottable();
}

void UCharacterInstanceComponent::DecideAction(ADSGameMode* md)
{
	DecideAttack(md);
}

void UCharacterInstanceComponent::DecideAttack(ADSGameMode* md)
{
}

void UCharacterInstanceComponent::CalculateSpeed()
{

}

AController* UCharacterInstanceComponent::GetController()
{
	return actor->GetController();
}

void UCharacterInstanceComponent::InitializeCharacter_Implementation()
{
	FinalFaceStat = BaseFaceStat;

	// 캐릭터 클래스, 종족에 따라 할수 있는 일들(ableActions)을 초기화
	ableActions.Push(NewObject<UAttack>(this));
	ableActions.Push(NewObject<USpellCast>(this));
	ableActions.Push(NewObject<UMove>(this));

	// 이름 초기화
	if (characterName.IsNone())
	{
		characterName = this->GetFName();
	}
}

// Called every frame
void UCharacterInstanceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsDead() && currentAction && currentAction->IsTargetLost() && currentAction->IsA<UAttack>())
	{
		DecideAttack(GetDSGameMode());
		GetDSGameMode()->PushForTurn(this);
	}
	// ...
}

void UCharacterInstanceComponent::ReceiveHeal_Implementation(int32 Amount)
{
	FinalFaceStat.Health = FMath::Min(FinalFaceStat.Health + Amount, FinalFaceStat.MaxHP);
}

void UCharacterInstanceComponent::ApplyDamage(int32 FinalDamage)
{
	FinalFaceStat.Health = FMath::Max(0, FinalFaceStat.Health - FinalDamage);
}

void UCharacterInstanceComponent::ReceiveDamage_Implementation(UCharacterInstanceComponent* attacker)
{
	GetDSGameMode()->PushFocus(this);
	ApplyDamage(CalculateDamage(attacker));
	GetDSGameMode()->PopFocus(this);
}

void UCharacterInstanceComponent::ReceiveSpellDamage_Implementation(UCharacterInstanceComponent* attacker, UDSSpellData* SpellData)
{
	GetDSGameMode()->PushFocus(this);
	ApplyDamage(CalculateSpellDamage(attacker, SpellData));
	GetDSGameMode()->PopFocus(this);
}

int UCharacterInstanceComponent::CalculateDamage_Implementation(UCharacterInstanceComponent* attacker)
{
	FWeaponStat weaponStat = attacker->GetHoldingWeapon()->GetStat();
	int finalDamage = FMath::RandRange(weaponStat.damageMinimum, weaponStat.damageMaximum) + weaponStat.damageEnhance;
	return finalDamage;
}

int UCharacterInstanceComponent::CalculateSpellDamage_Implementation(UCharacterInstanceComponent* attacker, UDSSpellData* SpellData)
{
	if (!SpellData) return 0;

	// MinPower~MaxPower 랜덤 기본값
	int32 BaseDamage = FMath::RandRange(
		FMath::RoundToInt(SpellData->MinPower),
		FMath::RoundToInt(SpellData->MaxPower)
	);

	// 시전자 Intelligence 보정 (STANDARD 기준 상대값)
	float AttackerBonus = 1.f + (attacker->GetCharStat().Intelligence - FCharacterStat::STANDARD) * 0.02f;

	// 피해자 원소 저항 적용
	int32 Resistance = 0;
	switch (SpellData->Element)
	{
		case ESpellElement::Aether: Resistance = FinalFaceStat.ResistAether; break;
		case ESpellElement::Water:  Resistance = FinalFaceStat.ResistWater;  break;
		case ESpellElement::Air:    Resistance = FinalFaceStat.ResistAir;    break;
		case ESpellElement::Earth:  Resistance = FinalFaceStat.ResistEarth;  break;
		case ESpellElement::Sun:    Resistance = FinalFaceStat.ResistSun;    break;
	}

	int32 FinalDamage = FMath::Max(0, FMath::RoundToInt(BaseDamage * AttackerBonus) - Resistance);
	return FinalDamage;
}

void UCharacterInstanceComponent::ReceiveTurn_Implementation()
{
	currentAction->ReadyAction();
	ForcedToAct();
}

void UCharacterInstanceComponent::ForcedToAct()
{
	if (IsDead())
	{
		actor->GetDSGameMode()->PopFocus(this);
		return;
	}
	if(currentAction)
	{
		currentAction->StartAction();
	}
}

//void UCharacterInstanceComponent::ForcedToAct()
//{
//	currentAction->ExecuteAction();
//}

void UCharacterInstanceComponent::ForcedToCease()
{
	if (currentAction->GetActionCount() == 0) 
	{
		currentAction->EndAction();
	}
}

void UCharacterInstanceComponent::ReserveTarget(int actionIndex)
{
	/*UDSAction* actionToReserve = ableActions[actionIndex];
	if (USpellCast* spellCast = Cast<USpellCast>(actionToReserve))
	{
		if (auto* targetForCharacter = Cast<ITargeter_DSCharacter>(spellCast->GetSpell()))
		{
			targetForCharacter->SetTarget_character()
		}
	}*/
}

