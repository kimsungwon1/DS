

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "DSStats.h"
#include "DSBlueprintLibrary.h"
#include "Weapon.generated.h"

UCLASS(Abstract)
class DS_API AWeapon : public AItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	float GetDistanceToAttack() const;

	const FWeaponStat& GetStat() const { return weaponStat; }
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FWeaponStat weaponStat;
};
