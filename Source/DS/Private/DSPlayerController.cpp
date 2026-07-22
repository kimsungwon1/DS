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
#include "SpellCast.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/Application/SlateApplication.h"
#include "Components/PrimitiveComponent.h"
#include "DSBlueprintLibrary.h"
#include "Item.h"

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

	// post process volume 찾기
	{
		TArray<AActor*> FoundVolumes;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APostProcessVolume::StaticClass(), FoundVolumes);
		if (FoundVolumes.Num() > 0)
		{
			TargetVolume = Cast<APostProcessVolume>(FoundVolumes[0]);
		}
	}

	if (baseSelectorCandidate)
	{
		ASelector* baseSelector = GetWorld()->SpawnActor<ASelector>(baseSelectorCandidate, params);
		baseSelector->Initialize(this, TargetVolume);
		baseSelector->SetActorTickEnabled(false);
		
		selectors.Add(baseSelector);

		currentSelector = baseSelector;
	}
	else
	{
		ASelector* selector = GetWorld()->SpawnActor<ASelector>(params);
		selector->Initialize(this, TargetVolume);
		selector->SetActorTickEnabled(false);

		selectors.Add(selector);

		currentSelector = selector;
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

	// 엔진 자동 커서 트레이스(bEnableMouseOverEvents/bEnableClickEvents)는 내부적으로 Trace Complex가 true로
	// 고정돼있어서 콜리전에 구멍 있는 메시는 클릭이 씹힘 - 대신 UpdateWorldObjectHover()에서 Trace Complex=false로
	// 직접 트레이스해서 같은 네이티브 델리게이트(OnClicked/OnBeginCursorOver/OnEndCursorOver)를 수동으로 쏴줌
	bEnableMouseOverEvents = false;
	bEnableClickEvents = false;
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

void ADSPlayerController::EnterHomeBase()
{
	if (mainWidget)
	{
		mainWidget->EnterHomeBase();
	}

	PartyMovableSwitch(false);

	// EnableTargetSelection처럼 타겟팅 시작하는 건 아니고, 그냥 커서만 보이게
	if (!bIsCursorVisible)
	{
		OnCursorSwitch(FInputActionValue());
	}
}

void ADSPlayerController::ExitHomeBase()
{
	if (mainWidget)
	{
		mainWidget->ExitHomeBase();
	}

	PartyMovableSwitch(true);

	if (bIsCursorVisible)
	{
		OnCursorSwitch(FInputActionValue());
	}
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

	if (bIsCameraFocused)
		bIsCameraFocused = false;

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

	if (bIsCameraFocused)
		bIsCameraFocused = false;

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
	if (bIsSelectingTarget && reservedAction)
	{
		ESetTargetReturnType eSuccess = currentSelector->SetTargetForAction(reservedAction);

		switch (eSuccess)
		{
		case ESetTargetReturnType::Success:
			DisableTargetSelection();
			break;
		case ESetTargetReturnType::NoHit:

			break;
		case ESetTargetReturnType::TooFar:

			break;
		}
		return;
	}

	// 타겟 선택 중이 아니면, 이번 틱에 UpdateWorldObjectHover가 잡아둔 월드 오브젝트(문 등)에 클릭 전달
	if (HoveredComponent.IsValid())
	{
		AActor* HitActor = HoveredComponent->GetOwner();

		// AItem이면 그 액터의 GetDistanceToUse()(오버라이드 가능) 쓰고, 아니면 공용 기본값
		float ReachDistance = UDSBlueprintLibrary::GetDefaultReachDistance();
		if (AItem* HitItem = Cast<AItem>(HitActor))
		{
			ReachDistance = HitItem->GetDistanceToUse();
		}

		if (APawn* MyPawn = GetPawn())
		{
			// 컴포넌트 원점 대신 실제 클릭(트레이스 충돌) 지점을 씀 - 문처럼 긴 오브젝트는 원점이랑 꽤 떨어질 수 있음.
			// Dist2D(수평만)는 위/아래층처럼 Z가 많이 다른 대상도 가깝다고 오판할 수 있어서 그냥 3D 직선거리 씀
			const float Distance = FVector::Dist(MyPawn->GetActorLocation(), HoveredHitLocation);
			if (Distance > ReachDistance)
			{
				// 너무 멀어서 상호작용 안 함
				return;
			}
		}

		HoveredComponent->OnClicked.Broadcast(HoveredComponent.Get(), EKeys::LeftMouseButton);
		if (HitActor)
		{
			HitActor->OnClicked.Broadcast(HitActor, EKeys::LeftMouseButton);
		}
	}
}

void ADSPlayerController::UpdateWorldObjectHover()
{
	// Selector가 커서를 쓰는 중이면 완전히 스킵 - 서로 절대 안 겹치게
	if (bIsSelectingTarget)
	{
		if (HoveredComponent.IsValid())
		{
			HoveredComponent->OnEndCursorOver.Broadcast(HoveredComponent.Get());
			if (AActor* OldOwner = HoveredComponent->GetOwner())
			{
				OldOwner->OnEndCursorOver.Broadcast(OldOwner);
			}
			HoveredComponent.Reset();
		}
		return;
	}

	FHitResult Hit;
	const bool bHit = GetHitResultUnderCursor(ECC_Visibility, /*bTraceComplex=*/false, Hit);
	UPrimitiveComponent* NewHovered = bHit ? Hit.GetComponent() : nullptr;

	// 같은 컴포넌트를 계속 호버 중이어도 커서가 그 표면 위에서 움직였을 수 있으니 매 틱 갱신
	if (bHit)
	{
		HoveredHitLocation = Hit.ImpactPoint;
	}

	if (NewHovered == HoveredComponent.Get())
	{
		return;
	}

	if (HoveredComponent.IsValid())
	{
		HoveredComponent->OnEndCursorOver.Broadcast(HoveredComponent.Get());
		if (AActor* OldOwner = HoveredComponent->GetOwner())
		{
			OldOwner->OnEndCursorOver.Broadcast(OldOwner);
		}
	}

	if (NewHovered)
	{
		NewHovered->OnBeginCursorOver.Broadcast(NewHovered);
		if (AActor* NewOwner = NewHovered->GetOwner())
		{
			NewOwner->OnBeginCursorOver.Broadcast(NewOwner);
		}
	}

	HoveredComponent = NewHovered;
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
		if (mainWidget)
		{
			mainWidget->OpenSpellList(sc->GetActor(), sc);
		}
		return;
	}

	switch (action->GetType())
	{
		case EDSTargetType::Self:
			return;
		case EDSTargetType::Opponent:
			currentSelector = selectors[0];
			EnableTargetSelection(action);
			break;
		case EDSTargetType::OpponentParty:
			EnableTargetSelection(action);
		break;
		default:
			return;
	}
}

