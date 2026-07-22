
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DSStats.h"
#include "CharacterClassRegistry.generated.h"

class UCharacterClass;
class UCharacterClassConfig;

UCLASS(BlueprintType)
class DS_API UCharacterClassRegistry : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CharacterClasses")
	UCharacterClass* GetCharacterClassByProfession(EProfession Profession) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CharacterClasses")
	const TArray<UCharacterClass*>& GetAllCharacterClasses() const { return AllCharacterClasses; }

private:
	TArray<TObjectPtr<UCharacterClass>> AllCharacterClasses;

	// 에셋 경로 — 에디터에서 Config 에셋 만든 뒤 경로 맞춰줘라
	static constexpr const TCHAR* ConfigAssetPath = TEXT("/Game/Made/Players/Classes/CharacterClassConfig");
};
