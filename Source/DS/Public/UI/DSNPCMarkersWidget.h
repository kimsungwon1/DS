#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DSNPCMarkersWidget.generated.h"

class UImage;

UCLASS()
class DS_API UDSNPCMarkersWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Marks")
	void SetSlot(int32 Index, bool bShow, FLinearColor Color);

private:
	UImage* GetSlot(int32 Index) const;
};
