// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"

#pragma comment(lib, "libmariadb.lib")

#include <mysql.h>
#include <random>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <functional>
#include <format>
#include <string_view>

#define U_NONE -1

#define INVENTORY_WINDOW_NUM 5
#define EQUIPMENT_WINDOW_NUM 1

#define EQUIPMENT_PARTS_NUM 6

#define ITEM_SLOT_IN_PADDING 12
#define EQUIPMENT_PARTS_OUT_PADDING 50

#define INVENTORY_COL_NUM 4
#define INVENTORY_ROW_NUM 6

#define CONSUME_MAX_CAPACITY 99

#define ID_INVENTORY 0
#define ID_EQUIPMENT 5
#define ID_ITEM_GACHA 6
#define ID_BASIC_STATUS 7
#define ID_ADVANCED_STATUS 8

// DB 전처리
#define DB_ADDRESS "localhost"
#define DB_ID "root"
#define DB_PW "0000"
#define DB_NAME "User"
#define DB_PORT 3306

#define STRING_SIZE 50

#define IS_EXISTED_IN_INVENTORY 0
#define IS_EXISTED_IN_EQUIPMENT 5

#define USER_GENDER_MALE 1
#define USER_GENDER_FEMALE 0

#define SP_LOGIN "CALL SP_LOGIN(?, ?)"
#define SP_REGISTER "CALL SP_REGISTER(?, ?, ?, ?)"
#define SP_DELETE_USER "CALL SP_DELETE_USER(?, ?)"
#define SP_CREATE_CHARACTER "CALL SP_CREATE_CHARACTER(?, ?, ?, ?)"
#define SP_DELETE_CHARACTER "CALL SP_DELETE_CHARACTER(?)"
#define SP_SET_CHARACTER_INFO "CALL SP_SET_CHARACTER_INFO(?, ?, ?, ?, ?, ?, ?, ?)"
#define SP_GET_CHARACTER_INFO "CALL SP_GET_CHARACTER_INFO(?)"
#define SP_SET_CHARACTER_ITEM "CALL SP_SET_CHARACTER_ITEM(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"
#define SP_GET_CHARACTER_ITEM "CALL SP_GET_CHARACTER_ITEM(?)"
#define SP_SET_ABILITY_DATA "CALL SP_SET_ABILITY_DATA(?, ?, ?, ?)"
#define SP_GET_ABILITY_DATA "CALL SP_GET_ABILITY_DATA(?)"
#define SP_INITIALIZE_CHARACTER_ITEM "CALL SP_INITIALIZE_CHARACTER_ITEM(?)"

// Item Generate
#define MAX_ABILITY_RANK 4 // 0 ~ 4
#define MAX_ABILITY_NUM 8

// Item Info
#define COMMON_COLOR RGB(255, 255, 255)  // White 
#define UNCOMMON_COLOR RGB(255,228,196)  // Bisque
#define RARE_COLOR RGB(0, 255, 255)		 // Aqua
#define EPIC_COLOR RGB(138, 43, 226)	 // BlueViolet
#define UNIQUE_COLOR RGB(255, 215, 0)	 // Gold
#define LEGENDARY_COLOR RGB(220, 20, 60) // Crimson

// Image Path
#define ERROR_IMAGE "res\\image\\error.bmp"
#define GACHA_BOX_IMAGE "res\\image\\gacha_box.png"

enum class ItemType {
	Err = -1,
	Equipment,
		Armor,
			Head,
			Chest,
			Pants,
			Gloves,
			Boots,
		Weapon,
			Sword,
			Bow,
			Staff,
			Daeger,
	Consume,
		Potion,
			HealthPotion,
			ManaPotion,
	Others
};

enum class AbilityType {
	Err = -1,
	AdditionalAbility,
		HP_Rate,
		HP_Val,
		MP_Rate,
		MP_Val,
		STR_Val,
		DEX_Val,
		INT_Val,
		LUK_Val,
		STR_Rate,
		DEX_Rate,
		INT_Rate,
		LUK_Rate,
		DEF_Rate,
		DEF_Val,
		ATK_Rate,
		ATK_Val,
		Magic_ATK_Rate,
		Magic_ATK_Val,
	SpecialAbility,
		CriticalRate,
		CriticalDamage,
		AttackSpeed,
		MoveSpeed,
		RegenerateHP,
		RegenerateMP,
		Dodge,
		BlockDamage,
		Poison,
		Ignite,
		Lightning,
		Explode,
		Freeze,
		LifeSteal,
		ReflectDamage,
	Others
};

enum class TargetType {
	Err = -1,
	Own,
	PartyMember,
	Enemy,
};

enum class CharacterType {
	Err = -1,
	User,
	NPC,
	Enemy
};

enum class GenderType {
	Err = -1,
	Male,
	Female
};

enum class ItemRank {
	Common = 0,
	Uncommon = 8,
	Rare = 16,
	Epic = 24,
	Unique = 32,
	Legendary = 40
};

enum class ItemInfoType {
	Basic,
	Additional,
	Special
};

enum class CharacterClass {
	Citizen,
	Warrior,
	Mage,
	Hunter,
	Paladin,
	Priest,
	Rogue
};

static std::string GetItemRankText(int rank) {
	std::string result = "Common";

	if (rank >= static_cast<int>(ItemRank::Legendary))
		result = "Legendary";
	else if (rank >= static_cast<int>(ItemRank::Unique))
		result = "Unique";
	else if (rank >= static_cast<int>(ItemRank::Epic))
		result = "Epic";
	else if (rank >= static_cast<int>(ItemRank::Rare))
		result = "Rare";
	else if (rank >= static_cast<int>(ItemRank::Uncommon))
		result = "Uncommon";

	return result;
}

static COLORREF GetItemRankColor(int rank) {
	COLORREF color = COMMON_COLOR;

	if (rank >= static_cast<int>(ItemRank::Legendary))
		color = LEGENDARY_COLOR;
	else if (rank >= static_cast<int>(ItemRank::Unique))
		color = UNIQUE_COLOR;
	else if (rank >= static_cast<int>(ItemRank::Epic))
		color = EPIC_COLOR;
	else if (rank >= static_cast<int>(ItemRank::Rare))
		color = RARE_COLOR;
	else if (rank >= static_cast<int>(ItemRank::Uncommon))
		color = UNCOMMON_COLOR;
	else
		color = COMMON_COLOR;

	return color;
}

static std::string GetItemTypeText(ItemType type) {
	std::string result;

	switch (type) {
	case ItemType::Equipment:
		result = "Equipment";
		break;
	case ItemType::Armor:
		result = "Armor";
		break;
	case ItemType::Head:
		result = "Head";
		break;
	case ItemType::Chest:
		result = "Chest";
		break;
	case ItemType::Pants:
		result = "Pants";
		break;
	case ItemType::Gloves:
		result = "Gloves";
		break;
	case ItemType::Boots:
		result = "Boots";
		break;
	case ItemType::Weapon:
		result = "Weapon";
		break;
	case ItemType::Sword:
		result = "Sword";
		break;
	case ItemType::Bow:
		result = "Bow";
		break;
	case ItemType::Staff:
		result = "Staff";
		break;
	case ItemType::Daeger:
		result = "Daeger";
		break;
	case ItemType::Consume:
		result = "Consume";
		break;
	case ItemType::Potion:
		result = "Potion";
		break;
	case ItemType::HealthPotion:
		result = "HealthPotion";
		break;
	case ItemType::ManaPotion:
		result = "ManaPotion";
		break;
	default:
		result = "Others";
		break;
	}

	return result;
}

#endif //PCH_H