


#include "DSPlayerParty.h"
#include "PlayerCharacterInstanceComponent.h"

ADSPlayerParty::ADSPlayerParty() : characters{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}
{
}

TArray<class UCharacterInstanceComponent*> ADSPlayerParty::GetCharacters()
{
    TArray<class UCharacterInstanceComponent*> ret;

    for (UPlayerCharacterInstanceComponent* pc : characters)
    {
        ret.Add(pc);
    }

    return ret;
}
