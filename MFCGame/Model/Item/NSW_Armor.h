#pragma once
#include "NSW_Equipment.h"

class CNSW_Armor : public CNSW_Equipment
{
public:
	//CNSW_Armor() :
	//	CNSW_Equipment(),
	//	m_nDefence(U_NONE) {}

	CNSW_Armor(const std::shared_ptr<CNSW_Armor>& armor) :
		CNSW_Equipment(armor->m_iItemIdentifier, armor->m_strFilePath, armor->m_strName, armor->m_nItemType, armor->m_nWeight, armor->m_nItemRank, armor->m_nTotalDurability, armor->m_nCurrentDurability, armor->m_pAdditionalData, armor->m_pSpecialData),
		m_nDefence(armor->m_nDefence) {}

	CNSW_Armor(std::pair<int, int> id,  std::string& file_path, std::string& name, ItemType type, int weight, int rank, int total_durability, int curr_durability, int defence, std::shared_ptr<CAbilityData> additional_data = nullptr, std::shared_ptr<CAbilityData> special_data = nullptr) :
		 CNSW_Equipment(id, file_path, name, type, weight, rank, total_durability, curr_durability, additional_data, special_data),
		m_nDefence(defence) {}

	virtual ~CNSW_Armor() = default;

	void SetDefence(int defence);
	int GetDefence() const;

	virtual std::string ShowInfo();

protected:
	int m_nDefence;
};