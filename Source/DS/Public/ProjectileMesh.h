

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "ProjectileMesh.generated.h"

UCLASS()
class DS_API AProjectileMesh : public AProjectileBase
{
	GENERATED_BODY()

public:
	AProjectileMesh();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	TObjectPtr<UStaticMeshComponent> MeshComp;
};
