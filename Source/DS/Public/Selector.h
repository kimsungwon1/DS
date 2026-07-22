

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DSTarget.h"
#include "Selector.generated.h"

class ADSPlayerController;
class USpell;
class UDSAction;

USTRUCT(BlueprintType)
struct FRetHit
{
public:
	FRetHit() {}
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	bool bHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	FHitResult hitResult;
};

UENUM(BlueprintType)
enum ESetTargetReturnType : uint8
{
	Success UMETA(DisplayName = "Success"),
	NoHit UMETA(DisplayName = "No Hit"),
	TooFar UMETA(DisplayName = "Too Far")
};

UCLASS(BlueprintType, Blueprintable)
class DS_API ASelector : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ASelector();

	// 이 셀렉터가 골라주는 대상 유형 — SpellData 검증(IsDataValid)에서 사용
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Selector")
	EDSTargetType ProducedTargetType = EDSTargetType::None;

	void Initialize(ADSPlayerController* controller, APostProcessVolume* volume, UDSAction* action = nullptr);

	void Empty();

	void SetLockedTarget(class UCharacterInstanceComponent* newTarget);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FRetHit CastHit();
	FRetHit CastHit_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ESetTargetReturnType SetTargetForAction(class UDSAction* action);
	ESetTargetReturnType SetTargetForAction_Implementation(class UDSAction* action);

	UFUNCTION(BlueprintCallable)
	void SetTargetOutlinerValid(bool isValid);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	ADSPlayerController* pcController;

	UPROPERTY(BlueprintReadWrite)
	TArray<class UCharacterInstanceComponent*> selectedCharacters;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDSAction> reservedAction = nullptr;

	UPROPERTY()
	TObjectPtr<APostProcessVolume> TargetVolume;
};
