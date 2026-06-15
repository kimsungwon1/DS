#pragma once

#include "CoreMinimal.h"
#include "ActionCastType.generated.h"

UENUM(BlueprintType)
enum class EActionCastType : uint8
{
	Instant UMETA(DisplayName = "Instant", ToolTip = "즉시 데미지가 간다."),
	ProjectileThrow UMETA(DisplayName = "ProjectileThrow", ToolTip = "화살, 표창 따위가 적에게 던져진다"),
};