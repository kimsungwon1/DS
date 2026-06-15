// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DSStats.h"
#include "CharacterClass.generated.h"
/**
 * 
 */
UCLASS(abstract)
class DS_API UCharacterClass : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EProfession classType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FCharacterStat neededStats;
};
