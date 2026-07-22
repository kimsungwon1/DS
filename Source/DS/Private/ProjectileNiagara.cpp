

#include "ProjectileNiagara.h"
#include "NiagaraComponent.h"

AProjectileNiagara::AProjectileNiagara()
{
	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	NiagaraComp->SetupAttachment(RootComponent);
}
