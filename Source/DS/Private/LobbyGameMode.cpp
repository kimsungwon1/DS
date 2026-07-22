

#include "LobbyGameMode.h"

ALobbyGameMode::ALobbyGameMode()
{
	// GameMode가 자동으로 스폰/빙의시키는 폰이 없게 - 레벨에 배치된 ALobbyDescendingCamera가 AutoPossessPlayer로 알아서 빙의됨
	DefaultPawnClass = nullptr;
}
