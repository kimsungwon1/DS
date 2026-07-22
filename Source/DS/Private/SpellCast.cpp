
#include "SpellCast.h"
#include "Spell.h"
#include "DSSpellData.h"
#include "CharacterInstanceComponent.h"
#include "Targeter_DSCharacter.h"

USpellCast::USpellCast()
{
	actionName = "Spell";
}

void USpellCast::SetSpellFromData(UDSSpellData* Data, int32 Level)
{
	if (!Data || Data->SpellClass.IsNull()) return;
	UClass* Cls = Data->SpellClass.LoadSynchronous();
	if (!Cls) return;
	heldSpell = NewObject<USpell>(this, Cls);
	heldSpell->SpellData = Data;
	heldSpell->Initialize(actor, SpellLevel);

	SpellLevel = FMath::Clamp(Level, 1, 7);
}

void USpellCast::Initialize_Implementation(UCharacterInstanceComponent* actorInstance)
{
	Super::Initialize_Implementation(actorInstance);
}

void USpellCast::StartAction_Implementation()
{
	Super::StartAction_Implementation();

	// 캐릭터를 대상으로 하는 스펠(Targeter_DSCharacter 구현체)만 재타겟팅 대상.
	// 위치 지정 스펠(AreaDamage 등)은 이 인터페이스를 안 갖고 있으므로 아래 체크를 자연스럽게 건너뜀.
	bool bTargetOk = true;
	
	bTargetOk = heldSpell->IsTargetValid();
	if (!bTargetOk)
	{
		// 죽었거나 타겟이 없음 — 대체 타겟을 찾아 재할당
		if (UCharacterInstanceComponent* NewTarget = heldSpell->FindReplacementTarget())
		{
			// ITargeter_DSCharacter::Execute_SetTarget_character(heldSpell, NewTarget);
			bTargetOk = true;
		}
	}
	
	if (actionCount > 0 && heldSpell && bTargetOk)
	{
		GetWorld()->GetTimerManager().SetTimer(
			SpellCastTimerHandle,
			this,
			&USpellCast::ExecuteAction_Implementation,
			0.8f,
			false
		);
	}
	else
	{
		EndAction_Implementation();
	}
}

void USpellCast::ExecuteAction_Implementation()
{
	Super::ExecuteAction_Implementation();
	actionCount--;
	if (heldSpell)
	{
		heldSpell->Initialize(actor, SpellLevel);
		heldSpell->CastSpell();
	}
}
