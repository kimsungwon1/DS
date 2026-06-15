


#include "Party.h"
#include "Components/BillboardComponent.h"

AParty::AParty()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    VisualHelper = CreateDefaultSubobject<UBillboardComponent>(TEXT("VisualHelper"));
    VisualHelper->SetupAttachment(RootComponent); // 루트에 부착

    VisualHelper->bHiddenInGame = true;
}
