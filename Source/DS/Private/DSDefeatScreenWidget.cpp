
#include "DSDefeatScreenWidget.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

UDSDefeatScreenWidget* UDSDefeatScreenWidget::Show(UObject* WorldContextObject, TSubclassOf<UDSDefeatScreenWidget> DefeatScreenClass, int32 ZOrder)
{
	if (!WorldContextObject || !DefeatScreenClass) return nullptr;

	UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	if (!World) return nullptr;

	UDSDefeatScreenWidget* Screen = CreateWidget<UDSDefeatScreenWidget>(World, DefeatScreenClass);
	if (!Screen) return nullptr;

	Screen->AddToViewport(ZOrder);
	return Screen;
}
