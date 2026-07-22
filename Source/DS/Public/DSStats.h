#pragma once

#include "CoreMinimal.h"
#include "ActionCastType.h"
#include "DSBlueprintLibrary.h"
#include "DSStats.generated.h"

class UDSSpellData;

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
struct FSkillStat
{
	GENERATED_BODY()
public:
	FSkillStat() {}

	static const int32 STANDARD;

	// ===== Weapon Skills =====
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 Sword = STANDARD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 HandsAndFeet = STANDARD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 Axe = STANDARD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 Bow = STANDARD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 MaceAndFlail = STANDARD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 PoleAndStaff = STANDARD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 Shield = STANDARD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 Sling = STANDARD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 Throw = STANDARD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 WandAndDagger = STANDARD;

	// ===== Physical Skills =====
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical")
	int32 Stealth = STANDARD;

	// 스펠 시전 성공률에 관여 (부족하면 fizzle/backfire)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical")
	int32 Oratory = STANDARD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical")
	int32 Skulduggery = STANDARD;

	// ===== Academia Skills =====
	// 마법 아이템 감정/사용 및 마법 스크롤 사용(Scribe 흡수)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Academia")
	int32 Artifacts = STANDARD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Academia")
	int32 Kirijutsu = STANDARD;

	// 마법사 스펠 습득 가능 여부 결정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Academia")
	int32 Thaumaturgy = STANDARD;

	// 프리스트 스펠 습득 가능 여부 결정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Academia")
	int32 Theology = STANDARD;
};

USTRUCT(BlueprintType)
struct FResistance
{
	GENERATED_BODY()

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

// 스펠 포인트 - 5개 마법 영역(Aether/Water/Air/Earth/Sun)마다 따로 관리
USTRUCT(BlueprintType)
struct FSpellPoints
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellPoints")
	int32 Aether = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellPoints")
	int32 Water = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellPoints")
	int32 Air = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellPoints")
	int32 Earth = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellPoints")
	int32 Sun = 0;

	int32 Total() const { return Aether + Water + Air + Earth + Sun; }

	void AddToAll(int32 Amount)
	{
		Aether += Amount;
		Water += Amount;
		Air += Amount;
		Earth += Amount;
		Sun += Amount;
	}
};

// 영역별로 배운 스펠 목록 - 스펠 포인트 계산의 Nr_Known_Spells_in_Realm은 이 배열들의 개수(.Num())
// 세이브데이터에도 그대로 들어가므로 소프트 레퍼런스로 보관 (필요할 때 LoadSynchronous)
USTRUCT(BlueprintType)
struct FKnownSpells
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UDSSpellData>> KnownSpells_Aether;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UDSSpellData>> KnownSpells_Water;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UDSSpellData>> KnownSpells_Air;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UDSSpellData>> KnownSpells_Earth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UDSSpellData>> KnownSpells_Sun;
};

USTRUCT(BlueprintType)
struct FFaceStat {
	GENERATED_BODY()

	FFaceStat() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	bool Gender; // true가 남, false가 여

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 MaxHP = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Health = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	FSpellPoints MaxSP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	FSpellPoints CurrentSP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Armor = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Karma = 8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	FResistance Resistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	int Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	int CurrentExp = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	int ExpToNextLevel = 100;
};

USTRUCT(BlueprintType)
struct FGrowthPoints
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Growth")
	int32 StatPoints = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Growth")
	int32 SkillPoints = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Growth")
	int32 SpellNumberToLearn = 0;
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
enum class EPrimaryStatType : uint8
{
	Strength,
	Intelligence,
	Piety,
	Vitality,
	Dexterity,
	Speed,
	Charisma
};

UENUM(BlueprintType)
enum class ERace : uint8
{
	HUMAN UMETA(DisplayName = "Human"),
	HALFELF UMETA(DisplayName = "Half Elf"),
	DWARF UMETA(DisplayName = "Dwarf"), 
	HALFLING UMETA(DisplayName = "Halfling"),
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