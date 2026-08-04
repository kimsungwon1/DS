


#include "DSGameMode.h"
#include "DSPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "NPCCharacterInstanceComponent.h"
#include "PlayerCharacterInstanceComponent.h"
#include "PlayerPartyMover.h"
#include "DSMainWidget.h"
#include "DSNPCParty.h"
#include "DSPlayerParty.h"
#include "Attack.h"
#include "DSSaveGameSubsystem.h"
#include "PlayerPartyManagerComponent.h"
#include "TimerManager.h"

ADSGameMode::ADSGameMode()
	: AGameModeBase(), pcMemberColors{FColor::Red, FColor::Green, FColor::Purple, FColor::Blue, FColor::Orange, FColor::Yellow}
{

}

void ADSGameMode::BeginPlay()
{
	Super::BeginPlay();

	UDSSaveGameSubsystem* SaveSubsystem = GetGameInstance() ? GetGameInstance()->GetSubsystem<UDSSaveGameSubsystem>() : nullptr;

	if (SaveSubsystem && SaveSubsystem->IsPendingNewGame())
	{
		EnterHomeBase();
	}
	else
	{
		// TODO: 이어하기 - 저장된 파티/마지막 위치를 복원해서 스폰 (세션 세이브 시스템 아직 없음)
	}
}

void ADSGameMode::StartBattle_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Mode starts Battle"));

	bIsBattle = true;

	DecideCharactersAction();

	OnBattleChanged.Broadcast(bIsBattle);

	auto* dsp = GetDSPlayerController();

	dsp->BeginBattle();
}

void ADSGameMode::EndBattle_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Mode ends Battle"));

	if (bIsCycle)
	{
		EndCycle();
	}

	bIsBattle = false;

	OnBattleChanged.Broadcast(bIsBattle);

	auto* dsp = GetDSPlayerController();

	dsp->EndBattle();
}

void ADSGameMode::StartCycle_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Mode starts Cycle"));

	if (bIsCycle)
	{
		return;
	}

	bIsCycle = true;

	OnCycleStarted.Broadcast(bIsCycle);

	ADSParty* pp = GetDSPlayerController()->GetPlayerPartyMover()->GetParty();
	for (UCharacterInstanceComponent* pc : pp->GetCharacters())
	{
		if (pc == nullptr)
		{
			continue;
		}
		pc->CalculateSpeed();

		if (pc->GetCurrentAction()->IsTargetLost())
		{
			arrCharactersUnable.Add(pc);
		}
		else
		{
			arrCharactersInTurn.Add(pc);
		}
	}

	for (ADSNPCParty* np : arrPartiesInBattle)
	{
		for (UNPCCharacterInstanceComponent* nc : np->characters)
		{
			nc->CalculateSpeed();
			arrCharactersInTurn.Add(nc);
		}
	}

	// GetDSPlayerController()->GetMainWidget()->ActionSelectDone();
	GetDSPlayerController()->DisableTargetSelection();

	SortCharacters();

	SwitchTurn();
}

void ADSGameMode::EndCycle_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Mode ends Cycle"));

	bIsCycle = false;

	OnCycleStarted.Broadcast(bIsCycle);

	currentTurnGetterIndex = NoCycleNumber;
	arrCharactersInTurn.Empty();

	DecideCharactersAction();
}

void ADSGameMode::EnterHomeBase_Implementation(bool bAfterDefeat)
{
	// 뭔가에 의해 두 번 불려도(문 상호작용 중복 등) 홈 위젯이 겹쳐서 또 뜨지 않도록 방지
	if (bIsInHomeBase)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Mode enters Home Base"));

	bIsInHomeBase = true;

	OnHomeBaseChanged.Broadcast(bIsInHomeBase);

	auto* dsp = GetDSPlayerController();

	dsp->EnterHomeBase(bAfterDefeat);
}

void ADSGameMode::ExitHomeBase_Implementation()
{
	if (!bIsInHomeBase)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Mode exits Home Base"));

	// TODO: 여기서 플레이어 파티를 마을 밖 시작 지점(스폰 포인트)으로 옮기는 로직 필요 (나중에)

	bIsInHomeBase = false;

	// 홈베이스에서 파티 구성(Add/Remove)이 바뀌었을 수 있으니 하단 UI 얼굴들 다시 갱신
	if (UPlayerPartyManagerComponent* Manager = GetPartyManager())
	{
		Manager->RefreshPartyUI();
	}

	OnHomeBaseChanged.Broadcast(bIsInHomeBase);

	auto* dsp = GetDSPlayerController();

	dsp->ExitHomeBase();
}

