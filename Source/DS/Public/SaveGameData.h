

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DSStats.h"
#include "SaveGameData.generated.h"

USTRUCT(BlueprintType)
struct FCharacterSaveData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FGuid characterID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	FFaceStat BaseFaceStat;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Stat")
	FFaceStat FinalFaceStat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	FCharacterStat CharStat;
};

USTRUCT(BlueprintType)
struct FPartySaveData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FGuid partyID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FCharacterSaveData> arrMembers;
};

USTRUCT(BlueprintType)
struct FWorldData
{
	GENERATED_BODY()
};

UCLASS(BlueprintType, Blueprintable)
class DS_API UDSRPGSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UDSRPGSaveGame();
	~UDSRPGSaveGame();
	
	UPROPERTY()
	FWorldData world;

	UPROPERTY()
	FPartySaveData playerParty;

	UPROPERTY()
	TArray<FPartySaveData> partys;
};

/**
 * * [초기화 캐릭터 데이터]
 * 플레이어가 캐릭터 생성창에서 편집을 완료한 '마스터 데이터'를 저장하는 클래스입니다.
 * 이 에셋은 런타임 중 변경되지 않는 '원본' 역할을 하며,
 * 게임 세션 시작 시 이 데이터를 복사하여 UDSRPGSaveGame의 실시간 데이터로 전송합니다.
 */
UCLASS(BlueprintType, Blueprintable, Meta = (ShortTooltip = "Initial Character Template Data"))
class DS_API UDSRPGSaveCharacter : public USaveGame
{
	GENERATED_BODY()

	UPROPERTY()
	FCharacterSaveData character;
};