

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SaveGameData.h"
#include "PlayerPartyManagerComponent.generated.h"

class ADSPlayerParty;
class APlayerPartyMover;
class UPlayerCharacterInstanceComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPartyMemberDied, UPlayerCharacterInstanceComponent*, Player);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPartyMemberLeft, UPlayerCharacterInstanceComponent*, Player);

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

	/** 파티원이 사망했을 때 */
	UPROPERTY(BlueprintAssignable, Category = "Party")
	FOnPartyMemberDied OnPartyMemberDied;

	/** 파티원이 파티를 떠났을 때 */
	UPROPERTY(BlueprintAssignable, Category = "Party")
	FOnPartyMemberLeft OnPartyMemberLeft;

	UFUNCTION(BlueprintCallable, Category = "Party")
	void NotifyMemberDied(UPlayerCharacterInstanceComponent* Player);

	UFUNCTION(BlueprintCallable, Category = "Party")
	void NotifyMemberLeft(UPlayerCharacterInstanceComponent* Player);

	// GatherPartyWidget 등에서 확정한 세이브데이터로 파티를 직접 채움 (eCharactersSpawnType 스위치와 무관하게 독립 호출).
	// 빈 슬롯(characterName == NAME_None)은 건너뜀. 슬롯당 PCInstanceComponentClass(예: BP_PCInstanceComponent)를
	// NewObject로 생성 + InitializeFromSaveData 호출 + 캡슐 슬롯까지 할당.
	UFUNCTION(BlueprintCallable, Category = "Party")
	void ApplySaveDataToParty(const TArray<FPlayerCharacterSaveData>& PartySaveData);

	// GatherPartyWidget의 파티 슬롯 하나에 세이브데이터로 바로 라이브 캐릭터를 만들어 꽂음 (스폰+캡슐할당까지).
	// 슬롯이 이미 차있으면 실패(nullptr).
	UFUNCTION(BlueprintCallable, Category = "Party")
	UPlayerCharacterInstanceComponent* AddCharacterToParty(const FPlayerCharacterSaveData& SaveData, int32 SlotIndex);

	// 파티 슬롯 하나를 비움 - 빼기 전에 그 캐릭터의 현재 상태를 UpdateCharacter로 저장하고, 캡슐 해제 + 컴포넌트 파괴까지 함
	UFUNCTION(BlueprintCallable, Category = "Party")
	bool RemoveCharacterFromParty(int32 SlotIndex);

	// partyObject->characters 순서대로 GetDSController()->TransferCharacterToUI() 호출해서 하단 파티 UI(초상화 등) 갱신.
	// BeginPlay 때 한 번 호출되고, 파티 구성이 바뀔 수 있는 시점(홈베이스 나갈 때 등)마다 다시 불러줘야 함
	UFUNCTION(BlueprintCallable, Category = "Party")
	void RefreshPartyUI();

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

#pragma region SaveDataApply
protected:
	// ApplySaveDataToParty가 각 파티원 슬롯에 실제로 만들 컴포넌트 클래스 (블루프린트에서 BP_PCInstanceComponent로 지정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PartySpawn")
	TSoftClassPtr<UPlayerCharacterInstanceComponent> PCInstanceComponentClass;
#pragma endregion

private:
	APlayerPartyMover* partyMover;
};
