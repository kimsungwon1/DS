#include "UI/DSBattleHUDWidget.h"
#include "UI/DSPartyMemberWidget.h"
#include "UI/DSActionPanelWidget.h"
#include "UI/DSEnemyInfoWidget.h"
#include "PlayerCharacterInstanceComponent.h"
#include "CharacterInstanceComponent.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"

void UDSBattleHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CombatStatusText)
		CombatStatusText->SetVisibility(ESlateVisibility::Collapsed);

	if (CombatLogBorder)
		CombatLogBorder->SetVisibility(ESlateVisibility::Collapsed);
}

void UDSBattleHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UDSBattleHUDWidget::SetCharacter_Implementation(int32 index, UPlayerCharacterInstanceComponent* playerInstance)
{
	Super::SetCharacter_Implementation(index, playerInstance);

	// characters 배열 크기 맞추기
	while (MemberWidgets.Num() <= index)
		MemberWidgets.Add(nullptr);

	if (!PartyMemberBox || !PartyMemberWidgetClass) return;

	// 이미 위젯이 있으면 데이터만 갱신
	if (MemberWidgets[index])
	{
		MemberWidgets[index]->SetCharacterInstance(playerInstance);
		return;
	}

	UDSPartyMemberWidget* Widget = CreateWidget<UDSPartyMemberWidget>(this, PartyMemberWidgetClass);
	if (!Widget) return;

	Widget->SetCharacterInstance(playerInstance);
	MemberWidgets[index] = Widget;

	UHorizontalBoxSlot* HSlot = PartyMemberBox->AddChildToHorizontalBox(Widget);
	if (HSlot)
	{
		HSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		HSlot->SetPadding(FMargin(2.f, 0.f));
	}

	// 첫 멤버 자동 선택
	if (index == 0)
		SelectPartyMember(0);
}

void UDSBattleHUDWidget::BeginBattle_Implementation()
{
	if (CombatStatusText)
	{
		CombatStatusText->SetText(FText::FromString(TEXT("Combat Paused")));
		CombatStatusText->SetVisibility(ESlateVisibility::Visible);
	}

	if (CombatLogBorder)
		CombatLogBorder->SetVisibility(ESlateVisibility::Visible);

	CombatLogLines.Empty();
	AddCombatLog(TEXT("전투 시작!"));
}

void UDSBattleHUDWidget::EndBattle_Implementation()
{
	if (CombatStatusText)
		CombatStatusText->SetVisibility(ESlateVisibility::Collapsed);

	if (CombatLogBorder)
		CombatLogBorder->SetVisibility(ESlateVisibility::Collapsed);
}

void UDSBattleHUDWidget::SetEnemyTarget(UCharacterInstanceComponent* Enemy)
{
	if (EnemyInfoWidget)
		EnemyInfoWidget->SetEnemyInstance(Enemy);
}

void UDSBattleHUDWidget::SelectPartyMember(int32 Index)
{
	if (!MemberWidgets.IsValidIndex(Index) || !MemberWidgets[Index])
		return;

	// 기존 선택 해제
	if (MemberWidgets.IsValidIndex(SelectedMemberIndex) && MemberWidgets[SelectedMemberIndex])
		MemberWidgets[SelectedMemberIndex]->SetSelected(false);

	SelectedMemberIndex = Index;
	MemberWidgets[Index]->SetSelected(true);

	// 행동 패널 갱신
	if (ActionPanelWidget)
		ActionPanelWidget->SetActiveCharacter(MemberWidgets[Index]->GetCharacterInstance());
}

void UDSBattleHUDWidget::SetCombatPaused(bool bPaused)
{
	if (CombatStatusText)
	{
		CombatStatusText->SetText(FText::FromString(bPaused ? TEXT("Combat Paused") : TEXT("")));
		CombatStatusText->SetVisibility(bPaused ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UDSBattleHUDWidget::AddCombatLog(const FString& Message)
{
	CombatLogLines.Add(Message);

	// 최대 줄 수 초과 시 오래된 것 제거
	while (CombatLogLines.Num() > MaxLogLines)
		CombatLogLines.RemoveAt(0);

	UpdateCombatLog();
}

void UDSBattleHUDWidget::HandlePartyMemberClicked(int32 Index)
{
	SelectPartyMember(Index);
}

void UDSBattleHUDWidget::RefreshAllMemberWidgets()
{
	for (UDSPartyMemberWidget* Widget : MemberWidgets)
	{
		if (Widget)
			Widget->RefreshUI();
	}
}

void UDSBattleHUDWidget::UpdateCombatLog()
{
	if (!CombatLogText) return;

	FString Combined;
	for (const FString& Line : CombatLogLines)
		Combined += Line + TEXT("\n");

	CombatLogText->SetText(FText::FromString(Combined.TrimEnd()));
}
