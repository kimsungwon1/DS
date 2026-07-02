
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SpellRegistry.generated.h"

class UDSSpellData;
class USpellRegistryConfig;

UCLASS(BlueprintType)
class DS_API USpellRegistry : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spells")
	UDSSpellData* GetSpellByName(FName SpellName) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spells")
	const TArray<UDSSpellData*>& GetAllSpells() const { return AllSpells; }

private:
	TArray<TObjectPtr<UDSSpellData>> AllSpells;

	// 에셋 경로 — 에디터에서 Config 에셋 만든 뒤 경로 맞춰줘라
	static constexpr const TCHAR* ConfigAssetPath = TEXT("/Game/Data/Spells/DA_SpellRegistryConfig");
};
