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
#include "CharacterClass.h"
#include "CharacterClassRegistry.h"

UPlayerCharacterInstanceComponent::UPlayerCharacterInstanceComponent()
{
	reservedBaseWeapon = AHands::StaticClass();
}

void UPlayerCharacterInstanceComponent::BeginPlay()
{
	Super::BeginPlay();

	actor = Cast<ABaseCharacter>(GetOwner());

	if (profession == nullptr)
	{
		auto* ccr = GetWorld()->GetGameInstance()->GetSubsystem<UCharacterClassRegistry>();
		TArray<UCharacterClass*> ccArr = ccr->GetAllCharacterClasses();
		if (!ccArr.IsEmpty())
		{
			profession = ccArr[0];
		}
	}

	FinalFaceStat = BaseFaceStat;
	FinalFaceStat.ExpToNextLevel = FMath::RoundToInt(profession->ExpBase * FMath::Pow((float)FinalFaceStat.Level, 1.5f));

	if (reservedWeapon)
	{
		UClass* loadedClass = reservedWeapon.LoadSynchronous();

		holdingWeapon = GetWorld()->SpawnActor<AWeapon>(loadedClass, actor->ActorToWorld());
	}

	InitializeCharacter();

	if (reservedBaseWeapon)
	{
		baseWeapon = GetWorld()->SpawnActor<AWeapon>(reservedBaseWeapon, actor->GetTransform());
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

bool UPlayerCharacterInstanceComponent::IsHostileForParam(UCharacterInstanceComponent* character)
{
	return character->IsHostileForParam(this);
}

void UPlayerCharacterInstanceComponent::GainExp(int32 Amount)
{
	FinalFaceStat.CurrentExp += Amount;

	while (FinalFaceStat.CurrentExp >= FinalFaceStat.ExpToNextLevel)
	{
		FinalFaceStat.CurrentExp -= FinalFaceStat.ExpToNextLevel;
		LevelUp();
	}
}

void UPlayerCharacterInstanceComponent::LevelUp()
{
	FinalFaceStat.Level++;
	FinalFaceStat.ExpToNextLevel = FMath::RoundToInt(profession->ExpBase * FMath::Pow((float)FinalFaceStat.Level, 1.5f));

	UnallocatedPoints.StatPoints += 4;
	UnallocatedPoints.SkillPoints += 15;

	const int32 HpMul = profession ? profession->Profession_HpMul : 3;
	const int32 HpGain = FMath::RoundToInt((4.f + 0.8f * HpMul) * (3.f / 8.f + CharStat.Vitality / 36.f));
	FinalFaceStat.MaxHP += HpGain;
	FinalFaceStat.Health += HpGain;

	// 증가분을 누적하는 대신, 매 레벨업마다 현재 상태 기준으로 처음부터 다시 계산
	// (증가분 방식은 Lvl/KnownSpells가 곱셈항이라 시간이 지날수록 원래 공식과 어긋남)
	const FSpellPoints OldMaxSP = FinalFaceStat.MaxSP;
	const FSpellPoints NewMaxSP = ComputeMaxSpellPoints();

	FinalFaceStat.MaxSP = NewMaxSP;
	FinalFaceStat.CurrentSP.Aether += NewMaxSP.Aether - OldMaxSP.Aether;
	FinalFaceStat.CurrentSP.Water  += NewMaxSP.Water  - OldMaxSP.Water;
	FinalFaceStat.CurrentSP.Air    += NewMaxSP.Air    - OldMaxSP.Air;
	FinalFaceStat.CurrentSP.Earth  += NewMaxSP.Earth  - OldMaxSP.Earth;
	FinalFaceStat.CurrentSP.Sun    += NewMaxSP.Sun    - OldMaxSP.Sun;
}

FSpellPoints UPlayerCharacterInstanceComponent::ComputeMaxSpellPoints() const
{
	const bool bCanThaumaturgy = SkillStat.Thaumaturgy != -1;
	const bool bCanTheology = SkillStat.Theology != -1;

	FSpellPoints Result;
	if (!bCanThaumaturgy && !bCanTheology)
	{
		return Result;
	}

	// 3번째/4번째 스펠북(Artifacts/Kirijutsu)은 마법이랑 무관해서 계산에서 제외. -1(사용 불가)인 쪽은 0 취급
	const float A = bCanThaumaturgy ? (float)SkillStat.Thaumaturgy : 0.f;
	const float B = bCanTheology ? (float)SkillStat.Theology : 0.f;
	const float Highest = FMath::Max(A, B);
	const float Second = FMath::Min(A, B);
	const float WeightedSkill = FMath::Min(125.f, Highest + Second * 0.5f);
	const float BaseTerm = 1.f + CharStat.Piety / 9.f + CharStat.Intelligence / 12.f + WeightedSkill * 0.05f;

	const int32 Level = FinalFaceStat.Level;

	// 아는 스펠이 아예 없는 영역은 기본 풀도 안 주고 그냥 0점 (DSCharacterStatsWidget::RefreshDerivedStats와 동일 규칙)
	auto ComputeRealmSP = [Level, BaseTerm](int32 KnownCount) -> int32
	{
		return KnownCount > 0 ? FMath::RoundToInt((Level + 1 + KnownCount) * BaseTerm) : 0;
	};

	Result.Aether = ComputeRealmSP(KnownSpells.KnownSpells_Aether.Num());
	Result.Water  = ComputeRealmSP(KnownSpells.KnownSpells_Water.Num());
	Result.Air    = ComputeRealmSP(KnownSpells.KnownSpells_Air.Num());
	Result.Earth  = ComputeRealmSP(KnownSpells.KnownSpells_Earth.Num());
	Result.Sun    = ComputeRealmSP(KnownSpells.KnownSpells_Sun.Num());
	return Result;
}

void UPlayerCharacterInstanceComponent::InitializeFromSaveData(const FPlayerCharacterSaveData& SaveData)
{
	characterName = SaveData.characterName;
	BaseFaceStat = SaveData.characterData.BaseFaceStat;
	FinalFaceStat = SaveData.characterData.FinalFaceStat;
	CharStat = SaveData.characterData.CharStat;
	SkillStat = SaveData.SkillStat;
	KnownSpells = SaveData.KnownSpells;
	PortraitTexture = SaveData.Portrait.LoadSynchronous();
	Race = SaveData.Race;

	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UCharacterClassRegistry* CCR = GI->GetSubsystem<UCharacterClassRegistry>())
			{
				profession = CCR->GetCharacterClassByProfession(SaveData.Profession);
			}
		}
	}
}

FPlayerCharacterSaveData UPlayerCharacterInstanceComponent::ToSaveData() const
{
	FPlayerCharacterSaveData SaveData;

	SaveData.characterName = characterName;
	SaveData.characterData.BaseFaceStat = BaseFaceStat;
	SaveData.characterData.FinalFaceStat = FinalFaceStat;
	SaveData.characterData.CharStat = CharStat;
	SaveData.SkillStat = SkillStat;
	SaveData.KnownSpells = KnownSpells;
	SaveData.Portrait = PortraitTexture;
	SaveData.Race = Race;
	SaveData.Profession = profession ? profession->classType : EProfession();

	return SaveData;
}
