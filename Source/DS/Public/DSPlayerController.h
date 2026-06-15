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

	void OnActionSelected(class UDSAction* action);
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
};
