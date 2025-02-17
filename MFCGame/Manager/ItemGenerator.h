#pragma once
#include "../Interface/SingletonTemplate.h"

#include "../Model/Item/NSW_Armor.h"
#include "../Model/Item/NSW_Weapon.h"
#include "../Model/Item/NSW_Potion.h"

class CItemGenerator : public SingletonTemplate<CItemGenerator>
{
public:
	friend class SingletonTemplate<CItemGenerator>;

	std::shared_ptr<CNSW_Item> GenerateRandomItem();	

	std::shared_ptr<CNSW_Item> ConstructItem(int generate_idx, int item_idx, int defence, int amount, int durability, int attack_power, int magic_power);
	std::shared_ptr<CAbilityOption> ConstructAbilityOption(int ability_index, int ability_rate, int ability_value);

private:
	CItemGenerator() : m_fWeightDecision(0.7f) {}
	~CItemGenerator() {}

	int uniform_random_value(int min, int max);
	int exp_random_value(int min, int max, float weight_decision);

	// generate_idx == Item Type Index In Generate File
	// item_idx == Item Index In Each Item File
	std::shared_ptr<CNSW_Armor> GenerateArmor(int generate_idx, int item_idx, std::vector<std::string> vItemData, int defence, int curr_durability = U_NONE);
	std::shared_ptr<CNSW_Weapon> GenerateWeapon(int generate_idx, int item_idx, std::vector<std::string> vItemData, int attack_power, int magic_power, int curr_durability = U_NONE);
	std::shared_ptr<CNSW_Potion> GeneratePotion(int generate_idx, int item_idx, std::vector<std::string> vItemData, int amount);

	void GenerateRandomAbility(std::pair<ItemType, ItemType> item_type_range, std::shared_ptr<CAbilityData>& additional, std::shared_ptr<CAbilityData>& special);
	std::shared_ptr<CAbilityOption> GenerateAbilityOption(std::vector<std::string> vItemData, int value, int rate);

	std::vector<std::vector<std::string>> ReadAllItem(std::string file_path);
	std::vector<std::string> ReadSelectedItem(std::string file_path, int idx);
	std::vector<std::string> RandomSelectItem(std::vector<std::vector<std::string>>& file_data);

	std::vector<std::string> SplitString(std::string str, char Delimiter);

	struct ItemStringData {
		std::string name;
		std::string image_path;
	};
	std::map<std::pair<int, int>, ItemStringData> m_ItemStringPool;
	std::map<int, std::string> m_AbilityCommentPool;

	float m_fWeightDecision;

	enum GenerateColumn { FileIndex, FilePath, FileProb };
	enum AbilityColumn { AbilityIndex, AbilityName, TypeOfAbility, MinRate, MaxRate, MinValue, MaxValue, Target, AvailableItemType, ActivationRange, ActivationDuration, ActivationTimes, Comment, AbilityProb };
	enum ItemColumn { ItemIndex, TypeOfItem, ItemName, Rank, ImagePath, Weight, Durability, RecoverAmount, MinDefence, MaxDefence, MinAttackPower, MaxAttackPower, MinMagicPower, MaxMagicPower, Range, AttackSpeed, ItemProb };

	std::string prob_for_print;
};
