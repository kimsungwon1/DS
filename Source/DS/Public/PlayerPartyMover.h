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
	APlayerPartyMover();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void TurnAround(const FVector2D& value);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ADSPlayerController* GetDSController();

	/**
	 * 플레이어를 캡슐 슬롯에 할당합니다.
	 * @param Player  할당할 플레이어 컴포넌트
	 * @param CapsuleIndex  0=중앙, 1=앞, 2=오른쪽, 3=뒤, 4=왼쪽. -1이면 자동 선택
	 * @return 성공 여부 (슬롯이 꽉 찼으면 false)
	 */
	UFUNCTION(BlueprintCallable)
	bool AllocatePlayerCapsule(UPlayerCharacterInstanceComponent* Player, int32 CapsuleIndex = -1);

	/** 플레이어의 슬롯 할당을 해제합니다. */
	UFUNCTION(BlueprintCallable)
	void DeallocatePlayerCapsule(UPlayerCharacterInstanceComponent* Player);

	/** 플레이어가 서야 할 월드 위치를 반환합니다. */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetPlayerLocation(const UPlayerCharacterInstanceComponent* Player) const;

	/** 플레이어가 할당된 캡슐 인덱스를 반환합니다. 미할당이면 -1 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetPlayerCapsuleIndex(const UPlayerCharacterInstanceComponent* Player) const;

	ADSParty* GetParty();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> MyCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "RPG")
	TObjectPtr<UPlayerPartyManagerComponent> playerCharacterManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTimelineComponent> timelineComp;

	float TurnRate;
	float LookUpRate;

	/** 캐릭터가 위치하는 캡슐 5개: [0]=중앙, [1]=앞, [2]=오른쪽, [3]=뒤, [4]=왼쪽 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Capsule")
	TArray<TObjectPtr<UCapsuleComponent>> CapsulesForCharacters;

	/** 대각선 충돌 방지용 캡슐 4개 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Capsule")
	TArray<TObjectPtr<UCapsuleComponent>> CapsulesForBlocking;

private:
	static constexpr int32 NumCapsules = 5;
	static constexpr int32 PlayersPerCapsule = 3;

	/**
	 * 슬롯 배열: 크기 NumCapsules * PlayersPerCapsule (= 15)
	 * [CapsuleIndex * PlayersPerCapsule + SlotIndex] = 할당된 플레이어 (nullptr = 빈 슬롯)
	 */
	UPROPERTY()
	TArray<TObjectPtr<UPlayerCharacterInstanceComponent>> CapsuleSlots;

	/** 플레이어 → 플랫 슬롯 인덱스 역방향 조회 */
	TMap<TObjectPtr<UPlayerCharacterInstanceComponent>, int32> PlayerToSlot;

	/** 빈 슬롯이 있는 캡슐 중 가장 적게 찬 것의 인덱스를 반환. 없으면 -1 */
	int32 FindBestCapsule() const;

	TObjectPtr<ADSPlayerController> dsPlayerController;
};
