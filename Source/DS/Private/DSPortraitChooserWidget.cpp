
#include "DSPortraitChooserWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Engine/Texture2D.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"

void UDSPortraitChooserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
	{
		return;
	}

	if (PortraitFolder.Path.IsEmpty())
	{
		PortraitFolder.Path = TEXT("/Game/Made/Players/Portraits");
	}

	// 실제 텍스처 씌우기 전, 디자이너에 세팅해둔 기본 브러시(해골 등)를 캐싱 - 선택 초기화 시 이걸로 복원
	if (IsValid(PortraitImage))
	{
		DefaultPortraitBrush = PortraitImage->GetBrush();
	}

	if (IsValid(PrevButton))
	{
		PrevButton->OnClicked.RemoveDynamic(this, &UDSPortraitChooserWidget::HandlePrevClicked);
		PrevButton->OnClicked.AddDynamic(this, &UDSPortraitChooserWidget::HandlePrevClicked);
	}
	if (IsValid(NextButton))
	{
		NextButton->OnClicked.RemoveDynamic(this, &UDSPortraitChooserWidget::HandleNextClicked);
		NextButton->OnClicked.AddDynamic(this, &UDSPortraitChooserWidget::HandleNextClicked);
	}

	RefreshPortraitList();
	CurrentIndex = INDEX_NONE;
	UpdateDisplay();
}

void UDSPortraitChooserWidget::RefreshPortraitList()
{
	Portraits.Empty();

	IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();

	TArray<FAssetData> FoundAssets;
	AssetRegistry.GetAssetsByPath(FName(*PortraitFolder.Path), FoundAssets, /*bRecursive=*/true);

	FoundAssets.Sort([](const FAssetData& A, const FAssetData& B)
	{
		return A.AssetName.LexicalLess(B.AssetName);
	});

	for (const FAssetData& Asset : FoundAssets)
	{
		if (Asset.AssetClassPath == UTexture2D::StaticClass()->GetClassPathName())
		{
			if (UTexture2D* Texture = Cast<UTexture2D>(Asset.GetAsset()))
			{
				Portraits.Add(Texture);
			}
		}
	}
}

void UDSPortraitChooserWidget::CyclePortrait(int32 Direction)
{
	if (Portraits.Num() <= 0) return;

	if (CurrentIndex == INDEX_NONE)
	{
		// 선택 없음 상태에서 처음 누르면 방향에 맞는 쪽 끝에서 시작
		CurrentIndex = (Direction >= 0) ? 0 : Portraits.Num() - 1;
	}
	else
	{
		CurrentIndex = (CurrentIndex + Direction + Portraits.Num()) % Portraits.Num();
	}
	UpdateDisplay();
}

void UDSPortraitChooserWidget::UpdateDisplay()
{
	if (CurrentIndex == INDEX_NONE)
	{
		// 선택 없음 - 디자이너 기본 브러시(해골 등)로 복원하고 선택 없음을 알림
		if (PortraitImage)
		{
			PortraitImage->SetBrush(DefaultPortraitBrush);
		}
		OnPortraitChanged.Broadcast(nullptr);
		return;
	}

	if (!Portraits.IsValidIndex(CurrentIndex)) return;

	UTexture2D* Current = Portraits[CurrentIndex];
	if (PortraitImage && Current)
	{
		PortraitImage->SetBrushFromTexture(Current);
	}

	OnPortraitChanged.Broadcast(Current);
}

UTexture2D* UDSPortraitChooserWidget::GetSelectedPortrait() const
{
	return Portraits.IsValidIndex(CurrentIndex) ? Portraits[CurrentIndex].Get() : nullptr;
}

void UDSPortraitChooserWidget::ResetSelection()
{
	CurrentIndex = INDEX_NONE;
	UpdateDisplay();
}

void UDSPortraitChooserWidget::SelectPortrait(UTexture2D* Portrait)
{
	if (!Portrait) return;

	const int32 FoundIndex = Portraits.IndexOfByKey(Portrait);
	if (FoundIndex != INDEX_NONE)
	{
		CurrentIndex = FoundIndex;
	}

	// 목록에서 정확히 못 찾았어도(포인터가 다른 로드 인스턴스인 경우 등) 일단 화면엔 그 텍스처를 그대로 보여줌
	if (PortraitImage)
	{
		PortraitImage->SetBrushFromTexture(Portrait);
	}
	OnPortraitChanged.Broadcast(Portrait);
}

void UDSPortraitChooserWidget::SetInteractable(bool bInteractable)
{
	if (IsValid(PrevButton))
	{
		PrevButton->SetIsEnabled(bInteractable);
	}
	if (IsValid(NextButton))
	{
		NextButton->SetIsEnabled(bInteractable);
	}
}

void UDSPortraitChooserWidget::HandlePrevClicked()
{
	CyclePortrait(-1);
}

void UDSPortraitChooserWidget::HandleNextClicked()
{
	CyclePortrait(1);
}
