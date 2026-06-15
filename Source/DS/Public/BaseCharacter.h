// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class ADSGameMode;

UCLASS(Abstract, BlueprintType, Blueprintable)
class DS_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	ADSGameMode* GetDSGameMode();

	UFUNCTION(BlueprintCallable)
	void StartBattle();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsForgottable();
	bool IsForgottable_Implementation() const { return false; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Die();
	void Die_Implementation() {}

	UFUNCTION(BlueprintCallable)
	virtual class ADSParty* GetParty() { return nullptr; }

protected:
	ADSGameMode* dsMode = nullptr;

public:
	// 일반 공격 몽타주
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* AttackMontage;

	// 마법 시전 몽타주
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* SpellMontage;
};
