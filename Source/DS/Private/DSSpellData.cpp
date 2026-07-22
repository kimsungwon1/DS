
#include "DSSpellData.h"
#include "Spell.h"

int32 UDSSpellData::GetRequiredSkillForPower(int32 PointPerPower)
{
	static const int32 Thresholds[] = { 5, 15, 30, 40, 50, 60, 70, 80, 90, 98 };
	const int32 Index = FMath::Clamp((PointPerPower / 2) - 1, 0, UE_ARRAY_COUNT(Thresholds) - 1);
	return Thresholds[Index];
}

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#include "Selector.h"
#include "Targeter_DSCharacter.h"
#include "Targeter_PositionVector.h"

namespace
{
	bool IsCharacterTargetType(EDSTargetType Type)
	{
		return Type == EDSTargetType::Opponent
			|| Type == EDSTargetType::OpponentParty
			|| Type == EDSTargetType::Self
			|| Type == EDSTargetType::Friend
			|| Type == EDSTargetType::Party;
	}

	bool IsPositionTargetType(EDSTargetType Type)
	{
		return Type == EDSTargetType::Around
			|| Type == EDSTargetType::Ground
			|| Type == EDSTargetType::UserDefine;
	}
}

EDataValidationResult UDSSpellData::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (SpellClass.IsNull() || !SelectorClass)
		return Result; // 둘 중 하나라도 미지정이면 대조할 게 없음

	UClass* SpellCls = SpellClass.LoadSynchronous();
	if (!SpellCls)
		return Result;

	const ASelector* SelectorCDO = SelectorClass->GetDefaultObject<ASelector>();
	if (!SelectorCDO)
		return Result;

	const EDSTargetType Produced = SelectorCDO->ProducedTargetType;
	const bool bWantsCharacter = SpellCls->ImplementsInterface(UTargeter_DSCharacter::StaticClass());
	const bool bWantsPosition  = SpellCls->ImplementsInterface(UTargeter_PositionVector::StaticClass());

	if (bWantsCharacter && !IsCharacterTargetType(Produced))
	{
		Context.AddError(FText::Format(
			FText::FromString(TEXT("스펠 '{0}'은 캐릭터 대상(ITargeter_DSCharacter)인데 셀렉터 '{1}'의 ProducedTargetType({2})이 캐릭터 유형이 아님")),
			FText::FromString(SpellCls->GetName()),
			FText::FromString(SelectorClass->GetName()),
			UEnum::GetDisplayValueAsText(Produced)));
		Result = EDataValidationResult::Invalid;
	}

	if (bWantsPosition && !IsPositionTargetType(Produced))
	{
		Context.AddError(FText::Format(
			FText::FromString(TEXT("스펠 '{0}'은 위치 대상(ITargeter_PositionVector)인데 셀렉터 '{1}'의 ProducedTargetType({2})이 위치 유형이 아님")),
			FText::FromString(SpellCls->GetName()),
			FText::FromString(SelectorClass->GetName()),
			UEnum::GetDisplayValueAsText(Produced)));
		Result = EDataValidationResult::Invalid;
	}

	if (!bWantsCharacter && !bWantsPosition)
	{
		Context.AddWarning(FText::Format(
			FText::FromString(TEXT("스펠 '{0}'이 Targeter 인터페이스를 상속하지 않음 — 셀렉터 검증 불가")),
			FText::FromString(SpellCls->GetName())));
	}

	return Result;
}
#endif