void ADSPlayerController::EnableTargetSelection_Implementation(UDSAction* action)
{
	bIsSelectingTarget = true;
	if (!bIsCursorVisible)
	{
		OnCursorSwitch(FInputActionValue());
	}

	currentSelector->Initialize(this, TargetVolume, action);
	// currentSelector->SetActorTickEnabled(true);
}

void ADSPlayerController::FocusOnActor(AActor* Target, float Duration, float BlendSpeed)
{
	if (!Target) return;

	GetWorldTimerManager().ClearTimer(CameraReturnTimerHandle);

	FVector CamLoc;
	FRotator CamRot;
	GetPlayerViewPoint(CamLoc, CamRot);

	FocusTarget = Target;

	FocusHeadZOffset = 50.f;
	if (USkeletalMeshComponent* Mesh = Target->FindComponentByClass<USkeletalMeshComponent>())
	{
		FVector HeadPos = Mesh->GetSocketLocation(FName("head"));
		if (!HeadPos.IsZero())
			FocusHeadZOffset = HeadPos.Z - Target->GetActorLocation().Z;
	}

	if (!bIsCameraFocused)
	{
		FocusOriginalRotation = CamRot;
		FocusCurrentVelocity = FRotator::ZeroRotator;
	}
	FocusBlendSpeed = BlendSpeed;
	bIsCameraFocused = true;
	bIsCameraReturning = false;
}

void ADSPlayerController::ReturnCamera(float BlendSpeed)
{
	GetWorldTimerManager().ClearTimer(CameraReturnTimerHandle);

	FocusTarget.Reset();
	FocusTargetRotation = FocusOriginalRotation;
	if (BlendSpeed > 0.f)
		FocusBlendSpeed = BlendSpeed;
	FocusCurrentVelocity = FRotator::ZeroRotator;
	bIsCameraFocused = true;
	bIsCameraReturning = true;
}

void ADSPlayerController::EnableSelect(UDSAction* action, TSubclassOf<ASelector> selectorType)
{
	// 나중에 selectorType을 통해 그 타입의 셀렉터가 목록에 있는지 확인한 다음 그거 쓰도록 하기.
	if (currentSelector) 
	{
		currentSelector->SetActorTickEnabled(false);
	}
	currentSelector = nullptr;

	if (selectorType == nullptr)
	{
		selectorType = baseSelectorCandidate;
	}

	for (ASelector* selector : selectors)
	{
		if (selector->GetClass() == selectorType)
		{
			currentSelector = selector;
			currentSelector->Initialize(this, TargetVolume, action);
			// currentSelector->SetActorTickEnabled(true);
			
		}
	}
	if (currentSelector == nullptr)
	{
		ASelector* sel = GetWorld()->SpawnActor<ASelector>(selectorType);
		sel->Initialize(this, TargetVolume, action);

		currentSelector = sel;

		selectors.Add(sel);
	}

	EnableTargetSelection(action);
}

void ADSPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateWorldObjectHover();

	if (!bIsCameraFocused) return;

	// 포커스 중이면 매 프레임 타겟 방향 갱신
	if (!bIsCameraReturning && FocusTarget.IsValid())
	{
		FVector CamLoc;
		FRotator CamRot;
		GetPlayerViewPoint(CamLoc, CamRot);

		const FVector LookAtPos = FocusTarget->GetActorLocation() + FVector(0.f, 0.f, FocusHeadZOffset);
		FocusTargetRotation = (LookAtPos - CamLoc).Rotation();
	}

	// 가속도 느낌: 속도를 매 프레임 증가시켜서 RInterpTo에 넘김
	FocusCurrentVelocity.Yaw = FMath::Min(FocusCurrentVelocity.Yaw + DeltaTime * FocusBlendSpeed * 40.f, FocusBlendSpeed * 12.f);
	const FRotator NewRot = FMath::RInterpTo(GetControlRotation(), FocusTargetRotation, DeltaTime, FocusCurrentVelocity.Yaw);
	SetControlRotation(NewRot);

	// 복귀 중이고 목표에 충분히 가까워지면 포커스 종료
	if (bIsCameraReturning)
	{
		if (FocusTargetRotation.Equals(GetControlRotation(), 0.5f))
			bIsCameraFocused = false;
	}
}

void ADSPlayerController::DisableTargetSelection_Implementation(bool bCloseActionWidgets)
{
	bIsSelectingTarget = false;

	if (currentSelector)
	{
		currentSelector->Empty();
		currentSelector->SetActorTickEnabled(false);
	}

	if (bCloseActionWidgets && mainWidget)
	{
		mainWidget->ActionSelectDone();
	}
}
