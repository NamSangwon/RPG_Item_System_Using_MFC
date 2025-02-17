#pragma once
#include "NSW_Equipment.h"

class CNSW_Weapon :public CNSW_Equipment
{
public:
	//CNSW_Weapon() :
	//	CNSW_Equipment(),
	//	m_nAttackPower(U_NONE),
	//	m_nMagicPower(U_NONE),
	//	m_nRange(U_NONE),
	//	m_fAttackSpeed(U_NONE) {}

	CNSW_Weapon(const std::shared_ptr<CNSW_Weapon>& weapon) :
		CNSW_Equipment(weapon->m_iItemIdentifier, weapon->m_strFilePath, weapon->m_strName, weapon->m_nItemType, weapon->m_nWeight, weapon->m_nItemRank, weapon->m_nTotalDurability, weapon->m_nCurrentDurability, weapon->m_pAdditionalData, weapon->m_pSpecialData),
		m_nAttackPower(weapon->m_nAttackPower),
		m_nMagicPower(weapon->m_nMagicPower),
		m_nRange(weapon->m_nRange),
		m_fAttackSpeed(weapon->m_fAttackSpeed) {}
	
	CNSW_Weapon(std::pair<int, int> id,  std::string& file_path, std::string& name, ItemType type, int weight, int rank, int total_durability, int curr_durability, int attack_power, int magic_power, int range, float AttackSpeed, std::shared_ptr<CAbilityData> additional_data=nullptr, std::shared_ptr<CAbilityData> special_data = nullptr) :
		CNSW_Equipment(id, file_path, name, type, weight, rank, total_durability, curr_durability, additional_data, special_data),
		m_nAttackPower(attack_power),
		m_nMagicPower(magic_power),
		m_nRange(range),
		m_fAttackSpeed(AttackSpeed) {}

	virtual ~CNSW_Weapon();

	void SetAttackPower(int attack_power);
	int GetAttackPower();

	void SetMagicPower(int magic_power);
	int GetMagicPower();

	void SetRange(int range);
	int GetRange();

	void SetAttackSpeed(float AttackSpeed);
	float GetAttackSpeed();

	virtual std::string ShowInfo();

protected:
	int m_nAttackPower;
	int m_nMagicPower;
	int m_nRange;
	float m_fAttackSpeed;
};

