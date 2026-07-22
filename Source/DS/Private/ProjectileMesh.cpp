

#include "ProjectileMesh.h"
#include "Components/StaticMeshComponent.h"

AProjectileMesh::AProjectileMesh()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
