#include "UI/DSEnemyInfoWidget.h"
#include "CharacterInstanceComponent.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UDSEnemyInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (EnemyHPBar)
		EnemyHPBar->SetFillColorAndOpacity(EnemyHPColor);
}

void UDSEnemyInfoWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (EnemyInstance)
		RefreshUI();
}

void UDSEnemyInfoWidget::SetEnemyInstance(UCharacterInstanceComponent* InEnemy)
{
	EnemyInstance = InEnemy;
	RefreshUI();
}

void UDSEnemyInfoWidget::RefreshUI()
{
	if (!EnemyInstance) return;

	const FFaceStat& Face = EnemyInstance->GetFinalFaceStat();
	const float HPRatio = (Face.MaxHP > 0) ? FMath::Clamp((float)Face.Health / Face.MaxHP, 0.f, 1.f) : 0.f;

	if (EnemyNameText)
		EnemyNameText->SetText(FText::FromName(EnemyInstance->characterName));

	if (EnemyHPBar)
		EnemyHPBar->SetPercent(HPRatio);

	if (EnemyHPText)
		EnemyHPText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), Face.Health, Face.MaxHP)));
}
