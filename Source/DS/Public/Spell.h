

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "ActionCastType.h"
#include "DSBlueprintLibrary.h"
#include "DSTarget.h"
#include "DSSpellData.h"
#include "Spell.generated.h"

class UCharacterInstanceComponent;
class AEffectBase;
class AProjectileBase;

UENUM(BlueprintType)
enum class ESpellResult : uint8
{
	Success UMETA(DisplayName = "Success"),
	Fizzle UMETA(DisplayName = "Fizzle"),
	Fizzle_SparkToParty UMETA(DisplayName = "SparkToParty"),
};

// 자식은 Targeter중 하나를 '무조건' 상속한다.
UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DS_API USpell : public UObject
{
	GENERATED_BODY()

public:
	USpell();

	UFUNCTION(BlueprintCallable)
	void Initialize(UCharacterInstanceComponent* InCaster, int32 InLevel = 1);

	UFUNCTION()
	ESpellResult CheckResult();

	UFUNCTION(BlueprintCallable)
	void CastSpell();

	// 현재 타겟이 유효한지(null 아니고 안 죽음) 검사만 함 — 여기서 타겟을 바꾸지 않음.
	// 무효 판정 시 실제 재타겟팅은 호출부(USpellCast::StartAction_Implementation)에서 처리.
	// 파티 전체 대상 스펠(예: Heal의 bTargetParty)은 개별 타겟 생사와 무관하게 항상 유효.
	UFUNCTION(BlueprintCallable)
	virtual bool IsTargetValid() const { return true; }

	// SpellData->spellStat.targetType 기준으로 이 스펠이 적을 원하는지(true) 아군을 원하는지(false)
	// USpellCast 등 USpell 외부에서도 호출해야 하므로 public
	UFUNCTION(BlueprintCallable)
	bool WantsHostileTarget() const;

	// 전장에 있는 모든 파티를 훑어 (적/아군 중 원하는 쪽의) 살아있는 캐릭터 하나를 찾음. 없으면 nullptr.
	// USpellCast 등 USpell 외부에서도 호출해야 하므로 public
	UFUNCTION(BlueprintCallable)
	virtual UCharacterInstanceComponent* FindReplacementTarget();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UDSSpellData> SpellData;

protected:
	void SpawnEffectAt(TSoftClassPtr<AEffectBase> EffectClass, FVector Location);
	void SpawnProjectileToward(TSoftClassPtr<AProjectileBase> ProjectileClass, FVector From, FVector To);

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Cast_Success();
	virtual void Cast_Success_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Fizzle();
	virtual void Fizzle_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Fizzle_SparkToParty();
	virtual void Fizzle_SparkToParty_Implementation() {}

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCharacterInstanceComponent> Caster;

	UPROPERTY(BlueprintReadOnly)
	int32 CastLevel = 1;
};
