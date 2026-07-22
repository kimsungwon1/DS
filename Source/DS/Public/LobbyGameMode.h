

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

// 로비 월드 전용 - ADSGameMode(전투/파티 로직)랑 완전히 무관한 가벼운 GameMode.
// DefaultPawnClass를 비워서 GameMode가 알아서 아무 폰도 스폰/빙의 안 시킴 -
// 레벨에 배치해둔 ALobbyDescendingCamera(AutoPossessPlayer)가 알아서 빙의됨
UCLASS()
class DS_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALobbyGameMode();
};
