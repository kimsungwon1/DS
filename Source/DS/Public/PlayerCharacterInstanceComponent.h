// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterInstanceComponent.h"
#include "DSVoiceSet.h"
#include "SaveGameData.h"
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

	virtual bool IsHostileForParam(UCharacterInstanceComponent* character) override;

	UFUNCTION(BlueprintCallable)
	void GainExp(int32 Amount);

	UFUNCTION(BlueprintCallable)
	void LevelUp();

	// 캐릭터 생성/GatherParty에서 저장한 FPlayerCharacterSaveData를 이 컴포넌트(실제 게임플레이 오브젝트)에 그대로 반영
	UFUNCTION(BlueprintCallable)
	void InitializeFromSaveData(const FPlayerCharacterSaveData& SaveData);

	// InitializeFromSaveData의 역방향 - 지금 이 컴포넌트(플레이 중 갱신된 상태)를 세이브 가능한 형태로 뽑아냄
	UFUNCTION(BlueprintCallable)
	FPlayerCharacterSaveData ToSaveData() const;
protected:
	virtual void InitializeCharacter_Implementation() override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	FSkillStat SkillStat;

	UPROPERTY(BlueprintReadOnly, Category = "Growth")
	FGrowthPoints UnallocatedPoints;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCharacterClass> profession;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	ERace Race = ERace::HUMAN;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Portrait")
	UTexture2D* PortraitTexture;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class AWeapon> reservedBaseWeapon;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class AWeapon> baseWeapon;

	UPROPERTY(BlueprintReadOnly)
	int pcIndex;

	// 이 캐릭터의 음성 세트. 다른 캐릭터와 공유 가능
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voice")
	TObjectPtr<UDSVoiceSet> VoiceSet;

private:
	APlayerPartyMover* partyMover;

	// 현재 Lvl/KnownSpells/Piety/Int/Skill 기준으로 5영역 스펠 포인트 최댓값을 처음부터 다시 계산
	FSpellPoints ComputeMaxSpellPoints() const;
};
