

#pragma once

#include "CoreMinimal.h"
#include "DSAction.h"
#include "Targeter_DSCharacter.h"
#include "Targeter_PositionVector.h"
#include "SpellCast.generated.h"

class USpell;

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

	virtual void StartAction_Implementation() override;

	virtual void ExecuteAction_Implementation() override;

protected:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<USpell> heldSpell;
};
