


#include "EffectBase.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"


// Sets default values
AEffectBase::AEffectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
}

// Called when the game starts or when spawned
void AEffectBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEffectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


