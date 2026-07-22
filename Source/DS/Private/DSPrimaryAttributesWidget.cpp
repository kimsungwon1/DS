
#include "DSPrimaryAttributesWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UDSPrimaryAttributesWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Strengthminus) Strengthminus->OnClicked.AddDynamic(this, &UDSPrimaryAttributesWidget::OnStrengthMinus);
	if (Strengthadd) Strengthadd->OnClicked.AddDynamic(this, &UDSPrimaryAttributesWidget::OnStrengthPlus);

	if (Intelligenceminus) Intelligenceminus->OnClicked.AddDynamic(this, &UDSPrimaryAttributesWidget::OnIntelligenceMinus);
	if (Intelligenceadd) Intelligenceadd->OnClicked.AddDynamic(this, &UDSPrimaryAttributesWidget::OnIntelligencePlus);

	if (Pietyminus) Pietyminus->OnClicked.AddDynamic(this, &UDSPrimaryAttributesWidget::OnPietyMinus);
	if (Pietyadd) Pietyadd->OnClicked.AddDynamic(this, &UDSPrimaryAttributesWidget::OnPietyPlus);

	if (Vitalityminus) Vitalityminus->OnClicked.AddDynamic(this, &UDSPrimaryAttributesWidget::OnVitalityMinus);
	if (Vitalityadd) Vitalityadd->OnClicked.AddDynamic(this, &UDSPrimaryAttributesWidget::OnVitalityPlus);

	if (Dexterityminus) Dexterityminus->OnClicked.AddDynamic(this, &UDSPrimaryAttributesWidget::OnDexterityMinus);
	if (Dexterityadd) Dexterityadd->OnClicked.AddDynamic(this, &UDSPrimaryAttributesWidget::OnDexterityPlus);

	if (Speedminus) Speedminus->OnClicked.AddDynamic(this, &UDSPrimaryAttributesWidget::OnSpeedMinus);
	if (Speedadd) Speedadd->OnClicked.AddDynamic(this, &UDSPrimaryAttributesWidget::OnSpeedPlus);

	if (Charismaminus) Charismaminus->OnClicked.AddDynamic(this, &UDSPrimaryAttributesWidget::OnCharismaMinus);
	if (Charismaadd) Charismaadd->OnClicked.AddDynamic(this, &UDSPrimaryAttributesWidget::OnCharismaPlus);
}

void UDSPrimaryAttributesWidget::SetStatDisplay(EPrimaryStatType Stat, int32 FinalValue, int32 UserAddedValue)
{
	UTextBlock* ResultText = nullptr;
	UTextBlock* RemainText = nullptr;

	switch (Stat)
	{
	case EPrimaryStatType::Strength:     ResultText = StrengthResult;     RemainText = Strengthremain;     break;
	case EPrimaryStatType::Intelligence: ResultText = IntelligenceResult; RemainText = Intelligenceremain; break;
	case EPrimaryStatType::Piety:        ResultText = PietyResult;        RemainText = Pietyremain;        break;
	case EPrimaryStatType::Vitality:     ResultText = VitalityResult;     RemainText = Vitalityremain;     break;
	case EPrimaryStatType::Dexterity:    ResultText = DexterityResult;    RemainText = Dexterityremain;    break;
	case EPrimaryStatType::Speed:        ResultText = SpeedResult;        RemainText = Speedremain;        break;
	case EPrimaryStatType::Charisma:     ResultText = CharismaResult;     RemainText = Charismaremain;     break;
	}

	if (ResultText) ResultText->SetText(FText::AsNumber(FinalValue));
	if (RemainText) RemainText->SetText(FText::AsNumber(UserAddedValue));
}

void UDSPrimaryAttributesWidget::SetAllStats(const FCharacterStat& FinalStats, const FCharacterStat& UserAddedStats)
{
	SetStatDisplay(EPrimaryStatType::Strength,     FinalStats.Strength,     UserAddedStats.Strength);
	SetStatDisplay(EPrimaryStatType::Intelligence, FinalStats.Intelligence, UserAddedStats.Intelligence);
	SetStatDisplay(EPrimaryStatType::Piety,        FinalStats.Piety,        UserAddedStats.Piety);
	SetStatDisplay(EPrimaryStatType::Vitality,     FinalStats.Vitality,     UserAddedStats.Vitality);
	SetStatDisplay(EPrimaryStatType::Dexterity,    FinalStats.Dexterity,    UserAddedStats.Dexterity);
	SetStatDisplay(EPrimaryStatType::Speed,        FinalStats.Speed,        UserAddedStats.Speed);
	SetStatDisplay(EPrimaryStatType::Charisma,     FinalStats.Charisma,     UserAddedStats.Charisma);
}

void UDSPrimaryAttributesWidget::OnStrengthMinus()     { OnStatButtonClicked.Broadcast(EPrimaryStatType::Strength, -1); }
void UDSPrimaryAttributesWidget::OnStrengthPlus()      { OnStatButtonClicked.Broadcast(EPrimaryStatType::Strength, 1); }
void UDSPrimaryAttributesWidget::OnIntelligenceMinus() { OnStatButtonClicked.Broadcast(EPrimaryStatType::Intelligence, -1); }
void UDSPrimaryAttributesWidget::OnIntelligencePlus()  { OnStatButtonClicked.Broadcast(EPrimaryStatType::Intelligence, 1); }
void UDSPrimaryAttributesWidget::OnPietyMinus()        { OnStatButtonClicked.Broadcast(EPrimaryStatType::Piety, -1); }
void UDSPrimaryAttributesWidget::OnPietyPlus()         { OnStatButtonClicked.Broadcast(EPrimaryStatType::Piety, 1); }
void UDSPrimaryAttributesWidget::OnVitalityMinus()     { OnStatButtonClicked.Broadcast(EPrimaryStatType::Vitality, -1); }
void UDSPrimaryAttributesWidget::OnVitalityPlus()      { OnStatButtonClicked.Broadcast(EPrimaryStatType::Vitality, 1); }
void UDSPrimaryAttributesWidget::OnDexterityMinus()    { OnStatButtonClicked.Broadcast(EPrimaryStatType::Dexterity, -1); }
void UDSPrimaryAttributesWidget::OnDexterityPlus()     { OnStatButtonClicked.Broadcast(EPrimaryStatType::Dexterity, 1); }
void UDSPrimaryAttributesWidget::OnSpeedMinus()        { OnStatButtonClicked.Broadcast(EPrimaryStatType::Speed, -1); }
void UDSPrimaryAttributesWidget::OnSpeedPlus()         { OnStatButtonClicked.Broadcast(EPrimaryStatType::Speed, 1); }
void UDSPrimaryAttributesWidget::OnCharismaMinus()     { OnStatButtonClicked.Broadcast(EPrimaryStatType::Charisma, -1); }
void UDSPrimaryAttributesWidget::OnCharismaPlus()      { OnStatButtonClicked.Broadcast(EPrimaryStatType::Charisma, 1); }
