
#include "DSGatherPartyWidget.h"
#include "DSCharacterSaveEntryWidget.h"
#include "DSMessageBoxYesNoWidget.h"
#include "DSMessageBoxWidget.h"
#include "DSSaveGameSubsystem.h"
#include "PlayerCharacterInstanceComponent.h"
#include "PlayerPartyManagerComponent.h"
#include "PlayerPartyMover.h"
#include "DSPlayerParty.h"
#include "DSPlayerController.h"
#include "DSGameMode.h"
#include "Components/ScrollBox.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

namespace
{
	FString GetProfessionDisplayNameLocal_GatherParty(EProfession Value)
	{
		const UEnum* EnumPtr = StaticEnum<EProfession>();
		return EnumPtr ? EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(Value)).ToString() : FString();
	}

	FString GetRaceDisplayNameLocal_GatherParty(ERace Value)
	{
		const UEnum* EnumPtr = StaticEnum<ERace>();
		return EnumPtr ? EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(Value)).ToString() : FString();
	}
}

UDSGatherPartyWidget::UDSGatherPartyWidget()
{
	static ConstructorHelpers::FClassFinder<UDSCharacterSaveEntryWidget> EntryClassFinder(TEXT("/Game/Made/UI/CharacterUi/DSG_CharacterSaveEntry"));
	if (EntryClassFinder.Succeeded())
	{
		EntryWidgetClass = EntryClassFinder.Class;
	}
}

void UDSGatherPartyWidget::SelectPartySlot(int32 PartyIndex)
{
	UPlayerCharacterInstanceComponent* Member = GetPartyMember(PartyIndex);
	if (!Member) return;

	Btn_RemoveFromParty1->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	SelectedPartySlotIndex = PartyIndex;

	// "선택된 캐릭터"를 이 파티원으로 갱신 - Review Character 등이 로스터가 아니라 이 캐릭터를 보게 됨
	SelectedCharacterData = Member->ToSaveData();
	SelectedCharacterName = SelectedCharacterData.characterName;
	ApplyCharacterToCenterColumn(SelectedCharacterData);
}

void UDSGatherPartyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
	{
		return;
	}

	if (IsValid(PartyMember1))
	{
		PartyMember1->OnClicked.RemoveDynamic(this, &UDSGatherPartyWidget::OnPartyMember1Clicked);
		PartyMember1->OnClicked.AddDynamic(this, &UDSGatherPartyWidget::OnPartyMember1Clicked);
	}
	if (IsValid(PartyMember2))
	{
		PartyMember2->OnClicked.RemoveDynamic(this, &UDSGatherPartyWidget::OnPartyMember2Clicked);
		PartyMember2->OnClicked.AddDynamic(this, &UDSGatherPartyWidget::OnPartyMember2Clicked);
	}
	if (IsValid(PartyMember3))
	{
		PartyMember3->OnClicked.RemoveDynamic(this, &UDSGatherPartyWidget::OnPartyMember3Clicked);
		PartyMember3->OnClicked.AddDynamic(this, &UDSGatherPartyWidget::OnPartyMember3Clicked);
	}
	if (IsValid(PartyMember4))
	{
		PartyMember4->OnClicked.RemoveDynamic(this, &UDSGatherPartyWidget::OnPartyMember4Clicked);
		PartyMember4->OnClicked.AddDynamic(this, &UDSGatherPartyWidget::OnPartyMember4Clicked);
	}
	if (IsValid(PartyMember5))
	{
		PartyMember5->OnClicked.RemoveDynamic(this, &UDSGatherPartyWidget::OnPartyMember5Clicked);
		PartyMember5->OnClicked.AddDynamic(this, &UDSGatherPartyWidget::OnPartyMember5Clicked);
	}
	if (IsValid(PartyMember6))
	{
		PartyMember6->OnClicked.RemoveDynamic(this, &UDSGatherPartyWidget::OnPartyMember6Clicked);
		PartyMember6->OnClicked.AddDynamic(this, &UDSGatherPartyWidget::OnPartyMember6Clicked);
	}
	if (IsValid(Btn_RemoveFromParty1))
	{
		Btn_RemoveFromParty1->OnClicked.RemoveDynamic(this, &UDSGatherPartyWidget::RequestRemoveFromParty);
		Btn_RemoveFromParty1->OnClicked.AddDynamic(this, &UDSGatherPartyWidget::RequestRemoveFromParty);
	}

	// 초상화 씌우기 전 원래(해골 등 기본) 스타일을 슬롯마다 캐싱 - 파티에서 뺄 때 이걸로 복원
	if (DefaultSlotStyles.Num() == 0)
	{
		DefaultSlotStyles.SetNum(6);
		for (int32 i = 0; i < 6; ++i)
		{
			if (UButton* SlotButton = GetSlotButton(i))
			{
				DefaultSlotStyles[i] = SlotButton->GetStyle();
			}
		}
	}

	// 이미 파티에 있는 라이브 캐릭터들 초상화 반영 (홈베이스 복귀 등)
	for (int32 i = 0; i < 6; ++i)
	{
		RefreshSlotImage(i);
	}

	RefreshRosterList();
}

