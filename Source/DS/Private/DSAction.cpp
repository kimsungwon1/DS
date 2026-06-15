


#include "DSAction.h"
#include "CharacterInstanceComponent.h"
#include "DSGameMode.h"
#include "DSPlayerController.h"
#include "DSMainWidget.h"

void UDSAction::Initialize_Implementation(UCharacterInstanceComponent* actorInstance)
{
	actor = actorInstance;
	actorChar = actor->GetCharacter();
}

bool UDSAction::SelectAction_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("%s selects %s"), *actor->characterName.ToString(), *actionName.ToString());

	auto* m = actor->GetDSGameMode();

	auto* c = m->GetDSPlayerController();

	c->OnActionSelected(this);

	return true;
}

void UDSAction::StartAction_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("%s starts %s"), *actor->characterName.ToString(), *actionName.ToString());
}

void UDSAction::ExecuteAction_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("%s executes %s"), *actor->characterName.ToString(), *actionName.ToString());
}

void UDSAction::EndAction_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("%s ends %s"), *actor->characterName.ToString(), *actionName.ToString());

	actor->GetDSGameMode()->PopFocus(actor);
}

void UDSAction::ReceiveTurn_Implementation(ADSGameMode* mode, UObject* passer)
{
	// Mode
}

void UDSAction::TurnFinished_Implementation(ADSGameMode* mode, float fDelay)
{

}

void UDSAction::PassTurn_Implementation(ADSGameMode* mode, UObject* switcher)
{

}
