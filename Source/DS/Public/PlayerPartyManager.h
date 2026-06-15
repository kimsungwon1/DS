

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PlayerPartyManager.generated.h"
/**
 * 
 */
class UDSPlayerCharacterInstance;
class UDSPlayerParty;

UCLASS(BlueprintType, Blueprintable)
class UPlayerPartyManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPlayerPartyManager();
	~UPlayerPartyManager();

	TArray<UDSPlayerCharacterInstance*> GetSavedPlayerCharacters();

};

