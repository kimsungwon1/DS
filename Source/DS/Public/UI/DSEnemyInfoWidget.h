#pragma once

#include "CoreMinimal.h"
#include "DSUserWidget.h"
#include "DSEnemyInfoWidget.generated.h"

class UCharacterInstanceComponent;
class UTextBlock;
class UProgressBar;
class UImage;
class UHorizontalBox;

/**
 * 전투 중 적 정보를 상단에 표시하는 위젯
 * 적 이름, HP, 아이콘 등
 */
UCLASS()
class DS_API UDSEnemyInfoWidget : public UDSUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetEnemyInstance(UCharacterInstanceComponent* InEnemy);

	UFUNCTION(BlueprintCallable)
	void RefreshUI();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> EnemyNameText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> EnemyHPBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> EnemyHPText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FLinearColor EnemyHPColor = FLinearColor(0.85f, 0.1f, 0.1f, 1.f);

private:
	UPROPERTY()
	TObjectPtr<UCharacterInstanceComponent> EnemyInstance;
};
