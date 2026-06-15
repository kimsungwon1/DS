

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurnSwitcher.h"
#include "ProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(Abstract)
class DS_API AProjectileBase : public AActor, public ITurnSwitcher
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ReceiveTurn_Implementation(ADSGameMode* mode, UObject* passer) {}

	virtual void TurnFinished_Implementation(ADSGameMode* mode, float fDelay) {}

	virtual void PassTurn_Implementation(ADSGameMode* mode, UObject* switcher) {}
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
};
