// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGCharacterData.h"
#include "DSStats.h"
#include "DSCharacterBaseData.generated.h"

/**
 * 
 */


UCLASS(Abstract, BlueprintType, Blueprintable)
class DS_API UDSCharacterBaseData : public URPGCharacterData
{
	GENERATED_BODY()
	
public:
	UDSCharacterBaseData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	FFaceStat BaseFaceStat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	FCharacterStat CharStat;
};
