#pragma once

#include "CoreMinimal.h"
#include "DSUserWidget.h"
#include "DSStats.h"
#include "DSPartyMemberWidget.generated.h"

class UPlayerCharacterInstanceComponent;
class UImage;
class UTextBlock;
class UProgressBar;
class UBorder;
class UOverlay;

/**
 * 파티 멤버 1명의 초상화 + HP/SP/MP 바를 표시하는 위젯
 * 하단 파티 패널에서 가로로 배치됨
 */
UCLASS()
class DS_API UDSPartyMemberWidget : public UDSUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetCharacterInstance(UPlayerCharacterInstanceComponent* InCharacter);

	UFUNCTION(BlueprintCallable)
	void RefreshUI();

	UFUNCTION(BlueprintCallable)
	void SetSelected(bool bSelected);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UPlayerCharacterInstanceComponent* GetCharacterInstance() const { return CharacterInstance; }

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// 캐릭터 사망 처리
	UFUNCTION(BlueprintNativeEvent)
	void OnCharacterDead();
	virtual void OnCharacterDead_Implementation();

protected:
	// 바인딩 - Blueprint에서 이름 맞춰 생성
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> PortraitImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CharacterNameText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ProfessionText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> HPBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> SPBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> HPText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> SPText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBorder> SelectionBorder;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UOverlay> DeadOverlay;

	// 선택 시 테두리 색상
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FLinearColor SelectedBorderColor = FLinearColor(1.f, 0.8f, 0.f, 1.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FLinearColor NormalBorderColor = FLinearColor(0.15f, 0.12f, 0.08f, 1.f);

	// HP 바 색상 (HP 비율에 따라 변화)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FLinearColor HPColorFull = FLinearColor(0.1f, 0.8f, 0.1f, 1.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FLinearColor HPColorLow = FLinearColor(0.8f, 0.1f, 0.1f, 1.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FLinearColor SPColor = FLinearColor(0.1f, 0.3f, 0.9f, 1.f);

private:
	UPROPERTY()
	TObjectPtr<UPlayerCharacterInstanceComponent> CharacterInstance;

	bool bIsSelected = false;
};
