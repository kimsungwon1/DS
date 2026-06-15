


#include "PlayerPartyManagerComponent.h"
#include "PlayerCharacterInstanceComponent.h"
#include "DSPlayerParty.h"
#include "PlayerPartyMover.h"
#include "DSPlayerController.h"

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

	// UI로 정보 전송
	auto* controller = GetDSController();

	if (controller && partyObject) {
		int32 i = 0;
		for (auto charac : partyObject->characters)
		{
			if (charac)
			{
				controller->TransferCharacterToUI(i, charac);
			}
			++i;
		}
	}
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

void UPlayerPartyManagerComponent::initializeParty_default()
{

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
}

void UPlayerPartyManagerComponent::initializeParty_saveCharactersData()
{
}