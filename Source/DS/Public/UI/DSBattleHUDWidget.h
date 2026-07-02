#pragma once

#include "CoreMinimal.h"
#include "DSMainWidget.h"
#include "DSBattleHUDWidget.generated.h"

class UPlayerCharacterInstanceComponent;
class UCharacterInstanceComponent;
class UDSPartyMemberWidget;
class UDSActionPanelWidget;
class UDSEnemyInfoWidget;
class UHorizontalBox;
class UVerticalBox;
class UTextBlock;
class UBorder;
class UOverlay;
class UCanvasPanel;

/**
 * D&D 풍 파티 RPG 메인 HUD
 *
 * 레이아웃:
 * ┌─────────────────────────────────────┐
 * │  [적 정보 바 - 상단]                 │
 * ├──────────────────────┬──────────────┤
 * │                      │  행동 패널   │
 * │   전투 뷰포트         │  (우측)      │
 * │   (중앙)             │              │
 * │                      │              │
 * ├──────────────────────┴──────────────┤
 * │  [파티 멤버 패널 - 하단 가로 정렬]   │
 * │  [캐1][캐2][캐3][캐4][캐5][캐6]     │
 * └─────────────────────────────────────┘
 */
UCLASS()
class DS_API UDSBattleHUDWidget : public UDSMainWidget
{
	GENERATED_BODY()

public:
	// UDSMainWidget interface
	virtual void SetCharacter_Implementation(int32 index, UPlayerCharacterInstanceComponent* playerInstance);
	virtual void BeginBattle_Implementation();
	virtual void EndBattle_Implementation();

	UFUNCTION(BlueprintCallable)
	void SetEnemyTarget(UCharacterInstanceComponent* Enemy);

	UFUNCTION(BlueprintCallable)
	void SelectPartyMember(int32 Index);

	UFUNCTION(BlueprintCallable)
	void SetCombatPaused(bool bPaused);

	UFUNCTION(BlueprintCallable)
	void AddCombatLog(const FString& Message);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// 파티 멤버 클릭 처리
	UFUNCTION()
	void HandlePartyMemberClicked(int32 Index);

private:
	void RefreshAllMemberWidgets();
	void UpdateCombatLog();

protected:
	// ── 상단 적 정보 ──
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UDSEnemyInfoWidget> EnemyInfoWidget;

	// ── 우측 행동 패널 ──
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UDSActionPanelWidget> ActionPanelWidget;

	// ── 하단 파티 패널 ──
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> PartyMemberBox;

	// ── 전투 상태 텍스트 (Combat Paused 등) ──
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CombatStatusText;

	// ── 전투 로그 ──
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CombatLogText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBorder> CombatLogBorder;

	// 파티 멤버 위젯 클래스 (BP에서 지정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UDSPartyMemberWidget> PartyMemberWidgetClass;

	// 전투 로그 최대 줄 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Log")
	int32 MaxLogLines = 5;

	// 파티 멤버 최대 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Party")
	int32 MaxPartySize = 6;

private:
	UPROPERTY()
	TArray<TObjectPtr<UDSPartyMemberWidget>> MemberWidgets;

	TArray<FString> CombatLogLines;

	int32 SelectedMemberIndex = 0;
};
