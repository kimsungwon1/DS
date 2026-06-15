

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerPartyManagerComponent.generated.h"

class ADSPlayerParty;
class APlayerPartyMover;

UENUM(BlueprintType)
enum class ECharactersSpawnType : uint8
{
	Default UMETA(DisplayName = "Default", ToolTip = "In Game. User Making new party or Loading saved Party. Others for Developer testing."),
	CharactersDefine UMETA(DisplayName = "CharactersDefine", ToolTip = "Developer defines all characters by character assets directly for testing."),
	PartyDefine UMETA(DisplayName = "PartyDefine", ToolTip = "Developer defines party by party assets for testing."),
	SaveGameData UMETA(DisplayName = "SaveGameData", ToolTip = "Developer imports party by saved game data."),
	SaveCharactersData UMETA(DisplayName = "SaveCharactersData", ToolTip = "Developer imports characters by saved characters data.")
};

UCLASS( ClassGroup=(Custom), BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent) )
class DS_API UPlayerPartyManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerPartyManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	APlayerPartyMover* GetPlayerPartyMover();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ADSPlayerController* GetDSController();

	UPROPERTY(BlueprintReadWrite, Category = "RPG")
	TObjectPtr<ADSPlayerParty> partyObject;

#pragma region HelperFunctions
private:
	void initializeParty_default();
	void initializeParty_charactersDefine();
	void initializeParty_partyDefine();
	void initializeParty_saveGameData();
	void initializeParty_saveCharactersData();
#pragma endregion

#pragma region SpawnType
protected:
	UPROPERTY(EditAnywhere, EditFixedSize, BlueprintReadWrite, Category = "PartySpawn")
	ECharactersSpawnType eCharactersSpawnType = ECharactersSpawnType::Default;

	UPROPERTY(EditAnywhere, EditFixedSize, BlueprintReadWrite, Category="PartySpawn")
	TArray<TSoftClassPtr<class UPlayerCharacterInstanceComponent>> characters_developerDefine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PartySpawn")
	TSoftClassPtr<ADSPlayerParty> party_developerDefine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PartySpawn")
	TSoftClassPtr<class UDSRPGSaveGame> saveGame_developerDefine;

	UPROPERTY(EditAnywhere, EditFixedSize, BlueprintReadWrite, Category = "PartySpawn")
	TArray<TSoftClassPtr<class UDSRPGSaveCharacter>> saveCharacter_developerDefine;
#pragma endregion

private:
	APlayerPartyMover* partyMover;
};
