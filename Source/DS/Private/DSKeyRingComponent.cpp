

#include "DSKeyRingComponent.h"

UDSKeyRingComponent::UDSKeyRingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDSKeyRingComponent::AddKey(FName KeyID, int32 Charges)
{
	for (FKeyRingEntry& Entry : KeyRing)
	{
		if (Entry.KeyID == KeyID)
		{
			Entry.Charges += Charges;
			return;
		}
	}

	FKeyRingEntry NewEntry;
	NewEntry.KeyID = KeyID;
	NewEntry.Charges = Charges;
	KeyRing.Add(NewEntry);
}

bool UDSKeyRingComponent::HasKey(FName KeyID) const
{
	for (const FKeyRingEntry& Entry : KeyRing)
	{
		if (Entry.KeyID == KeyID)
		{
			return Entry.Charges > 0;
		}
	}
	return false;
}

bool UDSKeyRingComponent::ConsumeKey(FName KeyID)
{
	for (int32 i = 0; i < KeyRing.Num(); ++i)
	{
		if (KeyRing[i].KeyID == KeyID)
		{
			KeyRing[i].Charges -= 1;
			if (KeyRing[i].Charges <= 0)
			{
				KeyRing.RemoveAt(i);
			}
			return true;
		}
	}
	return false;
}
