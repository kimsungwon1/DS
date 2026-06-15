#pragma once

#include "CoreMinimal.h"

#include "DSTarget.generated.h"

UENUM(BlueprintType)
enum class EDSTargetType : uint8
{
	Opponent UMETA(DisplayName = "Opponent"),
	OpponentParty UMETA(DisplayName = "OpponentParty"),
	Self UMETA(DisplayName = "Self"),
	Friend UMETA(DisplayName = "Friend"),
	Party UMETA(DisplayName = "Party"),
	Around UMETA(DisplayName = "Around"),
	Ground UMETA(DisplayName = "Ground"),
    UserDefine UMETA(DisplayName = "UserDefine", ToolTip = "User Defined Extent"),
    None UMETA(DisplayName = "None")
};
