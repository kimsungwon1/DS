
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DSTarget.h"
#include "ActionCastType.h"
#include "DSSpellData.generated.h"

class AEffectBase;
class AProjectileBase;
class USpell;
class ASelector;

UENUM(BlueprintType)
enum class ESpellElement : uint8
{
	Aether UMETA(DisplayName = "Aether"),
	Water UMETA(DisplayName = "Water"),
	Air UMETA(DisplayName = "Air"),
	Earth UMETA(DisplayName = "Earth"),
	Sun UMETA(DisplayName = "Sun")//2편에선 Dark로 변경
};

UENUM(BlueprintType)
enum class ESpellBookType : uint8
{
	Thaumaturgy UMETA(DisplayName = "Thaumaturgy(Mage)"),
	Theology UMETA(DisplayName = "Theology(Priest)")
};

USTRUCT(BlueprintType, Blueprintable)
struct FSpellStat
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	ESpellElement element;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EDSTargetType targetType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EActionCastType castType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat")
	ESpellBookType SpellBookType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int PointPerPower = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int MpCost = 2;
};

UCLASS(BlueprintType)
class DS_API UDSSpellData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Info")
	FName SpellName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Info")
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Info")
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat")
	FSpellStat spellStat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat")
	float MinPower = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat")
	float MaxPower = 20.f;

	// 시전자 위치에 스폰되는 이펙트
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	TSoftClassPtr<AEffectBase> CasterEffect;

	// 대상 위치에 스폰되는 이펙트
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	TSoftClassPtr<AEffectBase> TargetEffect;

	// 없으면 즉발, 있으면 투사체 발사
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	TSoftClassPtr<AProjectileBase> Projectile;

	// 이 마법을 실행할 Spell 아키타입 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spell")
	TSoftClassPtr<USpell> SpellClass;

	// 타겟 선택에 사용할 셀렉터 (미지정 시 기본 셀렉터)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeting")
	TSubclassOf<ASelector> SelectorClass;

	// 스펠 세기(PointPerPower: 2/4/6/.../20)를 배우는 데 필요한 최소 스킬(Thaumaturgy/Theology) 점수로 변환
	// 2->5, 4->15, 6->30, 8->40, 10->50, 12->60, 14->70, 16->80, 18->90, 20->98
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spell")
	static int32 GetRequiredSkillForPower(int32 PointPerPower);

#if WITH_EDITOR
	// SpellClass의 Targeter 인터페이스와 SelectorClass의 ProducedTargetType 모순 검증
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif
};