float ADSGameMode::GetNpcDistanceToTarget(const UNPCCharacterInstanceComponent* Npc)
{
	if (!Npc)
	{
		return TNumericLimits<float>::Max();
	}

	const UAttack* CurrentAttack = Cast<UAttack>(Npc->GetCurrentAction());
	const UCharacterInstanceComponent* AttackTarget = CurrentAttack ? CurrentAttack->GetTarget() : nullptr;

	const AActor* NpcActor = Npc->GetOwner();
	const AActor* TargetActor = AttackTarget ? AttackTarget->GetOwner() : nullptr;

	if (!NpcActor || !TargetActor)
	{
		return TNumericLimits<float>::Max();
	}

	return FVector::Dist(NpcActor->GetActorLocation(), TargetActor->GetActorLocation());
}

void ADSGameMode::SortCharacters()
{
	// 플레이어 캐릭터는 기존대로 속도(FinalSpeed) 내림차순.
	// NPC끼리는 속도 대신 공격 타겟까지의 거리가 가까운 순으로 정렬 (타겟에 가까운 NPC가 먼저 행동)
	// TODO: 지금은 근접(UAttack) 기준 거리 정렬만 있음 - 원거리/마법(USpellCast) NPC는 거리 계산이 안 돼서
	// 그냥 맨 뒤로 밀림. 나중엔 근접 NPC는 거리로, 원거리/마법 NPC는 속도로 정렬하게 나눠야 함
	arrCharactersInTurn.Sort([](const UCharacterInstanceComponent& A, const UCharacterInstanceComponent& B)
	{
		const UNPCCharacterInstanceComponent* NpcA = Cast<UNPCCharacterInstanceComponent>(&A);
		const UNPCCharacterInstanceComponent* NpcB = Cast<UNPCCharacterInstanceComponent>(&B);

		if (NpcA && NpcB)
		{
			return GetNpcDistanceToTarget(NpcA) < GetNpcDistanceToTarget(NpcB);
		}

		return A.FinalSpeed > B.FinalSpeed;
	});
}

void ADSGameMode::SwitchTurn_Implementation()
{
	if (IsBattleEnded())
	{
		EndBattle();
		return;
	}

	currentTurnGetterIndex++;

	UE_LOG(LogTemp, Log, TEXT("Mode switchs turn - current turn getter index is %d"), currentTurnGetterIndex);

	focuseeStack.Empty();

	if (currentTurnGetterIndex >= arrCharactersInTurn.Num())
	{
		EndCycle();
	}
	else
	{
		UCharacterInstanceComponent* TurnGetter = GetCurrentTurnGetter();
		PushFocus(TurnGetter);

		if (UNPCCharacterInstanceComponent* Npc = Cast<UNPCCharacterInstanceComponent>(TurnGetter))
		{
			if (AActor* NpcActor = Npc->GetOwner())
			{
				if (ADSPlayerController* PC = GetDSPlayerController())
				{
					PC->FocusOnActor(NpcActor, 1.5f, 2.f);
				}
			}
		}

		TurnGetter->ReceiveTurn();
	}
}

void ADSGameMode::PlayerDefeated_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Mode: player party is defeated"));

	if (ADSPlayerController* dsp = GetDSPlayerController())
	{
		dsp->PlayerDefeated();
	}
}

void ADSGameMode::ReturnToHomeBaseAfterDefeat()
{
	// 저승사자 화면이 잠깐이라도 혼자 보이도록, 홈 복귀(파티 정리 + EnterHomeBase)는 살짝 늦춰서 실행
	GetWorld()->GetTimerManager().SetTimer(
		DefeatToHomeBaseTimerHandle,
		this,
		&ADSGameMode::DoReturnToHomeBaseAfterDefeat,
		DefeatToHomeBaseDelay,
		false
	);
}

void ADSGameMode::DoReturnToHomeBaseAfterDefeat()
{
	if (UPlayerPartyManagerComponent* Manager = GetPartyManager())
	{
		if (ADSPlayerParty* Party = GetPartyObject())
		{
			for (int32 i = 0; i < Party->characters.Num(); ++i)
			{
				if (Party->characters[i])
				{
					Manager->RemoveCharacterFromParty(i);
				}
			}
		}
	}

	EnterHomeBase(true);
}

void ADSGameMode::PushFocus(UObject* focusee)
{
	if (!focuseeStack.Contains(focusee)) 
	{
		UE_LOG(LogTemp, Log, TEXT("Mode pushes focus for %s"), *focusee->GetFName().ToString());

		focuseeStack.Push(focusee);
	}
}

void ADSGameMode::PopFocus(UObject* focusee)
{
	UE_LOG(LogTemp, Log, TEXT("Mode pops focus for %s"), *focusee->GetFName().ToString());

	if (!focuseeStack.IsEmpty()  && focuseeStack.Top() == focusee)
	{
		focuseeStack.Pop();

		if (focuseeStack.IsEmpty())
		{
			SwitchTurn();
		}
		else if (focuseeStack.Top() == GetCurrentTurnGetter())
		{
			GetCurrentTurnGetter()->ForcedToAct();// ForcedToAct();
		}
	}
}

