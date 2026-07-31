
#pragma once

#include "CoreMinimal.h"
#include "DSUserWidget.h"
#include "DSDefeatScreenWidget.generated.h"

// 파티 전멸 시 뷰포트에 바로 띄우는 저승사자 이미지 화면 (DSG_DefeatScreen 전용).
// 잠깐 떠 있다가 알파값이 서서히 낮아지며 사라지는 연출은 BP 위젯 애니메이션으로 처리하고,
// 그 애니메이션이 끝나면 BP에서 RemoveFromParent를 호출해 스스로 정리하면 됨
UCLASS()
class DS_API UDSDefeatScreenWidget : public UDSUserWidget
{
	GENERATED_BODY()

public:
	// 화면을 만들어서 바로 뷰포트에 띄움
	UFUNCTION(BlueprintCallable, Category = "DefeatScreen")
	static UDSDefeatScreenWidget* Show(UObject* WorldContextObject, TSubclassOf<UDSDefeatScreenWidget> DefeatScreenClass, int32 ZOrder = 2000);
};