void UDSGatherPartyWidget::NativeDestruct()
{
	if (IsValid(PartyMember1)) PartyMember1->OnClicked.RemoveDynamic(this, &UDSGatherPartyWidget::OnPartyMember1Clicked);
	if (IsValid(PartyMember2)) PartyMember2->OnClicked.RemoveDynamic(this, &UDSGatherPartyWidget::OnPartyMember2Clicked);
	if (IsValid(PartyMember3)) PartyMember3->OnClicked.RemoveDynamic(this, &UDSGatherPartyWidget::OnPartyMember3Clicked);
	if (IsValid(PartyMember4)) PartyMember4->OnClicked.RemoveDynamic(this, &UDSGatherPartyWidget::OnPartyMember4Clicked);
	if (IsValid(PartyMember5)) PartyMember5->OnClicked.RemoveDynamic(this, &UDSGatherPartyWidget::OnPartyMember5Clicked);
	if (IsValid(PartyMember6)) PartyMember6->OnClicked.RemoveDynamic(this, &UDSGatherPartyWidget::OnPartyMember6Clicked);
	if (IsValid(Btn_RemoveFromParty1)) Btn_RemoveFromParty1->OnClicked.RemoveDynamic(this, &UDSGatherPartyWidget::RequestRemoveFromParty);

	Super::NativeDestruct();
}

void UDSGatherPartyWidget::OnPartyMember1Clicked() { SelectPartySlot(0); }
void UDSGatherPartyWidget::OnPartyMember2Clicked() { SelectPartySlot(1); }
void UDSGatherPartyWidget::OnPartyMember3Clicked() { SelectPartySlot(2); }
void UDSGatherPartyWidget::OnPartyMember4Clicked() { SelectPartySlot(3); }
void UDSGatherPartyWidget::OnPartyMember5Clicked() { SelectPartySlot(4); }
void UDSGatherPartyWidget::OnPartyMember6Clicked() { SelectPartySlot(5); }

