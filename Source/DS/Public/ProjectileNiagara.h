

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "ProjectileNiagara.generated.h"

class UNiagaraComponent;

UCLASS()
class DS_API AProjectileNiagara : public AProjectileBase
{
	GENERATED_BODY()

public:
	AProjectileNiagara();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	TObjectPtr<UNiagaraComponent> NiagaraComp;
};
