// Fill out your copyright notice in the Description page of Project Settings.


#include "DSCharacterBaseData.h"

const int32 FCharacterStat::STANDARD = 8;

UDSCharacterBaseData::UDSCharacterBaseData()
{
	BaseFaceStat.Health = BaseFaceStat.MaxHP;
	BaseFaceStat.Health = BaseFaceStat.MaxSP;
}
