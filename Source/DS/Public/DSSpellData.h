
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Spell.h"
#include "DSSpellData.generated.h"

class AEffectBase;
class AProjectileBase;
class USpell;

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
	ESpellElement Element;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat")
	float MinPower = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat")
	float MaxPower = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat")
	int32 MPCost = 5;

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
};
