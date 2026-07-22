
#include "DSSpellAddWidget.h"
#include "UI/SpellEntryWidget.h"
#include "DSSpellData.h"
#include "SpellRegistry.h"
#include "Components/ScrollBox.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

void UDSSpellAddWidget::PopulateSpells(const FSkillStat& Skill, const FKnownSpells& AlreadyKnown)
{
	bHasAnyLearnable = false;

	const bool bCanThaumaturgy = Skill.Thaumaturgy != -1;
	const bool bCanTheology = Skill.Theology != -1;

	if (!bCanThaumaturgy && !bCanTheology)
	{
		if (SpellListBox_Aether) SpellListBox_Aether->ClearChildren();
		if (SpellListBox_Water)  SpellListBox_Water->ClearChildren();
		if (SpellListBox_Air)    SpellListBox_Air->ClearChildren();
		if (SpellListBox_Earth)  SpellListBox_Earth->ClearChildren();
		if (SpellListBox_Sun)    SpellListBox_Sun->ClearChildren();
		return;
	}

	PopulateRealmList(SpellListBox_Aether, ESpellElement::Aether, Skill, AlreadyKnown.KnownSpells_Aether);
	PopulateRealmList(SpellListBox_Water,  ESpellElement::Water,  Skill, AlreadyKnown.KnownSpells_Water);
	PopulateRealmList(SpellListBox_Air,    ESpellElement::Air,    Skill, AlreadyKnown.KnownSpells_Air);
	PopulateRealmList(SpellListBox_Earth,  ESpellElement::Earth,  Skill, AlreadyKnown.KnownSpells_Earth);
	PopulateRealmList(SpellListBox_Sun,    ESpellElement::Sun,    Skill, AlreadyKnown.KnownSpells_Sun);
}

void UDSSpellAddWidget::PopulateRealmList(UScrollBox* ListBox, ESpellElement Element, const FSkillStat& Skill, const TArray<TSoftObjectPtr<UDSSpellData>>& AlreadyKnown)
{
	if (!ListBox || !SpellEntryClass) return;

	ListBox->ClearChildren();

	UGameInstance* GI = GetWorld() ? GetWorld()->GetGameInstance() : nullptr;
	USpellRegistry* Registry = GI ? GI->GetSubsystem<USpellRegistry>() : nullptr;
	if (!Registry) return;

	for (UDSSpellData* Spell : Registry->GetAllSpells())
	{
		if (!Spell || Spell->spellStat.element != Element || Spell->SpellClass.IsNull()) continue;

		// 마법사 스펠은 Thaumaturgy, 프리스트 스펠은 Theology - 해당 책 자체를 못 쓰면(-1) 제외
		const int32 BookSkill = (Spell->spellStat.SpellBookType == ESpellBookType::Thaumaturgy) ? Skill.Thaumaturgy : Skill.Theology;
		if (BookSkill == -1) continue;

		bool bAlreadyKnown = false;
		for (const TSoftObjectPtr<UDSSpellData>& Known : AlreadyKnown)
		{
			if (Known.Get() == Spell)
			{
				bAlreadyKnown = true;
				break;
			}
		}
		if (bAlreadyKnown) continue;

		const int32 RequiredSkill = UDSSpellData::GetRequiredSkillForPower(Spell->spellStat.PointPerPower);
		if (BookSkill < RequiredSkill) continue;

		USpellEntryWidget* Entry = CreateWidget<USpellEntryWidget>(this, SpellEntryClass);
		if (!Entry) continue;

		Entry->SetSpellData(Spell);
		Entry->OnSpellEntryClicked.RemoveDynamic(this, &UDSSpellAddWidget::HandleSpellEntryClicked);
		Entry->OnSpellEntryClicked.AddDynamic(this, &UDSSpellAddWidget::HandleSpellEntryClicked);
		ListBox->AddChild(Entry);

		bHasAnyLearnable = true;
	}
}

void UDSSpellAddWidget::HandleSpellEntryClicked(UDSSpellData* SpellData)
{
	// 실제 선택/해제 판단 및 포인트 증감은 상위(DSCharacterStatsWidget)가 IsSpellSelected/SetSpellSelected로 처리
	OnSpellEntryClicked.Broadcast(SpellData);
}

USpellEntryWidget* UDSSpellAddWidget::FindEntry(UDSSpellData* Spell) const
{
	if (!Spell) return nullptr;

	for (const UScrollBox* Box : { SpellListBox_Aether, SpellListBox_Water, SpellListBox_Air, SpellListBox_Earth, SpellListBox_Sun })
	{
		if (!Box) continue;

		for (int32 i = 0; i < Box->GetChildrenCount(); ++i)
		{
			if (USpellEntryWidget* Entry = Cast<USpellEntryWidget>(Box->GetChildAt(i)))
			{
				if (Entry->GetSpellData() == Spell)
				{
					return Entry;
				}
			}
		}
	}
	return nullptr;
}

bool UDSSpellAddWidget::IsSpellSelected(UDSSpellData* Spell) const
{
	const USpellEntryWidget* Entry = FindEntry(Spell);
	return Entry && Entry->IsSelected();
}

void UDSSpellAddWidget::SetSpellSelected(UDSSpellData* Spell, bool bSelected)
{
	if (USpellEntryWidget* Entry = FindEntry(Spell))
	{
		Entry->SetSelected(bSelected);
	}
}

TArray<UDSSpellData*> UDSSpellAddWidget::GetSelectedSpells() const
{
	TArray<UDSSpellData*> Result;

	for (const UScrollBox* Box : { SpellListBox_Aether, SpellListBox_Water, SpellListBox_Air, SpellListBox_Earth, SpellListBox_Sun })
	{
		if (!Box) continue;

		for (int32 i = 0; i < Box->GetChildrenCount(); ++i)
		{
			if (USpellEntryWidget* Entry = Cast<USpellEntryWidget>(Box->GetChildAt(i)))
			{
				if (Entry->IsSelected() && Entry->GetSpellData())
				{
					Result.Add(Entry->GetSpellData());
				}
			}
		}
	}
	return Result;
}

void UDSSpellAddWidget::ClearAllSelections()
{
	for (const UScrollBox* Box : { SpellListBox_Aether, SpellListBox_Water, SpellListBox_Air, SpellListBox_Earth, SpellListBox_Sun })
	{
		if (!Box) continue;

		for (int32 i = 0; i < Box->GetChildrenCount(); ++i)
		{
			if (USpellEntryWidget* Entry = Cast<USpellEntryWidget>(Box->GetChildAt(i)))
			{
				Entry->SetSelected(false);
			}
		}
	}
}
