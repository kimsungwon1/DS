

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS(Abstract)
class DS_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Use();
	virtual void Use_Implementation() {}

	UFUNCTION(BlueprintCallable)
	float GetDistanceToUse() const;

	void SetUser(class UCharacterInstanceComponent* u) { user = u; }
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UCharacterInstanceComponent> user;
};
