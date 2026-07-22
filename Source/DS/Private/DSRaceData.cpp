
#include "DSRaceData.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#include "Selector.h"
#include "Targeter_DSCharacter.h"
#include "Targeter_PositionVector.h"


EDataValidationResult UDSRaceData::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	return Result;
}
#endif
