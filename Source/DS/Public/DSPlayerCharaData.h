// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DSCharacterBaseData.h"
#include "DSPlayerCharaData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class DS_API UDSPlayerCharaData : public UDSCharacterBaseData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Portrait")
	UTexture2D* PortraitTexture;
};
