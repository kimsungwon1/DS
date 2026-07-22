

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "LobbyDescendingCamera.generated.h"

class UCameraComponent;

// 로비 배경용 - 계속 아래로 하강하면서 위쪽 뚫린 곳의 빛을 올려다보는 카메라. 레벨 시작하면 자동으로 빙의됨
UCLASS()
class DS_API ALobbyDescendingCamera : public APawn
{
	GENERATED_BODY()

public:
	ALobbyDescendingCamera();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;

protected:
	// 초당 하강 속도 (-Z 방향)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Descent")
	float DescendSpeed = 180.f;

	// 시작 위치 기준 이만큼 내려가면 다시 시작 위치로 되돌아가서 루프 (0 이하면 무한 하강)
	// 통로 끝(바닥 뚫린 구멍)에 절대 안 닿도록, 안개/빛이 안 닿아 이미 캄캄해진 깊이에서 리셋되게 잡음 -
	// 그래야 리셋되는 순간이 어두워서 티가 안 남. 통로 길이(40000)의 극히 일부만 씀 - 나머지는 순전히 여유분
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Descent")
	float LoopDepth = 9000.f;

private:
	FVector StartLocation = FVector::ZeroVector;
};
