
#pragma once

#include "CoreMinimal.h"
#include "DSUserWidget.h"
#include "SaveGameData.h"
#include "Styling/SlateTypes.h"
#include "DSGatherPartyWidget.generated.h"

class UScrollBox;
class UImage;
class UButton;
class UTextBlock;
class UDSCharacterSaveEntryWidget;
class UDSMessageBoxYesNoWidget;
class UDSMessageBoxWidget;
class UPlayerCharacterInstanceComponent;
class APlayerPartyMover;
class ADSPlayerParty;
class UPlayerPartyManagerComponent;
class ADSGameMode;

// DSG_GatherParty 전용. 세이브된 모든 캐릭터를 RosterList에 나열하고,
// 선택된 캐릭터 정보를 CenterColumn에 반영.
UCLASS()
class DS_API UDSGatherPartyWidget : public UDSUserWidget
{
	GENERATED_BODY()

public:
	UDSGatherPartyWidget();

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// 세이브 데이터를 다시 읽어서 로스터를 새로 그림. PreferredSelectionName이 있으면(막 만든 캐릭터 등)
	// 그 캐릭터를 선택하고, 없으면 첫번째 캐릭터를 선택함
	UFUNCTION(BlueprintCallable)
	void RefreshRosterList(FName PreferredSelectionName = NAME_None);

	// 현재 선택된 캐릭터 삭제를 요청 - "정말 -이름-을 삭제하시겠습니까?" 확인창부터 띄움 (Btn_DeleteCharacter에서 호출)
	UFUNCTION(BlueprintCallable)
	void RequestDeleteSelectedCharacter();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FName GetSelectedCharacterName() const { return SelectedCharacterName; }

	// 현재 선택된 캐릭터의 전체 세이브데이터 (Review Character 등에서 EnterReviewMode에 그대로 넘길 때 사용)
	UFUNCTION(BlueprintCallable, BlueprintPure)
	const FPlayerCharacterSaveData& GetSelectedCharacterData() const { return SelectedCharacterData; }

	// 로스터에서 선택된 캐릭터를 파티 빈 슬롯(순서대로)에 추가 - 꽉 찼으면 아무 일도 안 함 (Btn_AddToParty에서 호출)
	UFUNCTION(BlueprintCallable)
	void RequestAddSelectedToParty();

	// 파티 슬롯 이미지를 클릭해서 선택해둔 상태에서 호출 - 그 슬롯을 비우고 로스터로 되돌림 (Btn_RemoveFromParty에서 호출)
	UFUNCTION(BlueprintCallable)
	void RequestRemoveFromParty();

	// 현재 파티 슬롯 6칸의 라이브 캐릭터 (빈 슬롯은 nullptr) - PartySlots 같은 별도 버퍼 없이 ADSPlayerParty::characters를 그대로 반영
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UPlayerCharacterInstanceComponent*> GetPartyMembers() const;

	// 홈베이스 나가기(Exit/Confirm) 버튼에서 호출 - 파티가 0명이면 막고 메시지만 띄움,
	// 꽉 안 찼으면 정말 나갈지 확인창, 꽉 찼으면 바로 나감
	UFUNCTION(BlueprintCallable)
	void RequestExitHomeBase();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UScrollBox> RosterList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> PortraitImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CharNameText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CharClassText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CharRaceText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CharAlignText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> StatValue_Str;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> StatValue_Int;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> StatValue_Pie;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> StatValue_Vit;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> StatValue_Dex;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> StatValue_Spd;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> StatValue_Sen;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> SecondValue_HP;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> SecondValue_SP;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> SecondValue_Stam;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> SecondValue_Carry;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> SecondValue_Carry_1;

	// 파티 슬롯 버튼 6개 - 이름 자체가 파티 순서(1~6). 클릭하면 그 슬롯이 Remove 대상으로 선택됨
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UButton> PartyMember1;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UButton> PartyMember2;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UButton> PartyMember3;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UButton> PartyMember4;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UButton> PartyMember5;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) TObjectPtr<UButton> PartyMember6;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Btn_RemoveFromParty1;

	// DSG_CharacterSaveEntry(UDSCharacterSaveEntryWidget 상속) 클래스. 기본값은 생성자에서 자동으로 잡아줌
	UPROPERTY(EditDefaultsOnly, Category = "Roster")
	TSubclassOf<UDSCharacterSaveEntryWidget> EntryWidgetClass;

	// 삭제 확인용 Yes/No 메시지박스 (DSG_MessageBox_YesNo) - 파티 인원 부족 시 나가기 확인에도 재사용
	UPROPERTY(EditDefaultsOnly, Category = "Roster")
	TSubclassOf<UDSMessageBoxYesNoWidget> MessageBoxYesNoClass;

	// 파티 0명일 때 나가기 막는 안내창 (DSG_MessageBox_Close)
	UPROPERTY(EditDefaultsOnly, Category = "Roster")
	TSubclassOf<UDSMessageBoxWidget> MessageBoxClass;

private:
	UFUNCTION()
	void HandleEntryClicked(UDSCharacterSaveEntryWidget* Entry);

	UFUNCTION()
	void OnDeleteConfirmResult(bool bYes);

	UFUNCTION()
	void OnExitConfirmResult(bool bYes);

	void ApplyCharacterToCenterColumn(const FPlayerCharacterSaveData& Data);

	UButton* GetSlotButton(int32 PartyIndex) const;
	void RefreshSlotImage(int32 PartyIndex);
	bool IsCharacterInParty(FName CharacterName) const;
	void SelectPartySlot(int32 PartyIndex);
	UPlayerCharacterInstanceComponent* GetPartyMember(int32 PartyIndex) const;

	// 필요할 때마다 그때그때 GameMode를 새로 찾음 - 위젯 쪽에 레퍼런스를 따로 캐싱해두지 않음
	ADSGameMode* GetGameMode() const;

	UFUNCTION() void OnPartyMember1Clicked();
	UFUNCTION() void OnPartyMember2Clicked();
	UFUNCTION() void OnPartyMember3Clicked();
	UFUNCTION() void OnPartyMember4Clicked();
	UFUNCTION() void OnPartyMember5Clicked();
	UFUNCTION() void OnPartyMember6Clicked();

	FName SelectedCharacterName;
	FPlayerCharacterSaveData SelectedCharacterData;

	int32 SelectedPartySlotIndex = INDEX_NONE;

	// 6개 슬롯 버튼의 원래(빈 슬롯) 스타일 - 초상화 씌웠다가 빼면 이걸로 복원
	TArray<FButtonStyle> DefaultSlotStyles;
};
