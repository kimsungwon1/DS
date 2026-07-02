// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DSTarget.h"
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

	UFUNCTION(BlueprintCallable, BlueprintPure)
	APlayerPartyMover* GetPlayerParty();

	UDSMainWidget* GetMainWidget() { return mainWidget; }

	ASelector* GetBaseSelector() { return baseSelector; }

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

	UFUNCTION(BlueprintNativeEvent)
	void OnClick();
	void OnClick_Implementation();

	UFUNCTION(BlueprintCallable)
	void PartyMovableSwitch(bool bAble);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EnableTargetSelection();
	void EnableTargetSelection_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DisableTargetSelection();
	void DisableTargetSelection_Implementation();
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
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ASelector> baseSelector;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ASelector> currentSelector;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<ASelector> baseSelectorCandidate;

private:
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
