
#pragma once

#include "CoreMinimal.h"
#include "DSUserWidget.h"
#include "Styling/SlateBrush.h"
#include "DSPortraitChooserWidget.generated.h"

class UImage;
class UButton;
class UTexture2D;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPortraitChanged, UTexture2D*, NewPortrait);

// /Game/Made/Players/Portraits/ 안의 모든 텍스처를 좌우 버튼으로 순환시키는 위젯
UCLASS()
class DS_API UDSPortraitChooserWidget : public UDSUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTexture2D* GetSelectedPortrait() const;

	// 선택 없음 상태로 되돌림 - 디자이너에 세팅해둔 기본(해골 등) 브러시로 복원됨 (캐릭터 생성 취소/완료 후 다시 시작할 때 사용)
	UFUNCTION(BlueprintCallable)
	void ResetSelection();

	// 목록에서 특정 텍스처를 찾아서 그걸로 선택 (캐릭터 리뷰 화면에서 그 캐릭터의 실제 초상화를 보여줄 때 사용)
	// 목록에 없으면(원본 애셋이 지워졌거나 등) 무시하고 현재 선택 유지
	UFUNCTION(BlueprintCallable)
	void SelectPortrait(UTexture2D* Portrait);

	// 좌우 버튼을 눌러도 반응 안 하게 (읽기 전용 화면 등에서 사용)
	UFUNCTION(BlueprintCallable)
	void SetInteractable(bool bInteractable);

	UPROPERTY(BlueprintAssignable)
	FOnPortraitChanged OnPortraitChanged;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> PortraitImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> PrevButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> NextButton;

	// 초상화를 긁어올 폴더
	UPROPERTY(EditDefaultsOnly, Category = "Portrait")
	FDirectoryPath PortraitFolder;

private:
	void RefreshPortraitList();
	void CyclePortrait(int32 Direction);
	void UpdateDisplay();

	UFUNCTION() void HandlePrevClicked();
	UFUNCTION() void HandleNextClicked();

	UPROPERTY()
	TArray<TObjectPtr<UTexture2D>> Portraits;

	// INDEX_NONE(-1)이면 "선택 없음" - 디자이너 기본 브러시(해골 등) 표시 상태
	int32 CurrentIndex = INDEX_NONE;

	// PortraitImage에 아직 아무 텍스처도 안 씌운 시점(NativeConstruct 초입)의 기본 브러시 - 선택 초기화 시 복원용
	FSlateBrush DefaultPortraitBrush;
};
