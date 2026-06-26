

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Components/WidgetComponent.h"
#include "BaseNPC.generated.h"

class UDSNPCCharacterInstance;
class UNPCCharacterInstanceComponent;

UCLASS(Abstract)
class DS_API ABaseNPC : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ABaseNPC();

protected:
	virtual void BeginPlay() override;

	void Initialize_CharacterDefine();
public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftClassPtr<class UNPCCharacterInstanceComponent> softNpcPtr;
};
