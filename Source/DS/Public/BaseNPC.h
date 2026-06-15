

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "BaseNPC.generated.h"

class UDSNPCCharacterInstance;
class UNPCCharacterInstanceComponent;

UCLASS(Abstract)
class DS_API ABaseNPC : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Initialize_CharacterDefine();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftClassPtr<class UNPCCharacterInstanceComponent> softNpcPtr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNPCCharacterInstanceComponent* npcCharacterComponent = nullptr;
};
