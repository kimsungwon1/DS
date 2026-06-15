

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "TurnSwitcher.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTurnSwitcher : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */

//나중에 투사체한테도 걸자.
class DS_API ITurnSwitcher
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReceiveTurn(ADSGameMode* mode, UObject* passer);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TurnFinished(ADSGameMode* mode, float fDelay);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PassTurn(ADSGameMode* mode, UObject* switcher);
};
