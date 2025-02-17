#pragma once
#include "NSW_Item.h"
#include "../Ability/AbilityData.h"

class CNSW_Equipment : public CNSW_Item
{
public:
	//CNSW_Equipment() :
	//	CNSW_Item(),
	//	m_nTotalDurability(U_NONE),
	//	m_nCurrentDurability(U_NONE),
	//	m_pAdditionalData(nullptr),
	//	m_pSpecialData(nullptr) {}

	CNSW_Equipment(const std::shared_ptr<CNSW_Equipment>& equipment) :
		CNSW_Item(equipment->m_iItemIdentifier, equipment->m_strFilePath, equipment->m_strName, equipment->m_nItemType, equipment->m_nWeight, equipment->m_nItemRank),
		m_pAdditionalData(equipment->m_pAdditionalData),
		m_pSpecialData(equipment->m_pSpecialData),
		m_nTotalDurability(equipment->m_nTotalDurability),
		m_nCurrentDurability(equipment->m_nCurrentDurability) {}

	CNSW_Equipment(std::pair<int, int> id,  std::string& file_path, std::string& name, ItemType type, int weight, int rank, int total_durability, int curr_durability, std::shared_ptr<CAbilityData> additional_data = nullptr, std::shared_ptr<CAbilityData> special_data = nullptr) :
		CNSW_Item(id, file_path, name, type, weight, rank),
		m_pAdditionalData(additional_data),
		m_pSpecialData(special_data),
		m_nTotalDurability(total_durability),
		m_nCurrentDurability(curr_durability) {}

	virtual ~CNSW_Equipment() = default;

	void SetTotalDurability(int durability);
	int GetTotalDurability();

	void SetCurrentDurability(int durability);
	int GetCurrentDurability();

	virtual std::string ShowInfo();

	void CreateAdditionalData();
	void SetAdditionalData(std::shared_ptr<CAbilityData> pAbilityData);
	std::shared_ptr<CAbilityData> GetAdditionalData();	
	
	void CreateSpecialData();
	void SetSpecialData(std::shared_ptr<CAbilityData> pAbilityData);
	std::shared_ptr<CAbilityData> GetSpecialData();

protected:
	int m_nTotalDurability;
	int m_nCurrentDurability;

	std::shared_ptr<CAbilityData> m_pAdditionalData;
	std::shared_ptr<CAbilityData> m_pSpecialData;
};

