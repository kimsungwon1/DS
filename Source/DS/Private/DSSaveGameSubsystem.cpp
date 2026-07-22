
#include "DSSaveGameSubsystem.h"
#include "Kismet/GameplayStatics.h"

const FString UDSSaveGameSubsystem::IndexSlotName = TEXT("CharacterIndex");
const int32 UDSSaveGameSubsystem::UserIndex = 0;

UDSRPGCharacterIndex* UDSSaveGameSubsystem::LoadOrCreateIndex() const
{
	UDSRPGCharacterIndex* Index = Cast<UDSRPGCharacterIndex>(
		UGameplayStatics::LoadGameFromSlot(IndexSlotName, UserIndex));

	if (!Index)
	{
		Index = Cast<UDSRPGCharacterIndex>(
			UGameplayStatics::CreateSaveGameObject(UDSRPGCharacterIndex::StaticClass()));
	}

	return Index;
}

bool UDSSaveGameSubsystem::DoesCharacterNameExist(FName Name) const
{
	UDSRPGCharacterIndex* Index = LoadOrCreateIndex();
	return Index && Index->AllCharacterNames.Contains(Name);
}

bool UDSSaveGameSubsystem::SaveCharacter(const FPlayerCharacterSaveData& Character, FText& OutError)
{
	if (DoesCharacterNameExist(Character.characterName))
	{
		OutError = FText::FromString(TEXT("This name already exists."));
		return false;
	}

	UDSRPGSaveCharacter* SaveObj = Cast<UDSRPGSaveCharacter>(
		UGameplayStatics::CreateSaveGameObject(UDSRPGSaveCharacter::StaticClass()));
	if (!SaveObj)
	{
		OutError = FText::FromString(TEXT("Failed to create save object."));
		return false;
	}

	SaveObj->character = Character;

	const FString SlotName = Character.characterName.ToString();
	if (!UGameplayStatics::SaveGameToSlot(SaveObj, SlotName, UserIndex))
	{
		OutError = FText::FromString(TEXT("Failed to save."));
		return false;
	}

	UDSRPGCharacterIndex* Index = LoadOrCreateIndex();
	Index->AllCharacterNames.Add(Character.characterName);
	UGameplayStatics::SaveGameToSlot(Index, IndexSlotName, UserIndex);

	return true;
}

bool UDSSaveGameSubsystem::UpdateCharacter(const FPlayerCharacterSaveData& Character, FText& OutError)
{
	UDSRPGSaveCharacter* SaveObj = Cast<UDSRPGSaveCharacter>(
		UGameplayStatics::CreateSaveGameObject(UDSRPGSaveCharacter::StaticClass()));
	if (!SaveObj)
	{
		OutError = FText::FromString(TEXT("Failed to create save object."));
		return false;
	}

	SaveObj->character = Character;

	const FString SlotName = Character.characterName.ToString();
	if (!UGameplayStatics::SaveGameToSlot(SaveObj, SlotName, UserIndex))
	{
		OutError = FText::FromString(TEXT("Failed to save."));
		return false;
	}

	// 원래 있던 이름이면 인덱스엔 이미 있겠지만, 없던 경우(신규)도 방어적으로 처리
	UDSRPGCharacterIndex* Index = LoadOrCreateIndex();
	Index->AllCharacterNames.AddUnique(Character.characterName);
	UGameplayStatics::SaveGameToSlot(Index, IndexSlotName, UserIndex);

	return true;
}

TArray<FPlayerCharacterSaveData> UDSSaveGameSubsystem::LoadAllCharacters() const
{
	TArray<FPlayerCharacterSaveData> Result;

	UDSRPGCharacterIndex* Index = LoadOrCreateIndex();
	if (!Index) return Result;

	for (const FName& Name : Index->AllCharacterNames)
	{
		if (UDSRPGSaveCharacter* Saved = Cast<UDSRPGSaveCharacter>(
			UGameplayStatics::LoadGameFromSlot(Name.ToString(), UserIndex)))
		{
			Result.Add(Saved->character);
		}
	}

	return Result;
}

bool UDSSaveGameSubsystem::DeleteCharacter(FName Name)
{
	UDSRPGCharacterIndex* Index = LoadOrCreateIndex();
	if (!Index || !Index->AllCharacterNames.Contains(Name))
	{
		return false;
	}

	UGameplayStatics::DeleteGameInSlot(Name.ToString(), UserIndex);
	Index->AllCharacterNames.Remove(Name);
	UGameplayStatics::SaveGameToSlot(Index, IndexSlotName, UserIndex);

	return true;
}
