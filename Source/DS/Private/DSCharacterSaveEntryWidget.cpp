
#include "DSCharacterSaveEntryWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"

namespace
{
	FString GetProfessionDisplayNameLocal_SaveEntry(EProfession Value)
	{
		const UEnum* EnumPtr = StaticEnum<EProfession>();
		return EnumPtr ? EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(Value)).ToString() : FString();
	}

	FString GetRaceDisplayNameLocal_SaveEntry(ERace Value)
	{
		const UEnum* EnumPtr = StaticEnum<ERace>();
		return EnumPtr ? EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(Value)).ToString() : FString();
	}
}

FReply UDSCharacterSaveEntryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		OnEntryClicked.Broadcast(this);
		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UDSCharacterSaveEntryWidget::SetCharacterData(const FPlayerCharacterSaveData& InData)
{
	CharacterData = InData;

	if (RosterEntryPortrait)
	{
		if (UTexture2D* Portrait = CharacterData.Portrait.LoadSynchronous())
		{
			RosterEntryPortrait->SetBrushFromTexture(Portrait);
		}
	}

	if (RosterEntryName)
	{
		RosterEntryName->SetText(FText::FromName(CharacterData.characterName));
	}
	if (RosterEntryClass)
	{
		RosterEntryClass->SetText(FText::FromString(GetProfessionDisplayNameLocal_SaveEntry(CharacterData.Profession)));
	}
	if (RosterEntryRace)
	{
		RosterEntryRace->SetText(FText::FromString(GetRaceDisplayNameLocal_SaveEntry(CharacterData.Race)));
	}
}
