
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveGameData.h"
#include "DSSaveGameSubsystem.generated.h"

UCLASS()
class DS_API UDSSaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// 이름 중복 체크 + 저장 + 인덱스 갱신까지 한번에. 중복이면 false + OutError 채워서 리턴
	UFUNCTION(BlueprintCallable)
	bool SaveCharacter(const FPlayerCharacterSaveData& Character, FText& OutError);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool DoesCharacterNameExist(FName Name) const;

	UFUNCTION(BlueprintCallable)
	TArray<FPlayerCharacterSaveData> LoadAllCharacters() const;

	UFUNCTION(BlueprintCallable)
	bool DeleteCharacter(FName Name);

	// SaveCharacter랑 다르게 이름 중복이어도 막지 않고 그냥 덮어씀 - 이미 존재하는 캐릭터의 최신 상태(홈베이스 복귀 등)를 갱신할 때 씀
	UFUNCTION(BlueprintCallable)
	bool UpdateCharacter(const FPlayerCharacterSaveData& Character, FText& OutError);

	// 디스크에 저장되는 값 아님 - 레벨 전환 직전에 Lobby가 세팅해서 GameMode::BeginPlay가 새 게임/이어하기 분기에만 씀
	UFUNCTION(BlueprintCallable)
	void SetPendingNewGame(bool bNewGame) { bPendingNewGame = bNewGame; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsPendingNewGame() const { return bPendingNewGame; }

private:
	bool bPendingNewGame = false;

	UDSRPGCharacterIndex* LoadOrCreateIndex() const;

	static const FString IndexSlotName;
	static const int32 UserIndex;
};
