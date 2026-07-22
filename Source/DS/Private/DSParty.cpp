


#include "DSParty.h"
#include "CharacterInstanceComponent.h"

bool ADSParty::IsWipedOut() const
{
    bool bWiped = true;
    for (UCharacterInstanceComponent* c : GetCharacters())
    {
        if (!c->IsDead())
        {
            bWiped = false;
        }
    }

    return bWiped;
}
