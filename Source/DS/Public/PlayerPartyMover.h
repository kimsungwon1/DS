// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Components/TimelineComponent.h"
#include "PlayerPartyMover.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UPlayerCharacterInstanceComponent;
class UPlayerPartyManagerComponent;
class ADSPlayerController;

UCLASS(BlueprintType, Blueprintable)
class DS_API APlayerPartyMover : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerPartyMover();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void TurnAround(const FVector2D& value);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ADSPlayerController* GetDSController();

	void InitializeCapsulesAndPlayers();

	UFUNCTION(BlueprintCallable)
	bool AllocatePlayerCapsule(UPlayerCharacterInstanceComponent* player, int capsuleIndex);

	UFUNCTION(BlueprintCallable)
	FVector GetPlayerLocation(const UPlayerCharacterInstanceComponent* player) const;

	class ADSParty* GetParty();
protected:
	// 카메라 관련
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* MyCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "RPG")
	UPlayerPartyManagerComponent* playerCharacterManager;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTimelineComponent* timelineComp;

	float TurnRate;
	float LookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Capsule")
	TArray<TObjectPtr<UCapsuleComponent>> capsulesForCharacters;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Capsule")
	TArray<TObjectPtr<UCapsuleComponent>> capsulesForJustBlocking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Capsule")
	TMap<TObjectPtr<UPlayerCharacterInstanceComponent>, int> capsuleIndexAllocatedToPlayers;

	UPROPERTY(BlueprintReadWrite, Category = "Capsule")
	TArray<int> capsuleAllocatedPlayersNum;

private:
	ADSPlayerController* dsPlayerController;
};
