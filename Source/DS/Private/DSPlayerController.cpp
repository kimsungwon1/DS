// Fill out your copyright notice in the Description page of Project Settings.


#include "DSPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "PlayerPartyMover.h"
#include "DSMainWidget.h"
#include "PlayerCharacterInstanceComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Selector.h"
#include "ActionList.h"

void ADSPlayerController::TransferCharacterToUI(int32 index, UPlayerCharacterInstanceComponent* data)
{
	if (mainWidget)
	{
		mainWidget->SetCharacter(index, data);
	}

	PartyMovableSwitch(true);
}

void ADSPlayerController::BeginPlay()
{
	APlayerController::BeginPlay();

	// SetupInputComponent();
	FActorSpawnParameters params;

	if (baseSelectorCandidate.IsNull())
	{
		baseSelector = GetWorld()->SpawnActor<ASelector>(params);
		baseSelector->Initialize(this);
		baseSelector->SetActorTickEnabled(false);
	}
	else
	{
		UClass* loadedClass = baseSelectorCandidate.LoadSynchronous();
		
		baseSelector = GetWorld()->SpawnActor<ASelector>(loadedClass);
		baseSelector->Initialize(this);
		baseSelector->SetActorTickEnabled(false);
	}

	bIsCursorVisible = false;

	OnCursorSwitch(FInputActionValue());

	// 💡 마우스 모드를 정밀하게 세팅
	FInputModeGameAndUI InputMode;

	// 핵심 1: 마우스를 클릭(캡처)할 때 커서를 절대 숨기지 마라!
	InputMode.SetHideCursorDuringCapture(false);

	// // 핵심 2: 마우스가 뷰포트(게임 화면) 밖으로 탈출하지 못하게 가두기 (선택 사항)
	// InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreenOrWindowed);

	// 핵심 3: 포커스를 잡을 타깃 UI가 있다면 지정 (없다면 pass)
	// InputMode.SetWidgetToFocus(YourWidgetRef); 

	SetInputMode(InputMode);

	// Enhanced Input Subsystem 가져오기
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (PlayerMappingContext)
			{
				Subsystem->AddMappingContext(PlayerMappingContext, 0);
			}
		}
	}
}

APlayerPartyMover* ADSPlayerController::GetPlayerParty()
{
	APlayerPartyMover* retParty = party;
	if (retParty == nullptr) {
		party = Cast<APlayerPartyMover>(GetPawn());
		retParty = party;
	}

	return retParty;
}

void ADSPlayerController::BeginBattle()
{
	if (mainWidget)
	{
		mainWidget->BeginBattle();
	}

	PartyMovableSwitch(false);
}

void ADSPlayerController::EndBattle()
{
	if (mainWidget)
	{
		mainWidget->EndBattle();
	}

	PartyMovableSwitch(true);
}

void ADSPlayerController::SetupInputComponent()
{
	APlayerController::SetupInputComponent();

	if (!InputComponent)
	{
		InputComponent = NewObject<UEnhancedInputComponent>(this, TEXT("EnhancedInputComponent"));
		InputComponent->RegisterComponent();
	}

	InputComponent->BindKey(EKeys::LeftMouseButton, IE_Pressed, this, &ADSPlayerController::OnClick);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (JumpAction)
		{
			EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &ADSPlayerController::OnJump);
		}
		if (MoveAction)
		{
			EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADSPlayerController::OnMove);
		}
		if (TurnAction)
		{
			EIC->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ADSPlayerController::OnLookAround);
		}
		if (CursorSwitchAction)
		{
			EIC->BindAction(CursorSwitchAction, ETriggerEvent::Started, this, &ADSPlayerController::OnCursorSwitch);
			EIC->BindAction(CursorSwitchAction, ETriggerEvent::Completed, this, &ADSPlayerController::OnCursorSwitch);
		}
		if (RunShiftAction)
		{
			EIC->BindAction(RunShiftAction, ETriggerEvent::Started, this, &ADSPlayerController::OnRunShift);
			EIC->BindAction(RunShiftAction, ETriggerEvent::Completed, this, &ADSPlayerController::OnRunShiftFinished);
		}
	}
}

