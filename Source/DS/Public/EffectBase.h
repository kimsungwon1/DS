

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurnSwitcher.h"
#include "EffectBase.generated.h"

class UNiagaraComponent;

UCLASS(Abstract)
class DS_API AEffectBase : public AActor, public ITurnSwitcher
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEffectBase();

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
	TObjectPtr<UNiagaraComponent> NiagaraComp;
	
};