void ADSGameMode::SwitchFocus(UObject* focusee, UObject* newFocusee)
{
	UE_LOG(LogTemp, Log, TEXT("Mode pops focus for %s, %s"), *focuseeStack.Top()->GetFName().ToString(), *newFocusee->GetFName().ToString());

	if (focuseeStack.Top() == focusee)
	{
		focuseeStack.Pop();

		focuseeStack.Push(newFocusee);
	}
}

void ADSGameMode::PushForTurn(UCharacterInstanceComponent* character)
{
	if (arrCharactersUnable.Contains(character))
	{
		arrCharactersUnable.Remove(character);
		arrCharactersInTurn.Add(character);
	}
}

void ADSGameMode::JoinNpcToBattle_Implementation(UNPCCharacterInstanceComponent* npc, AActor* actor)
{
	auto* party = npc->GetNpcParty();

	if (arrPartiesInBattle.Contains(party))
	{
		return;
	}

	arrPartiesInBattle.Add(party);

	for (UNPCCharacterInstanceComponent* partyMember : party->characters)
	{
		partyMember->SetIsBattle(true);
	}
}

void ADSGameMode::RemoveNpcFromBattle_Implementation(UNPCCharacterInstanceComponent* npc)
{
	auto* party = npc->GetNpcParty();

	bool isAllForgottable = true;

	for (UNPCCharacterInstanceComponent* otherNpc : party->characters)
	{
		if (!otherNpc->Forgottable())
		{
			isAllForgottable = false;
		}
	}

	if (isAllForgottable)
	{
		arrPartiesInBattle.Remove(party);

		for (UNPCCharacterInstanceComponent* otherNpc : party->characters)
		{
			otherNpc->SetIsBattle(false);
		}
	}
}

UCharacterInstanceComponent* ADSGameMode::GetCurrentTurnGetter()
{
	if (currentTurnGetterIndex == NoCycleNumber)
	{
		return nullptr;
	}

	return arrCharactersInTurn[currentTurnGetterIndex];
}

bool ADSGameMode::IsThisTop(UObject* ob) const
{
	if (focuseeStack.IsEmpty())
	{
		return false;
	}
	else if (focuseeStack.Top() == ob)
	{
		return true;
	}
	return false;
}

ADSPlayerController* ADSGameMode::GetDSPlayerController()
{
	ADSPlayerController* dsp = dsPlayerController;

	if (dsp == nullptr)
	{
		dsp = dsPlayerController = Cast<ADSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	}

	return dsp;
}

APlayerPartyMover* ADSGameMode::GetPartyMover()
{
	if (partyMover == nullptr)
	{
		partyMover = GetDSPlayerController()->GetPlayerPartyMover();
	}

	return partyMover;
}

ADSPlayerParty* ADSGameMode::GetPartyObject()
{
	if (cachedPartyObject == nullptr)
	{
		if (APlayerPartyMover* Mover = GetPartyMover())
		{
			cachedPartyObject = Cast<ADSPlayerParty>(Mover->GetParty());
		}
	}

	return cachedPartyObject;
}

UPlayerPartyManagerComponent* ADSGameMode::GetPartyManager()
{
	if (cachedPartyManager == nullptr)
	{
		if (APlayerPartyMover* Mover = GetPartyMover())
		{
			cachedPartyManager = Mover->FindComponentByClass<UPlayerPartyManagerComponent>();
		}
	}

	return cachedPartyManager;
}

float ADSGameMode::GetCycleProgress() const
{
	if (currentTurnGetterIndex == NoCycleNumber)
	{
		return 1.0f;
	}

	return float(currentTurnGetterIndex) / float(arrCharactersInTurn.Num());
}

void ADSGameMode::DecideCharactersAction()
{
	ADSParty* pp = GetPartyMover() ->GetParty();
	for (UCharacterInstanceComponent* pc : pp->GetCharacters())
	{
		if (pc == nullptr || pc->IsDead())
		{
			continue;
		}
		pc->DecideAction(this);
	}

	for (ADSNPCParty* np : arrPartiesInBattle)
	{
		for (UNPCCharacterInstanceComponent* nc : np->characters)
		{
			if (!nc->IsDead()) 
			{
				nc->DecideAction(this);
			}
		}
	}
}

bool ADSGameMode::IsBattleEnded()
{
	bool bPlayersAllDied = true;
	bool bEnemiesAllDied = true;

	for (auto* pc : GetPartyMover()->GetParty()->GetCharacters())
	{
		if (pc && !pc->IsDead())
		{
			bPlayersAllDied = false;
		}
	}
	if (bPlayersAllDied)
	{
		PlayerDefeated();
		return true;
	}

	for (auto* npcParty : arrPartiesInBattle)
	{
		if (npcParty->bIsHostile)
		{
			for (UNPCCharacterInstanceComponent* npc : npcParty->characters)
			{
				if (!npc->IsDead())
				{
					bEnemiesAllDied = false;
				}
			}
		}
	}
	if (bEnemiesAllDied)
	{
		return true;
	}

	return false;
}
