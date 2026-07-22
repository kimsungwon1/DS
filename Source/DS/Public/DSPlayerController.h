// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DSTarget.h"
#include "DSPlayerController.generated.h"

class APlayerPartyMover;
class UInputMappingContext;
class UInputAction;
class UDSUserWidget;
class UDSPlayerCharaData;
class UDSMainWidget;
class UPlayerCharacterInstanceComponent;
class ASelector;
class UDSAction;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class DS_API ADSPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void TransferCharacterToUI(int32 index, UPlayerCharacterInstanceComponent* data);

	UFUNCTION(BlueprintCallable)
	void BeginBattle();

	UFUNCTION(BlueprintCallable)
	void EndBattle();

	UFUNCTION(BlueprintCallable)
	void EnterHomeBase();

	UFUNCTION(BlueprintCallable)
	void ExitHomeBase();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	APlayerPartyMover* GetPlayerParty();

	UDSMainWidget* GetMainWidget() { return mainWidget; }

	ASelector* GetBaseSelector() { return selectors[0]; }

	void OnActionSelected(class UDSAction* action);

	virtual void Tick(float DeltaTime) override;

	/**
	 * 특정 액터에 카메라를 부드럽게 포커스합니다.
	 * @param Target      포커스할 대상
	 * @param Duration    포커스 유지 시간 (초)
	 * @param BlendTime   카메라 전환 블렌드 시간 (초)
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void FocusOnActor(AActor* Target, float Duration = 2.f, float BlendTime = 0.5f);

	/** 카메라를 원래 시점으로 복귀합니다. */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void ReturnCamera(float BlendSpeed = 0.f);

	UFUNCTION(BlueprintCallable)
	void EnableSelect(UDSAction* action, TSubclassOf<ASelector> selectorType = nullptr);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EnableTargetSelection(UDSAction* action);
	void EnableTargetSelection_Implementation(UDSAction* action);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DisableTargetSelection(bool bCloseActionWidgets = true);
	void DisableTargetSelection_Implementation(bool bCloseActionWidgets = true);
protected:
	void BeginPlay() override;

	virtual void SetupInputComponent() override;

	// 블루프린트에서 할당할 Input Assets
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;
	
	// DSPlayerController.h
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* TurnAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* CursorSwitchAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* RunShiftAction;

	// 주변 둘러보기
	void OnLookAround(const FInputActionValue& Value);

	// 움직이기.
	void OnMove(const FInputActionValue& Value);

	// 입력 처리 함수
	void OnJump(const FInputActionValue& Value);

	void OnCursorSwitch(const FInputActionValue& Value);

	void OnRunShift(const FInputActionValue& Value);

	void OnRunShiftFinished(const FInputActionValue& Value);

	// 매 틱 커서 밑 월드 오브젝트(문 등)를 감지해서 HoveredComponent를 갱신하고, 바뀌면 OnBeginCursorOver/OnEndCursorOver를
	// 컴포넌트+액터 양쪽에 수동으로 쏴줌. bEnableClickEvents/bEnableMouseOverEvents(엔진 자동 커서 트레이스)는
	// Trace Complex가 내부적으로 true 고정이라 콜리전에 따라 클릭이 씹히는 문제가 있어서 대신 이걸 씀.
	// bIsSelectingTarget이 true(Selector가 커서 쓰는 중)면 완전히 스킵해서 서로 안 겹치게 함.
	void UpdateWorldObjectHover();

	UFUNCTION(BlueprintNativeEvent)
	void OnClick();
	void OnClick_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void OnClickOff();
	void OnClickOff_Implementation() {}

	UFUNCTION(BlueprintCallable)
	void PartyMovableSwitch(bool bAble);

protected:
	UPROPERTY(BlueprintReadWrite)
	APlayerPartyMover* party = nullptr;

	UPROPERTY(BlueprintReadWrite)
	UDSMainWidget* mainWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCursorVisible = true;

	UPROPERTY(BlueprintReadOnly)
	bool bMovable = true;

	UPROPERTY(BlueprintReadOnly)
	bool bIsSelectingTarget = false;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UDSAction> reservedAction;
	
	// UPROPERTY(BlueprintReadOnly)
	// TObjectPtr<ASelector> baseSelector;

	UPROPERTY(BlueprintReadOnly)
	TArray<ASelector*> selectors;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ASelector> currentSelector;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASelector> baseSelectorCandidate;

	UPROPERTY()
	TObjectPtr<APostProcessVolume> TargetVolume;

private:
	// UpdateWorldObjectHover가 이번 틱 기준으로 커서 밑에 있다고 판단한 컴포넌트 (없으면 invalid)
	TWeakObjectPtr<class UPrimitiveComponent> HoveredComponent;

	// 위 컴포넌트를 맞춘 트레이스의 실제 충돌 지점 - 거리 체크(GetDefaultReachDistance 등)에 컴포넌트 원점 대신 이걸 씀
	FVector HoveredHitLocation = FVector::ZeroVector;

	bool bIsCameraFocused = false;
	bool bIsCameraReturning = false;
	FTimerHandle CameraReturnTimerHandle;

	TWeakObjectPtr<AActor> FocusTarget;
	FRotator FocusTargetRotation;
	FRotator FocusOriginalRotation;
	FRotator FocusCurrentVelocity = FRotator::ZeroRotator;
	float FocusHeadZOffset = 50.f;
	float FocusBlendSpeed = 5.f;
};
