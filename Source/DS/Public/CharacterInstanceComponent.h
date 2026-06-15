// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DSStats.h"
#include "CharacterInstanceComponent.generated.h"

class UDSAction;
class AWeapon;
class ADSGameMode;

UENUM(BlueprintType)
enum class EAbleActionState : uint8
{
	Whole UMETA(DisplayName = "Whole", ToolTip = "Done Nothing, thus It can do full actions."),
	OneActionAble UMETA(DisplayName = "One Action Able", ToolTip = "Moved a little, thus It can do one action."),
	Expended UMETA(DisplayName = "Expended", ToolTip = "Moved a lot, thus It can do nothing more.")
};

UCLASS(abstract, ClassGroup=(RPG), BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent) )
class DS_API UCharacterInstanceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterInstanceComponent();

	UFUNCTION(BlueprintCallable)
	FGuid GetID() const { return characterID; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// BeginPlay에서 로드로 캐릭터 불러올 시 호출.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InitializeCharacter();
	virtual void InitializeCharacter_Implementation();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	virtual FVector GetCharacterLocation() const;

	const AWeapon* GetHoldingWeapon() const { return holdingWeapon; }

	UFUNCTION(BlueprintCallable)
	class USkeletalMeshComponent* GetMesh() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReceiveDamage(UCharacterInstanceComponent* attacker);
	virtual void ReceiveDamage_Implementation(UCharacterInstanceComponent* attacker);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int CalculateDamage(UCharacterInstanceComponent* attacker);
	int CalculateDamage_Implementation(UCharacterInstanceComponent* attacker);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReceiveTurn();
	virtual void ReceiveTurn_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BeTarget(int playerIndex, bool targetSet = true);
	void BeTarget_Implementation(int playerIndex, bool targetSet = true) {}

	UFUNCTION(BlueprintCallable)
	void ForcedToAct();

	// UFUNCTION(BlueprintCallable)
	// void ForcedToAct();

	UFUNCTION(BlueprintCallable)
	void ForcedToCease();

	// 플레이어더러 화면상의 목표를 선택하게 함.
	UFUNCTION(BlueprintCallable)
	void ReserveTarget(int actionIndex);

	UFUNCTION()
	const UDSAction* GetCurrentAction() const { return currentAction; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UDSAction* GetActionByIndex(int index) { return ableActions[index]; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AController* GetController();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsDead() const { return FinalFaceStat.Health <= 0; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsIdle() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ADSGameMode* GetDSGameMode();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ABaseCharacter* GetCharacter();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual bool IsGotTurn();

	void SetIsBattle(bool bBattle) { bInBattle = bBattle; }

	UFUNCTION(BlueprintCallable)
	bool Forgottable();

	void CalculateSpeed();

	virtual void DecideAction(ADSGameMode* md);

	virtual void DecideAttack(ADSGameMode* md);

	UFUNCTION(BlueprintCallable)
	virtual bool IsHostileForParam(UCharacterInstanceComponent* character) { return true; }

	virtual class ADSParty* GetParty() { return nullptr; }
public:
	UPROPERTY(BlueprintReadWrite)
	int FinalSpeed = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName characterName;

protected:
	UPROPERTY(BlueprintReadOnly)
	FGuid characterID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	FFaceStat BaseFaceStat;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stat")
	FFaceStat FinalFaceStat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	FCharacterStat CharStat;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	TArray<TObjectPtr<UDSAction>> ableActions;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	TObjectPtr<UDSAction> currentAction;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	TObjectPtr<UDSAction> pastAction;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSoftClassPtr<AWeapon> reservedWeapon;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AWeapon> holdingWeapon;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class ABaseCharacter> actor = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ADSGameMode> mode = nullptr;

	UPROPERTY(BlueprintReadWrite)
	EAbleActionState ableActionState;

	UPROPERTY(BlueprintReadWrite)
	float moveCount = 500;

	UPROPERTY(BlueprintReadOnly)
	float moveCountMax = 500;

	UPROPERTY(BlueprintReadOnly)
	bool bInBattle = false;
};