void ADSPlayerController::OnLookAround(const FInputActionValue& Value)
{
	if (bIsCursorVisible) {
		return;
	}
	FVector2D TurnVector = Value.Get<FVector2D>();

	if (auto* playerParty = GetPlayerParty()) {
		playerParty->TurnAround(TurnVector);
	}
}

void ADSPlayerController::OnMove(const FInputActionValue& Value)
{
	if (!bMovable)
	{
		return;
	}

	FVector2D MovementVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn())
	{
		// 앞뒤 이동
		ControlledPawn->AddMovementInput(ControlledPawn->GetActorForwardVector(), MovementVector.X);

		// 좌우 이동
		ControlledPawn->AddMovementInput(ControlledPawn->GetActorRightVector(), MovementVector.Y);
	}
}

void ADSPlayerController::OnJump(const FInputActionValue& Value)
{
	if (auto* MyCharacter = GetPlayerParty())
	{
		MyCharacter->Jump();
	}
}

void ADSPlayerController::OnCursorSwitch(const FInputActionValue& Value)
{
	bIsCursorVisible = !bIsCursorVisible;

	if (bIsCursorVisible) {
		bShowMouseCursor = true;

		FInputModeGameAndUI fInput;
		fInput.SetHideCursorDuringCapture(false);
		SetInputMode(fInput);
		SetIgnoreLookInput(true);

		UE_LOG(LogTemp, Log, TEXT("On CursorSwitch On"));
	}
	else
	{
		bShowMouseCursor = false;

		SetInputMode(FInputModeGameOnly());
		SetIgnoreLookInput(false);

		UE_LOG(LogTemp, Log, TEXT("On CursorSwitch Off"));
	}
}

void ADSPlayerController::OnRunShift(const FInputActionValue& Value)
{
	auto* player = GetPlayerParty();
	player->GetCharacterMovement()->MaxWalkSpeed = 1200;
}

void ADSPlayerController::OnRunShiftFinished(const FInputActionValue& Value)
{
	auto* player = GetPlayerParty();
	player->GetCharacterMovement()->MaxWalkSpeed = 600;
}

void ADSPlayerController::PartyMovableSwitch(bool bAble)
{
	GetPawn()->bUseControllerRotationYaw = bAble;

	bMovable = bAble;
}

void ADSPlayerController::OnClick_Implementation()
{
	if (bIsSelectingTarget)
	{
		ESetTargetReturnType eSuccess = currentSelector->SetTargetForAction(reservedAction);

		DisableTargetSelection();
	}
}

void ADSPlayerController::OnActionSelected(UDSAction* action)
{
	reservedAction = action;

	if (action->IsA<UUseItem>())
	{
		UUseItem* useItem = Cast<UUseItem>(action);

	}
	else if (action->IsA<USpellCast>())
	{
		USpellCast* sc = Cast<USpellCast>(action);

	}

	switch (action->GetType())
	{
		case EDSTargetType::Self:
			return;
		case EDSTargetType::Opponent:
			currentSelector = baseSelector;
			EnableTargetSelection();
			break;
		case EDSTargetType::OpponentParty:
			EnableTargetSelection();
		break;
		default:
			return;
	}
}

void ADSPlayerController::EnableTargetSelection_Implementation()
{
	bIsSelectingTarget = true;
	if (!bIsCursorVisible)
	{
		OnCursorSwitch(FInputActionValue());
	}

	currentSelector->SetActorTickEnabled(true);
}

void ADSPlayerController::DisableTargetSelection_Implementation()
{
	bIsSelectingTarget = false;

	currentSelector->Empty();
	currentSelector->SetActorTickEnabled(false);
}
