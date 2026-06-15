// Fill out your copyright notice in the Description page of Project Settings.


#include "Hands.h"

AHands::AHands()
{
	weaponStat.damageMinimum = 1;
	weaponStat.damageMaximum = 4;
	weaponStat.eAttackCastType = EActionCastType::Instant;
	weaponStat.eWeaponRangeType = EWeaponRangeType::Short;
}
