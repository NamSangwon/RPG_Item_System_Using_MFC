#include "pch.h"
#include "NSW_Armor.h"

void CNSW_Armor::SetDefence(int defence)
{
	this->m_nDefence = defence;
}

int CNSW_Armor::GetDefence() const
{
	return this->m_nDefence;
}

std::string CNSW_Armor::ShowInfo() {
	std::string result;
	result += ("[ Name ]\n");
	result += m_strName;
	result += "\n\n[ Rank ]\n";
	result += GetItemRankText(this->m_nItemRank);
	result += "\n\n[ Type ]\n";
	result += GetItemTypeText(m_nItemType);
	result += "\n\n[ Durability ]\n";
	result += std::to_string(m_nCurrentDurability) + " / " + std::to_string(m_nTotalDurability);
	result += "\n\n[ Defence ]\n";
	result += std::to_string(m_nDefence);
	result += "\n\n[ Weight ]\n";
	result += std::to_string(m_nWeight);

	return result;
}
