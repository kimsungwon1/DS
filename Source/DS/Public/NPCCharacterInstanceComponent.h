// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterInstanceComponent.h"
#include "NPCCharacterInstanceComponent.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ENPCBattleAction : uint8
{
	Attack UMETA(DisplayName = "Attack"),
	Spell UMETA(DisplayName = "Spell"),
	Run UMETA(DisplayName = "Run"),
};

class UDSNPCCharacterData;
class USpell;
class ADSNPCParty;

USTRUCT(Blueprintable, BlueprintType)
struct FActionPriority
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ToAttackNormal = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ToSpell = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ToRun = 0;
};

UCLASS(ClassGroup = (RPG), BlueprintType, meta = (BlueprintSpawnableComponent))
class DS_API UNPCCharacterInstanceComponent : public UCharacterInstanceComponent
{
	GENERATED_BODY()

public:
	UNPCCharacterInstanceComponent();

	void BeginPlay() override;

	ADSNPCParty* GetNpcParty() const { return npcParty; }

	UFUNCTION(BlueprintCallable)
	void JoinToBattle(AActor* aactor);

	virtual void DecideAction(ADSGameMode* mode) override;

	virtual void DecideAttack(ADSGameMode* mode) override;

	virtual void ReceiveTurn_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void SwitchTurn();

	bool IsGotTurn() override;

	bool IsHostileForParam(UCharacterInstanceComponent* character) override;

	class ADSParty* GetParty() override;
protected:
	// void SpawnByBlueprint();

	virtual void InitializeCharacter_Implementation();

protected:
	UPROPERTY(EditAnywhere)
	TArray<TSoftClassPtr<USpell>> ableSpellsCandidates;

	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<USpell>> ableSpells;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TObjectPtr<ADSNPCParty> npcParty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FActionPriority actionPriority;

	UPROPERTY(BlueprintReadWrite)
	ENPCBattleAction reservedAction;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UDSNPCCharacterData> baseData;

	UPROPERTY(BlueprintReadOnly)
	bool bIsGotTurn = false;
};
