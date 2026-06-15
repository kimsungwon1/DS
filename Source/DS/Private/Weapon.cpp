


#include "Weapon.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	weaponStat.eWeaponRangeType = EWeaponRangeType::Short;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AWeapon::GetDistanceToAttack() const
{
	return UDSBlueprintLibrary::GetAttackRange(weaponStat.eWeaponRangeType);
}


