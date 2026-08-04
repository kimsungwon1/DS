

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "DSDoorBase.generated.h"

UENUM(BlueprintType)
enum class EDoorSide : uint8
{
	Either,
	SideA,
	SideB
};

// 문 종류(그냥 열림/열쇠/버튼/편방향 등)를 전부 다루는 공통 조상. 실제 메시/애니메이션은 BP 자식에서 처리
UCLASS(Abstract, Blueprintable)
class DS_API ADSDoorBase : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ADSDoorBase();

	virtual void Tick(float DeltaTime) override;

	// 열기 시도. 조건 통과 못하면 false만 리턴하고 아무 일도 안 일어남 (BP에서 "잠겨있음" 연출용)
	UFUNCTION(BlueprintCallable, Category = "Door")
	bool TryOpen(AActor* Interactor);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Door")
	bool CanOpen(AActor* Interactor) const;

	// 실제로 문을 여는 동작. 기본 구현은 bIsOpen만 바꿈 - BP에서 오버라이드해서 애니메이션/사운드 추가
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Door")
	void Open();
	virtual void Open_Implementation();

	// 문을 닫음. bStaysOpenForever면 아무 일도 안 일어남
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Door")
	void Close();
	virtual void Close_Implementation();

	virtual bool IsInteractable_Implementation(class APlayerPartyMover* ppm, float distance) override;

	virtual bool Interact_Implementation(class APlayerPartyMover* ppm, FText& textToExpress) override;
protected:
	// 문짝(회전하는 부분)의 피벗. 이 밑에 실제로 열릴 때 같이 돌아야 하는 메시들을 자식으로 붙일 것
	// (문틀처럼 안 움직이는 부분은 여기 밑이 아니라 액터 루트 밑에 따로 붙여야 함)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
	TObjectPtr<USceneComponent> DoorLeafRoot;

	// 열렸을 때 DoorLeafRoot가 회전할 각도 (로컬 Yaw 기준, 도)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	float OpenYaw = 90.0f;

	// 회전 보간 속도 - 클수록 목표 각도에 빨리 도달함 (FInterpTo 계수)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	float RotationInterpSpeed = 3.0f;

	// 아래 조건들은 전부 비워두면(=기본값) 그냥 열리는 문이 됨

	// 필요한 열쇠 ID 목록. 여러 개면 OR - 이 중 아무거나 하나만 있으면 됨
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door|Condition")
	TArray<FName> RequiredKeyIDs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door|Condition")
	bool bRequiresButton = false;

	// 연결된 버튼/레버가 눌리면 BP에서 true로 세팅
	UPROPERTY(BlueprintReadWrite, Category = "Door|Condition")
	bool bButtonPressed = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door|Condition")
	EDoorSide AllowedSide = EDoorSide::Either;

	// 조건 통과해서 한 번 열리면 true로 굳어짐 - 그 뒤론 조건 재검사 없이 항상 열림
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	bool bIsUnlocked = false;

	// true면 한번 열린 뒤 다시 닫히지 않음 (실제 닫기 로직은 BP가 이 값을 보고 판단)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	bool bStaysOpenForever = false;

	UPROPERTY(BlueprintReadOnly, Category = "Door")
	bool bIsOpen = false;

	// 스킬/마법으로 여는 조건 - 문마다 다르므로 BP에서 오버라이드
	UFUNCTION(BlueprintNativeEvent, Category = "Door|Condition")
	bool CheckSkillOrSpellCondition(AActor* Interactor) const;
	virtual bool CheckSkillOrSpellCondition_Implementation(AActor* Interactor) const { return false; }

	// 어느 쪽에서 접근했는지 - 문 배치/축 방향에 따라 다르므로 BP에서 오버라이드
	UFUNCTION(BlueprintNativeEvent, Category = "Door|Condition")
	EDoorSide GetInteractorSide(AActor* Interactor) const;
	virtual EDoorSide GetInteractorSide_Implementation(AActor* Interactor) const { return EDoorSide::Either; }

private:
	// DoorLeafRoot가 매 틱 이 각도를 향해 보간됨 (Open/Close에서 세팅)
	float TargetYaw = 0.0f;
};
