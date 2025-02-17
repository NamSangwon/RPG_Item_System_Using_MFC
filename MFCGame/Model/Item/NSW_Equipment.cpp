#include "pch.h"
#include "NSW_Equipment.h"

void CNSW_Equipment::SetTotalDurability(int durability)
{
	this->m_nTotalDurability = durability;
}

int CNSW_Equipment::GetTotalDurability()
{
	return this->m_nTotalDurability;
}

void CNSW_Equipment::SetCurrentDurability(int durability)
{
	this->m_nCurrentDurability = durability;
}

int CNSW_Equipment::GetCurrentDurability()
{
	return this->m_nCurrentDurability;
}

std::string CNSW_Equipment::ShowInfo()
{
	std::string result;
	result += ("[ Name ]\n");
	result += m_strName;
	result += "\n\n[ Rank ]\n";
	result += GetItemRankText(this->m_nItemRank);
	result += "\n\n[ Type ]\n";
	result += GetItemTypeText(m_nItemType);
	result += "\n\n[ Durability ]\n";
	result += std::to_string(m_nCurrentDurability) + " / " + std::to_string(m_nTotalDurability);
	result += "\n\n[ Weight ]\n";
	result += std::to_string(m_nWeight);

	return result;
}

void CNSW_Equipment::CreateAdditionalData()
{
	this->m_pAdditionalData = std::make_shared<CAbilityData>();
}

void CNSW_Equipment::SetAdditionalData(std::shared_ptr<CAbilityData> pAbilityData)
{
	this->m_pAdditionalData = std::move(pAbilityData);
}

std::shared_ptr<CAbilityData> CNSW_Equipment::GetAdditionalData()
{
	return this->m_pAdditionalData;
}

void CNSW_Equipment::CreateSpecialData()
{
	this->m_pSpecialData = std::make_shared<CAbilityData>();
}

void CNSW_Equipment::SetSpecialData(std::shared_ptr<CAbilityData> pAbilityData)
{
	this->m_pSpecialData = std::move(pAbilityData);
}

std::shared_ptr<CAbilityData> CNSW_Equipment::GetSpecialData()
{
	return this->m_pSpecialData;
}
