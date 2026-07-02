#pragma once

#include "CoreMinimal.h"
#include "ActionCastType.h"
#include "DSBlueprintLibrary.h"
#include "DSStats.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStat {
	GENERATED_BODY()
public:
	FCharacterStat() {}

	static const int32 STANDARD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Strength = STANDARD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Intelligence = STANDARD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Piety = STANDARD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Vitality = STANDARD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Dexterity = STANDARD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Speed = STANDARD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Charisma = STANDARD;

};

USTRUCT(BlueprintType)
struct FFaceStat {
	GENERATED_BODY()

	FFaceStat() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 MaxHP = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Health = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 MaxSP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 CurrentSP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Armor = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Karma = 8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resistance")
	int32 ResistAether = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resistance")
	int32 ResistWater = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resistance")
	int32 ResistAir = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resistance")
	int32 ResistEarth = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resistance")
	int32 ResistSun = 0;
};


USTRUCT(BlueprintType)
struct FWeaponStat
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int damageMinimum = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int damageMaximum = 4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int damageEnhance = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EActionCastType eAttackCastType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EWeaponRangeType eWeaponRangeType;
};

UENUM(BlueprintType)
enum class ERace : uint8
{
	HUMAN UMETA(DisplayName = "Human"),
	HALFELF UMETA(DisplayName = "Half Elf"),
	DWARF UMETA(DisplayName = "Dwarf"), 
	HALFLING UMETA(DisplayName = "HALFLING"),
	GNOME UMETA(DisplayName = "Gnome"),
	BEASTMAN UMETA(DisplayName = "Beastman"),
	DRACON UMETA(DisplayName = "Dracon"),
	LIZARDMAN UMETA(DisplayName = "Lizardman")
};

UENUM(BlueprintType)
enum class EProfession : uint8
{
	WARRIOR UMETA(DisplayName = "Warrior"),
	MAGE UMETA(DisplayName = "Mage"),
	PRIEST UMETA(DisplayName = "Priest"),
	THIEF UMETA(DisplayName = "Thief"),
	DRUID UMETA(DisplayName = "Druid"),
	ALCHEMIST UMETA(DisplayName = "Alchemist"),
	BARD UMETA(DisplayName = "Bard"),
	PALADIN UMETA(DisplayName = "Paladin"),
	VALKYRIE UMETA(DisplayName = "Valkyrie"),
	SAMURAI UMETA(DisplayName = "Samurai"),
	BABARIAN UMETA(DisplayName = "Babarian"),
	NINJA UMETA(DisplayName = "Ninja"),
	SHADOW UMETA(DisplayName = "Shadow"),
	BISHOP UMETA(DisplayName = "Bishop"),
	MONK UMETA(DisplayName = "MONK"),
	NECROMANCER UMETA(DisplayName = "Necromancer")
};