

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "ActionCastType.h"
#include "DSBlueprintLibrary.h"
#include "DSTarget.h"
#include "Spell.generated.h"

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
	// Sets default values for this component's properties
	USpell();

	UFUNCTION()
	ESpellResult CheckResult();

	UFUNCTION(BlueprintCallable)
	void Cast();

protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Cast_Success();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Fizzle();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Fizzle_SparkToParty();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FSpellStat SpellStat;

};
