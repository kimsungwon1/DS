// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DSStats.h"
#include "CharacterClass.generated.h"
/**
 * 
 */
UCLASS(BlueprintType)
class DS_API UCharacterClass : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EProfession classType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FCharacterStat neededStats;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FSkillStat StartingSkillStat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int ExpBase = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Profession_HpMul = 3;
};