void UDSGatherPartyWidget::RefreshRosterList(FName PreferredSelectionName)
{
	if (!IsValid(RosterList) || !EntryWidgetClass) return;

	UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr;
	UDSSaveGameSubsystem* SaveSubsystem = GI ? GI->GetSubsystem<UDSSaveGameSubsystem>() : nullptr;
	if (!SaveSubsystem) return;

	RosterList->ClearChildren();

	const TArray<FPlayerCharacterSaveData> Characters = SaveSubsystem->LoadAllCharacters();

	UDSCharacterSaveEntryWidget* FirstEntry = nullptr;
	UDSCharacterSaveEntryWidget* PreferredEntry = nullptr;

	for (const FPlayerCharacterSaveData& Character : Characters)
	{
		if (IsCharacterInParty(Character.characterName)) continue;

		UDSCharacterSaveEntryWidget* Entry = CreateWidget<UDSCharacterSaveEntryWidget>(this, EntryWidgetClass);
		if (!Entry) continue;

		Entry->SetCharacterData(Character);
		Entry->OnEntryClicked.RemoveDynamic(this, &UDSGatherPartyWidget::HandleEntryClicked);
		Entry->OnEntryClicked.AddDynamic(this, &UDSGatherPartyWidget::HandleEntryClicked);

		RosterList->AddChild(Entry);

		if (!FirstEntry) FirstEntry = Entry;
		if (PreferredSelectionName != NAME_None && Character.characterName == PreferredSelectionName)
		{
			PreferredEntry = Entry;
		}
	}

	// 로스터 쪽(자동 선택 포함)이 선택된 상태라 파티 슬롯 선택이 아님 - Remove From Party는 숨김
	if (IsValid(Btn_RemoveFromParty1))
	{
		Btn_RemoveFromParty1->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (UDSCharacterSaveEntryWidget* ToSelect = PreferredEntry ? PreferredEntry : FirstEntry)
	{
		SelectedCharacterData = ToSelect->GetCharacterData();
		SelectedCharacterName = SelectedCharacterData.characterName;
		ApplyCharacterToCenterColumn(SelectedCharacterData);
	}
	else
	{
		SelectedCharacterData = FPlayerCharacterSaveData();
		SelectedCharacterName = NAME_None;
	}
}

void UDSGatherPartyWidget::RequestDeleteSelectedCharacter()
{
	// 선택된 게 로스터가 아니라 이미 파티에 들어간 캐릭터면(파티원 클릭으로 선택된 경우) 삭제 금지
	if (SelectedCharacterName.IsNone() || IsCharacterInParty(SelectedCharacterName) || !MessageBoxYesNoClass) return;

	const FText Message = FText::FromString(FString::Printf(TEXT("Are you sure you want to delete %s?"), *SelectedCharacterName.ToString()));
	if (UDSMessageBoxYesNoWidget* MsgBox = UDSMessageBoxYesNoWidget::Show(this, MessageBoxYesNoClass, Message))
	{
		MsgBox->OnResult.AddDynamic(this, &UDSGatherPartyWidget::OnDeleteConfirmResult);
	}
}

void UDSGatherPartyWidget::OnDeleteConfirmResult(bool bYes)
{
	if (!bYes || SelectedCharacterName.IsNone()) return;

	UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr;
	if (UDSSaveGameSubsystem* SaveSubsystem = GI ? GI->GetSubsystem<UDSSaveGameSubsystem>() : nullptr)
	{
		SaveSubsystem->DeleteCharacter(SelectedCharacterName);
	}

	RefreshRosterList();
}

void UDSGatherPartyWidget::RequestExitHomeBase()
{
	int32 MemberCount = 0;
	for (UPlayerCharacterInstanceComponent* Member : GetPartyMembers())
	{
		if (Member) ++MemberCount;
	}

	if (MemberCount == 0)
	{
		if (MessageBoxClass)
		{
			UDSMessageBoxWidget::Show(this, MessageBoxClass, FText::FromString(TEXT("Your party is empty. You need at least one member to leave.")));
		}
		return;
	}

	if (MemberCount < 6)
	{
		const FText Message = FText::FromString(FString::Printf(TEXT("Your party only has %d member(s). Leave anyway?"), MemberCount));
		if (UDSMessageBoxYesNoWidget* MsgBox = UDSMessageBoxYesNoWidget::Show(this, MessageBoxYesNoClass, Message))
		{
			MsgBox->OnResult.AddDynamic(this, &UDSGatherPartyWidget::OnExitConfirmResult);
			return;
		}
	}

	if (ADSGameMode* GM = GetGameMode())
	{
		GM->ExitHomeBase();
	}
}

void UDSGatherPartyWidget::OnExitConfirmResult(bool bYes)
{
	if (!bYes) return;

	if (ADSGameMode* GM = GetGameMode())
	{
		GM->ExitHomeBase();
	}
}

void UDSGatherPartyWidget::HandleEntryClicked(UDSCharacterSaveEntryWidget* Entry)
{
	if (!Entry) return;

	Btn_RemoveFromParty1->SetVisibility(ESlateVisibility::Collapsed);

	SelectedCharacterData = Entry->GetCharacterData();
	SelectedCharacterName = SelectedCharacterData.characterName;
	ApplyCharacterToCenterColumn(SelectedCharacterData);
}

void UDSGatherPartyWidget::ApplyCharacterToCenterColumn(const FPlayerCharacterSaveData& Data)
{
	if (PortraitImage)
	{
		if (UTexture2D* Portrait = Data.Portrait.LoadSynchronous())
		{
			PortraitImage->SetBrushFromTexture(Portrait);
		}
	}

	if (CharNameText) CharNameText->SetText(FText::FromName(Data.characterName));
	if (CharClassText) CharClassText->SetText(FText::FromString(GetProfessionDisplayNameLocal_GatherParty(Data.Profession)));
	if (CharRaceText) CharRaceText->SetText(FText::FromString(GetRaceDisplayNameLocal_GatherParty(Data.Race)));
	if (CharAlignText) CharAlignText->SetText(FText::GetEmpty());

	const FCharacterStat& Stat = Data.characterData.CharStat;
	if (StatValue_Str) StatValue_Str->SetText(FText::AsNumber(Stat.Strength));
	if (StatValue_Int) StatValue_Int->SetText(FText::AsNumber(Stat.Intelligence));
	if (StatValue_Pie) StatValue_Pie->SetText(FText::AsNumber(Stat.Piety));
	if (StatValue_Vit) StatValue_Vit->SetText(FText::AsNumber(Stat.Vitality));
	if (StatValue_Dex) StatValue_Dex->SetText(FText::AsNumber(Stat.Dexterity));
	if (StatValue_Spd) StatValue_Spd->SetText(FText::AsNumber(Stat.Speed));
	if (StatValue_Sen) StatValue_Sen->SetText(FText::AsNumber(Stat.Charisma));

	const FFaceStat& Face = Data.characterData.FinalFaceStat;
	if (SecondValue_HP) SecondValue_HP->SetText(FText::AsNumber(Face.MaxHP));
	if (SecondValue_SP) SecondValue_SP->SetText(FText::AsNumber(Face.MaxSP.Total()));

	// 스태미나/소지중량 데이터 모델이 아직 없어서 자리만 채워둠
	if (SecondValue_Stam) SecondValue_Stam->SetText(FText::FromString(TEXT("-")));
	if (SecondValue_Carry) SecondValue_Carry->SetText(FText::FromString(TEXT("-")));
	if (SecondValue_Carry_1) SecondValue_Carry_1->SetText(FText::FromString(TEXT("-")));
}

void UDSGatherPartyWidget::RequestAddSelectedToParty()
{
	// 선택된 게 로스터 캐릭터가 아니라 이미 파티에 들어간 캐릭터면(파티원 클릭으로 선택된 경우) 무시
	if (SelectedCharacterName.IsNone() || IsCharacterInParty(SelectedCharacterName)) return;

	ADSGameMode* GM = GetGameMode();
	UPlayerPartyManagerComponent* Manager = GM ? GM->GetPartyManager() : nullptr;
	ADSPlayerParty* Party = GM ? GM->GetPartyObject() : nullptr;
	if (!Manager || !Party) return;

	int32 EmptyIndex = INDEX_NONE;
	for (int32 i = 0; i < Party->characters.Num(); ++i)
	{
		if (Party->characters[i] == nullptr)
		{
			EmptyIndex = i;
			break;
		}
	}
	if (EmptyIndex == INDEX_NONE) return; // 파티 6명 꽉 참

	// 세이브데이터로 그 자리에서 바로 라이브 캐릭터 스폰 (컴포넌트 생성 + 캡슐 할당까지)
	if (Manager->AddCharacterToParty(SelectedCharacterData, EmptyIndex))
	{
		RefreshSlotImage(EmptyIndex);

		// 방금 파티에 넣은 캐릭터는 IsCharacterInParty에 걸려서 로스터에서 자동으로 빠짐
		RefreshRosterList();
	}
}

void UDSGatherPartyWidget::RequestRemoveFromParty()
{
	ADSGameMode* GM = GetGameMode();
	UPlayerPartyManagerComponent* Manager = GM ? GM->GetPartyManager() : nullptr;
	ADSPlayerParty* Party = GM ? GM->GetPartyObject() : nullptr;
	if (!Manager || !Party) return;
	if (!Party->characters.IsValidIndex(SelectedPartySlotIndex)) return;

	UPlayerCharacterInstanceComponent* Member = Party->characters[SelectedPartySlotIndex];
	if (!Member) return;

	const FName RemovedName = Member->characterName;

	// 현재 상태 디스크 저장 + 캡슐 해제 + 컴포넌트 파괴 + 슬롯 비우기까지 매니저가 처리
	Manager->RemoveCharacterFromParty(SelectedPartySlotIndex);

	// 뺀 자리 뒤에 있던 애들을 한 칸씩 앞으로 당김 (123456 중 3번 제거 -> 124560)
	for (int32 i = SelectedPartySlotIndex; i < Party->characters.Num() - 1; ++i)
	{
		Party->characters[i] = Party->characters[i + 1];
	}
	Party->characters[Party->characters.Num() - 1] = nullptr;

	for (int32 i = 0; i < 6; ++i)
	{
		RefreshSlotImage(i);
	}

	SelectedPartySlotIndex = INDEX_NONE;

	if (IsValid(Btn_RemoveFromParty1))
	{
		Btn_RemoveFromParty1->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 남은 파티원이 있으면 그쪽을 선택, 파티가 텅 비었으면 방금 뺀 캐릭터를 로스터에서 선택
	int32 RemainingIndex = INDEX_NONE;
	for (int32 i = 0; i < Party->characters.Num(); ++i)
	{
		if (Party->characters[i] != nullptr)
		{
			RemainingIndex = i;
			break;
		}
	}

	if (RemainingIndex != INDEX_NONE)
	{
		RefreshRosterList();
		SelectPartySlot(RemainingIndex);
	}
	else
	{
		RefreshRosterList(RemovedName);
	}
}

UButton* UDSGatherPartyWidget::GetSlotButton(int32 PartyIndex) const
{
	// PartyMember1~6 이름 자체가 파티 순서라 배열 인덱스+1로 바로 매핑됨
	switch (PartyIndex)
	{
	case 0: return PartyMember1;
	case 1: return PartyMember2;
	case 2: return PartyMember3;
	case 3: return PartyMember4;
	case 4: return PartyMember5;
	case 5: return PartyMember6;
	default: return nullptr;
	}
}

ADSGameMode* UDSGatherPartyWidget::GetGameMode() const
{
	return GetWorld() ? GetWorld()->GetAuthGameMode<ADSGameMode>() : nullptr;
}

UPlayerCharacterInstanceComponent* UDSGatherPartyWidget::GetPartyMember(int32 PartyIndex) const
{
	ADSGameMode* GM = GetGameMode();
	ADSPlayerParty* Party = GM ? GM->GetPartyObject() : nullptr;
	if (!Party || !Party->characters.IsValidIndex(PartyIndex)) return nullptr;
	return Party->characters[PartyIndex];
}

TArray<UPlayerCharacterInstanceComponent*> UDSGatherPartyWidget::GetPartyMembers() const
{
	TArray<UPlayerCharacterInstanceComponent*> Result;
	ADSGameMode* GM = GetGameMode();
	ADSPlayerParty* Party = GM ? GM->GetPartyObject() : nullptr;
	if (Party)
	{
		for (UPlayerCharacterInstanceComponent* Member : Party->characters)
		{
			Result.Add(Member);
		}
	}
	return Result;
}

void UDSGatherPartyWidget::RefreshSlotImage(int32 PartyIndex)
{
	UButton* SlotButton = GetSlotButton(PartyIndex);
	if (!SlotButton) return;

	UPlayerCharacterInstanceComponent* Member = GetPartyMember(PartyIndex);
	UTexture2D* Portrait = Member ? Member->ToSaveData().Portrait.LoadSynchronous() : nullptr;

	if (!Portrait)
	{
		// 빈 슬롯 - 초상화 씌우기 전 원래 스타일(해골 등)로 복원
		if (DefaultSlotStyles.IsValidIndex(PartyIndex))
		{
			SlotButton->SetStyle(DefaultSlotStyles[PartyIndex]);
		}
		return;
	}

	FButtonStyle Style = DefaultSlotStyles.IsValidIndex(PartyIndex) ? DefaultSlotStyles[PartyIndex] : SlotButton->GetStyle();
	Style.Normal.SetResourceObject(Portrait);
	Style.Hovered.SetResourceObject(Portrait);
	Style.Pressed.SetResourceObject(Portrait);
	SlotButton->SetStyle(Style);
}

bool UDSGatherPartyWidget::IsCharacterInParty(FName CharacterName) const
{
	if (CharacterName.IsNone()) return false;

	ADSGameMode* GM = GetGameMode();
	ADSPlayerParty* Party = GM ? GM->GetPartyObject() : nullptr;
	if (!Party) return false;

	for (const UPlayerCharacterInstanceComponent* Member : Party->characters)
	{
		if (Member && Member->characterName == CharacterName) return true;
	}
	return false;
}
