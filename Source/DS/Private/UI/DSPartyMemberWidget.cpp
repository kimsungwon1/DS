#include "UI/DSPartyMemberWidget.h"
#include "PlayerCharacterInstanceComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Border.h"
#include "Components/Overlay.h"
#include "Engine/Texture2D.h"

void UDSPartyMemberWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SelectionBorder)
		SelectionBorder->SetBrushColor(NormalBorderColor);

	if (DeadOverlay)
		DeadOverlay->SetVisibility(ESlateVisibility::Collapsed);
}

void UDSPartyMemberWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (CharacterInstance)
		RefreshUI();
}

void UDSPartyMemberWidget::SetCharacterInstance(UPlayerCharacterInstanceComponent* InCharacter)
{
	CharacterInstance = InCharacter;
	RefreshUI();
}

void UDSPartyMemberWidget::RefreshUI()
{
	if (!CharacterInstance)
		return;

	const FFaceStat& Face = CharacterInstance->GetFinalFaceStat();
	const float HPRatio = (Face.MaxHP > 0) ? FMath::Clamp((float)Face.Health / Face.MaxHP, 0.f, 1.f) : 0.f;
	const float SPRatio = (Face.MaxSP > 0) ? FMath::Clamp((float)Face.CurrentSP / Face.MaxSP, 0.f, 1.f) : 0.f;

	if (CharacterNameText)
		CharacterNameText->SetText(FText::FromName(CharacterInstance->characterName));

	if (HPBar)
	{
		HPBar->SetPercent(HPRatio);
		HPBar->SetFillColorAndOpacity(FMath::Lerp(HPColorLow, HPColorFull, HPRatio));
	}

	if (SPBar)
	{
		SPBar->SetPercent(SPRatio);
		SPBar->SetFillColorAndOpacity(SPColor);
	}

	if (HPText)
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), Face.Health, Face.MaxHP)));

	if (SPText)
		SPText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), Face.CurrentSP, Face.MaxSP)));

	const bool bDead = CharacterInstance->IsDead();
	if (DeadOverlay)
		DeadOverlay->SetVisibility(bDead ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	if (bDead)
		OnCharacterDead();
}

void UDSPartyMemberWidget::SetSelected(bool bSelected)
{
	bIsSelected = bSelected;
	if (SelectionBorder)
		SelectionBorder->SetBrushColor(bSelected ? SelectedBorderColor : NormalBorderColor);
}

void UDSPartyMemberWidget::OnCharacterDead_Implementation()
{
	// Blueprint에서 사망 연출 추가 가능
}
