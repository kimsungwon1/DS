// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterInstanceComponent.h"
#include "PlayerCharacterInstanceComponent.generated.h"

class APlayerPartyMover;
class ADSPlayerController;
class ADSPlayerParty;
class UDSPlayerCharaData;
class UCharacterClass;
/**
 * 
 */

UENUM(BlueprintType)
enum class EPCBattleAction : uint8
{
	Attack UMETA(DisplayName = "Attack"),
	Spell UMETA(DisplayName = "Spell"),
	Guard UMETA(DisplayName = "Guard"),
	Protect UMETA(DisplayName = "Protect"),
	Use UMETA(DisplayName = "Use"),
	Equip UMETA(DisplayName = "Equip"),
	Hide UMETA(DisplayName = "Hide"),
};

UCLASS(ClassGroup = (RPG), BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class DS_API UPlayerCharacterInstanceComponent : public UCharacterInstanceComponent
{
	GENERATED_BODY()
public:
	UPlayerCharacterInstanceComponent();

	void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void SetCharacter(int32 index);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	APlayerPartyMover* GetPlayerPartyMover();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const APlayerPartyMover* GetPlayerPartyMover_const() const { return partyMover; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ADSPlayerController* GetDSController();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetPartyIndex();

	virtual FVector GetCharacterLocation() const override;

	virtual void DecideAction(ADSGameMode* md) override;

	virtual void DecideAttack(ADSGameMode* md) override;

	class ADSParty* GetParty() override;
protected:
	virtual void InitializeCharacter_Implementation() override;
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCharacterClass> profession;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Portrait")
	UTexture2D* PortraitTexture;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSoftClassPtr<class AWeapon> reservedBaseWeapon;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class AWeapon> baseWeapon;

	UPROPERTY(BlueprintReadOnly)
	int pcIndex;

private:
	APlayerPartyMover* partyMover;

};
