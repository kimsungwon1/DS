

#pragma once

#include "CoreMinimal.h"
#include "DSAction.h"
#include "Targeter_DSCharacter.h"
#include "Attack.generated.h"
/**
 * 
 */

class AWeapon;

UCLASS()
class DS_API UAttack : public UDSAction, public ITargeter_DSCharacter
{
	GENERATED_BODY()
public:
	UAttack();
	~UAttack();

	bool IsTargetValid_character_Implementation(class UCharacterInstanceComponent* targetCharacter) override;

	virtual void ReadyAction_Implementation() override;

	virtual void StartAction_Implementation() override;

	virtual void ExecuteAction_Implementation() override;

	virtual void EndAction_Implementation() override { Super::EndAction_Implementation(); }

	void SetTarget_character_Implementation(class UCharacterInstanceComponent* targetCharacter) { target = targetCharacter; }

	void Initialize_Implementation(UCharacterInstanceComponent* actorInstance);
protected:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AWeapon> usingWeapon;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UCharacterInstanceComponent> target;

	bool IsTargetLost_Implementation() const override;

	FTimerHandle AttackTimerHandle;
};
