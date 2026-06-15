


#include "Spell.h"


// Sets default values for this component's properties
USpell::USpell()
{

}

ESpellResult USpell::CheckResult()
{

	return ESpellResult::Success;
}

void USpell::Cast()
{
	auto result = CheckResult();

	switch (result)
	{
	case ESpellResult::Success:
		Cast_Success();
		return;
	case ESpellResult::Fizzle:
		Fizzle();
		return;
	case ESpellResult::Fizzle_SparkToParty:
		Fizzle_SparkToParty();
		return;
	}
}