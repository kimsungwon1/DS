


#include "PlayerPartyManagerComponent.h"
#include "PlayerCharacterInstanceComponent.h"
#include "DSPlayerParty.h"
#include "PlayerPartyMover.h"
#include "DSPlayerController.h"
#include "DSSaveGameSubsystem.h"

// Sets default values for this component's properties
UPlayerPartyManagerComponent::UPlayerPartyManagerComponent() : characters_developerDefine {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerPartyManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	switch (eCharactersSpawnType)
	{
	case ECharactersSpawnType::CharactersDefine:
		initializeParty_charactersDefine();
		break;
	case ECharactersSpawnType::PartyDefine:
		initializeParty_partyDefine();
		break;
	case ECharactersSpawnType::SaveGameData:
		initializeParty_saveGameData();
		break;
	case ECharactersSpawnType::SaveCharactersData:
		initializeParty_saveCharactersData();
		break;
	case ECharactersSpawnType::Default:
	default:
		initializeParty_default();
		break;
	}

	RefreshPartyUI();
}


// Called every frame
void UPlayerPartyManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

APlayerPartyMover* UPlayerPartyManagerComponent::GetPlayerPartyMover()
{
	if (!partyMover)
	{
		auto* owner = GetOwner();
		if (owner) {
			partyMover = Cast<APlayerPartyMover>(owner);
		}
	}
	return partyMover;
}

ADSPlayerController* UPlayerPartyManagerComponent::GetDSController()
{
	if (auto* p = GetPlayerPartyMover()) {
		return p->GetDSController();
	}
	return nullptr;
}

void UPlayerPartyManagerComponent::RefreshPartyUI()
{
	auto* controller = GetDSController();
	if (!controller || !partyObject) return;

	int32 i = 0;
	for (auto charac : partyObject->characters)
	{
		// 빈 슬롯도 nullptr로 명시적으로 넘겨야 UI가 "비었음"(X 이미지 등)으로 갱신됨 - 그냥 건너뛰면 예전 얼굴이 그대로 남음
		controller->TransferCharacterToUI(i, charac);
		++i;
	}
}

void UPlayerPartyManagerComponent::NotifyMemberDied(UPlayerCharacterInstanceComponent* Player)
{
	if (Player)
		OnPartyMemberDied.Broadcast(Player);
}

void UPlayerPartyManagerComponent::NotifyMemberLeft(UPlayerCharacterInstanceComponent* Player)
{
	if (Player)
		OnPartyMemberLeft.Broadcast(Player);
}

void UPlayerPartyManagerComponent::initializeParty_default()
{
	partyObject = GetWorld()->SpawnActor<ADSPlayerParty>(FVector{}, FRotator{});
}

void UPlayerPartyManagerComponent::initializeParty_charactersDefine()
{
	partyObject = GetWorld()->SpawnActor<ADSPlayerParty>(FVector{}, FRotator{});
	int i = 0;
	
	for(auto var : characters_developerDefine)
	{
		if (!var.IsNull())
		{
			UClass* loadedClass = var.LoadSynchronous();
			if (loadedClass)
			{
				auto* newCharacter = NewObject<UPlayerCharacterInstanceComponent>(GetPlayerPartyMover(), loadedClass);
				newCharacter->RegisterComponent();
				partyObject->characters[i] = newCharacter;
			}
		}
		i++;
	}
}

void UPlayerPartyManagerComponent::initializeParty_partyDefine()
{
	if (!party_developerDefine.IsNull())
	{
		UClass* loadedClass = party_developerDefine.LoadSynchronous();
		if (loadedClass)
		{
			partyObject = GetWorld()->SpawnActor<ADSPlayerParty>(FVector{}, FRotator{});
		}
	}
}

void UPlayerPartyManagerComponent::initializeParty_saveGameData()
{
	partyObject = GetWorld()->SpawnActor<ADSPlayerParty>(FVector{}, FRotator{});
}

void UPlayerPartyManagerComponent::ApplySaveDataToParty(const TArray<FPlayerCharacterSaveData>& PartySaveData)
{
	APlayerPartyMover* Mover = GetPlayerPartyMover();
	if (!Mover)
	{
		return;
	}

	UClass* LoadedClass = PCInstanceComponentClass.LoadSynchronous();
	if (!LoadedClass)
	{
		return;
	}

	if (!partyObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplySaveDataToParty: partyObject가 없습니다. 파티 컨테이너가 먼저 준비되어 있어야 합니다."));
		return;
	}

	for (int32 i = 0; i < PartySaveData.Num(); ++i)
	{
		const FPlayerCharacterSaveData& SaveData = PartySaveData[i];
		if (SaveData.characterName.IsNone())
		{
			continue;
		}

		auto* NewCharacter = NewObject<UPlayerCharacterInstanceComponent>(Mover, LoadedClass);
		NewCharacter->RegisterComponent();
		NewCharacter->InitializeFromSaveData(SaveData);

		if (partyObject->characters.IsValidIndex(i))
		{
			partyObject->characters[i] = NewCharacter;
		}

		int32 OutCapsuleIndex = INDEX_NONE;
		int32 OutSlotIndex = INDEX_NONE;
		Mover->AllocatePlayerCapsule(NewCharacter, -1, OutCapsuleIndex, OutSlotIndex);
	}
}

void UPlayerPartyManagerComponent::initializeParty_saveCharactersData()
{
}

UPlayerCharacterInstanceComponent* UPlayerPartyManagerComponent::AddCharacterToParty(const FPlayerCharacterSaveData& SaveData, int32 SlotIndex)
{
	APlayerPartyMover* Mover = GetPlayerPartyMover();
	if (!Mover || !partyObject || !partyObject->characters.IsValidIndex(SlotIndex))
	{
		return nullptr;
	}

	if (partyObject->characters[SlotIndex] != nullptr)
	{
		return nullptr; // 이미 차있는 슬롯
	}

	UClass* LoadedClass = PCInstanceComponentClass.LoadSynchronous();
	if (!LoadedClass)
	{
		return nullptr;
	}

	auto* NewCharacter = NewObject<UPlayerCharacterInstanceComponent>(Mover, LoadedClass);
	NewCharacter->RegisterComponent();
	NewCharacter->InitializeFromSaveData(SaveData);

	partyObject->characters[SlotIndex] = NewCharacter;

	int32 OutCapsuleIndex = INDEX_NONE;
	int32 OutSlotIndex = INDEX_NONE;
	Mover->AllocatePlayerCapsule(NewCharacter, -1, OutCapsuleIndex, OutSlotIndex);

	return NewCharacter;
}

bool UPlayerPartyManagerComponent::RemoveCharacterFromParty(int32 SlotIndex)
{
	APlayerPartyMover* Mover = GetPlayerPartyMover();
	if (!Mover || !partyObject || !partyObject->characters.IsValidIndex(SlotIndex))
	{
		return false;
	}

	UPlayerCharacterInstanceComponent* Member = partyObject->characters[SlotIndex];
	if (!Member)
	{
		return false;
	}

	// 파티에서 빠지기 전 현재 상태를 디스크에 반영 (SaveCharacter는 이름 중복이면 거부하니까 UpdateCharacter로)
	if (UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr)
	{
		if (UDSSaveGameSubsystem* SaveSubsystem = GI->GetSubsystem<UDSSaveGameSubsystem>())
		{
			FText UpdateError;
			SaveSubsystem->UpdateCharacter(Member->ToSaveData(), UpdateError);
		}
	}

	Mover->DeallocatePlayerCapsule(Member);
	Member->DestroyComponent();

	partyObject->characters[SlotIndex] = nullptr;

	return true;
}