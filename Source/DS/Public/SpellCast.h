
#pragma once

#include "CoreMinimal.h"
#include "DSAction.h"
#include "SpellCast.generated.h"

class USpell;
class UDSSpellData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DS_API USpellCast : public UDSAction
{
	GENERATED_BODY()

public:
	USpellCast();

	UFUNCTION(BlueprintCallable)
	const USpell* GetSpell_const() const { return heldSpell; }
	UFUNCTION(BlueprintCallable)
	USpell* GetSpell() { return heldSpell; }

	UFUNCTION(BlueprintCallable)
	void SetSpellFromData(UDSSpellData* Data, int32 Level = 1);

	virtual void Initialize_Implementation(UCharacterInstanceComponent* actorInstance) override;
	virtual void StartAction_Implementation() override;
	virtual void ExecuteAction_Implementation() override;

protected:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<USpell> heldSpell;

	UPROPERTY(BlueprintReadOnly)
	int32 SpellLevel = 1;
};
