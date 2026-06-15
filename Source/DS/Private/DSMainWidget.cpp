


#include "DSMainWidget.h"
#include "PlayerCharacterInstanceComponent.h"

UDSMainWidget::UDSMainWidget() : characters{ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr }
{

}

void UDSMainWidget::SetCharacter_Implementation(int32 index, UPlayerCharacterInstanceComponent* playerData)
{
	if (index < 0 || index > 6) {
		return;
	}
	characters[index] = playerData;
	playerData->SetCharacter(index);
}