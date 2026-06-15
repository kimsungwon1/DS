


#include "DSNPCParty.h"
#include "NPCCharacterInstanceComponent.h"

TArray<class UCharacterInstanceComponent*> ADSNPCParty::GetCharacters()
{
    TArray<class UCharacterInstanceComponent*> ret;

    for (UNPCCharacterInstanceComponent* nc : characters)
    {
        ret.Add(nc);
    }

    return ret;
}
