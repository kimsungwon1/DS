

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DSKeyRingComponent.generated.h"

USTRUCT(BlueprintType)
struct FKeyRingEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName KeyID;

	// 이 열쇠로 몇 번 더 열 수 있는지. 0이 되면 KeyRing에서 자동으로 사라짐
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Charges = 1;
};

// 열쇠 전용 미니멀 소지 목록. 파티(예: APlayerPartyMover)에 붙여서 씀 -
// 전멸하면 이 컴포넌트를 가진 액터와 함께 열쇠도 그 자리에 남게 하려는 의도라 GameSubsystem이 아니라 ActorComponent로 둠
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DS_API UDSKeyRingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDSKeyRingComponent();

	// 이미 갖고 있는 키면 Charges만 더해줌 (여러 문 열 수 있는 열쇠를 나중에 또 얻는 경우 대비)
	UFUNCTION(BlueprintCallable, Category = "Key")
	void AddKey(FName KeyID, int32 Charges = 1);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Key")
	bool HasKey(FName KeyID) const;

	// 충전 1 소모. 0이 되면 목록에서 제거. 애초에 갖고 있지 않았으면 false
	UFUNCTION(BlueprintCallable, Category = "Key")
	bool ConsumeKey(FName KeyID);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key")
	TArray<FKeyRingEntry> KeyRing;
};
