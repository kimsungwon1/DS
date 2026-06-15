


#include "SpellCast.h"
#include "Spell.h"

USpellCast::USpellCast()
{
	actionName = "Spell";
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
		heldSpell->Cast();
	}

	EndAction_Implementation();
}
