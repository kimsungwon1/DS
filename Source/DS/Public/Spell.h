

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "ActionCastType.h"
#include "DSBlueprintLibrary.h"
#include "DSTarget.h"
#include "Spell.generated.h"

class UCharacterInstanceComponent;
class UDSSpellData;
class AEffectBase;
class AProjectileBase;

UENUM(BlueprintType)
enum class ESpellResult : uint8
{
	Success UMETA(DisplayName = "Success"),
	Fizzle UMETA(DisplayName = "Fizzle"),
	Fizzle_SparkToParty UMETA(DisplayName = "SparkToParty"),
};

UENUM(BlueprintType)
enum class ESpellElement : uint8
{
	Aether UMETA(DisplayName = "Aether"),
	Water UMETA(DisplayName = "Water"),
	Air UMETA(DisplayName = "Air"),
	Earth UMETA(DisplayName = "Earth"),
	Sun UMETA(DisplayName = "Sun")//2편에선 Dark로 변경
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int PointPerPower = 2;
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
	void Cast();

	const FSpellStat& GetSpellStat() const { return SpellStat; }

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FSpellStat SpellStat;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCharacterInstanceComponent> Caster;

	UPROPERTY(BlueprintReadOnly)
	int32 CastLevel = 1;
};
