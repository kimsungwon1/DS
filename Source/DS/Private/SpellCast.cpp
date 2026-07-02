
#include "SpellCast.h"
#include "Spell.h"
#include "DSSpellData.h"

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
	SpellLevel = FMath::Clamp(Level, 1, 6);
}

void USpellCast::Initialize_Implementation(UCharacterInstanceComponent* actorInstance)
{
	Super::Initialize_Implementation(actorInstance);
}

void USpellCast::StartAction_Implementation()
{
	Super::StartAction_Implementation();
	ExecuteAction_Implementation();
}

void USpellCast::ExecuteAction_Implementation()
{
	if (heldSpell)
	{
		heldSpell->Initialize(actor, SpellLevel);
		heldSpell->Cast();
	}

	EndAction_Implementation();
}
