// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DSAction.h"
#include "Targeter_PositionVector.h"
#include "Move.generated.h"

/**
 * 
 */
UCLASS()
class DS_API UMove : public UDSAction, public ITargeter_PositionVector
{
	GENERATED_BODY()
	
public:
	UMove();

public:
	void SetTarget_position_Implementation(FVector targetPosition) { destination = targetPosition; }

protected:
	UPROPERTY(BlueprintReadOnly)
	FVector destination;
};
