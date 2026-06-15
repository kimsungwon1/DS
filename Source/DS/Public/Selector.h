

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Selector.generated.h"

class ADSPlayerController;

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

	void Initialize(ADSPlayerController* controller) { pcController = controller; }

	void Empty();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FRetHit CastHit();
	FRetHit CastHit_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ESetTargetReturnType SetTargetForAction(class UDSAction* action);
	ESetTargetReturnType SetTargetForAction_Implementation(class UDSAction* action);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	ADSPlayerController* pcController;

	UPROPERTY(BlueprintReadWrite)
	TArray<class UCharacterInstanceComponent*> selectedCharacters;
};
