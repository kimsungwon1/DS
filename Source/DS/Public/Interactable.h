

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DS_API IInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsInteractable(class APlayerPartyMover* ppm, float distance);
	virtual bool IsInteractable_Implementation(class APlayerPartyMover* ppm, float distance) { return false; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool Interact(class APlayerPartyMover* ppm, FText& textToExpress);
	virtual bool Interact_Implementation(class APlayerPartyMover* ppm, FText& textToExpress) { return false; }
};
