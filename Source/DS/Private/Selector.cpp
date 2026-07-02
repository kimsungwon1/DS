


#include "Selector.h"
#include "DSPlayerController.h"
#include "BaseCharacter.h"
#include "CharacterInstanceComponent.h"
#include "Attack.h"
#include "DSAction.h"
#include "SpellCast.h"
#include "Targeter_DSCharacter.h"
#include "Targeter_PositionVector.h"
#include "Spell.h"

// Sets default values for this component's properties
ASelector::ASelector()
{
	PrimaryActorTick.bCanEverTick = true;
}

ESetTargetReturnType ASelector::SetTargetForAction_Implementation(UDSAction* action)
{
	if (selectedCharacters.IsEmpty())
		return ESetTargetReturnType::NoHit;

	// SpellCast면 heldSpell에 직접 위임
	UObject* targeter = action;
	if (USpellCast* SpellCast = Cast<USpellCast>(action))
	{
		if (USpell* Spell = SpellCast->GetSpell())
			targeter = Spell;
	}

	if (!targeter->Implements<UTargeter_DSCharacter>())
		return ESetTargetReturnType::NoHit;

	if (!ITargeter_DSCharacter::Execute_IsTargetValid_character(targeter, selectedCharacters[0]))
		return ESetTargetReturnType::TooFar;

	ITargeter_DSCharacter::Execute_SetTarget_character(targeter, selectedCharacters[0]);
	return ESetTargetReturnType::Success;
}

FRetHit ASelector::CastHit_Implementation()
{
	FRetHit ret;
	FHitResult HitResult;
	bool bHit = pcController->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel3, false, HitResult);

	ret.bHit = bHit;
	ret.hitResult = HitResult;
	return ret;
}

// Called when the game starts
void ASelector::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void ASelector::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (pcController)
	{
		// bool bHit = pcController->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel3, false, HitResult);

		FRetHit hr = CastHit();

		bool bHit = hr.bHit;
		FHitResult HitResult = hr.hitResult;

		if (bHit && HitResult.GetActor() && HitResult.GetActor()->IsA<ABaseCharacter>())
		{
			UActorComponent* ac = HitResult.GetActor()->GetComponentByClass<UCharacterInstanceComponent>();
			
			UMeshComponent* mesh = HitResult.GetActor()->GetComponentByClass<UMeshComponent>();

			UCharacterInstanceComponent* cic = Cast<UCharacterInstanceComponent>(ac);

			if (selectedCharacters.IsValidIndex(0) && selectedCharacters[0] != cic)
			{
				// 이전 대상의 아웃라인을 안전하게 꺼줍니다.
				selectedCharacters[0]->GetMesh()->SetRenderCustomDepth(false);

				selectedCharacters.Empty();
			}

			if (cic && mesh && selectedCharacters.Num() == 0)
			{
				mesh->SetRenderCustomDepth(true);
				selectedCharacters.Add(cic);
			}
		}
		else if ((!bHit || !HitResult.GetActor()->IsA<ABaseCharacter>()) && selectedCharacters.IsValidIndex(0))
		{
			// 이전 대상의 아웃라인을 안전하게 꺼줍니다.
			selectedCharacters[0]->GetMesh()->SetRenderCustomDepth(false);
			selectedCharacters.Empty();
		}
	}
}

void ASelector::Empty()
{
	for (auto* selected : selectedCharacters)
	{
		selected->GetMesh()->SetRenderCustomDepth(false);
	}
	selectedCharacters.Empty();
}

void ASelector::SetLockedTarget(UCharacterInstanceComponent* newTarget)
{
	for (auto* selected : selectedCharacters)
	{
		selected->GetMesh()->SetRenderCustomDepth(false);
	}
	selectedCharacters.Empty();

	if (newTarget)
	{
		newTarget->GetMesh()->SetRenderCustomDepth(true);
		selectedCharacters.Add(newTarget);
	}
}

