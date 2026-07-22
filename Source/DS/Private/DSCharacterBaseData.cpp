// Fill out your copyright notice in the Description page of Project Settings.


#include "DSCharacterBaseData.h"

const int32 FCharacterStat::STANDARD = 8;
const int32 FSkillStat::STANDARD = 0;

UDSCharacterBaseData::UDSCharacterBaseData()
{
	BaseFaceStat.Health = BaseFaceStat.MaxHP;
	BaseFaceStat.CurrentSP = BaseFaceStat.MaxSP;
}
