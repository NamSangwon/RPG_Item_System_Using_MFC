#include "pch.h"
#include "NSW_Potion.h"

void CNSW_Potion::SetRecoverAmount(int recover_amount)
{
	this->m_nRecoverAmount = recover_amount;
}

int CNSW_Potion::GetRecoverAmount()
{
	return this->m_nRecoverAmount;
}


std::string CNSW_Potion::ShowInfo() {
	std::string result;
	result += ("[ Name ]\n");
	result += m_strName;
	result += "\n\n[ Rank ]\n";
	result += GetItemRankText(this->m_nItemRank);
	result += "\n\n[ Type ]\n";
	result += GetItemTypeText(m_nItemType);
	result += "\n\n[ Recover Amount ]\n";
	result += std::to_string(m_nRecoverAmount);
	result += "\n\n[ Amount ]\n";
	result += std::to_string(m_nAmount);
	result += "\n\n[ Weight ]\n";
	result += std::to_string(m_nWeight);

	return result;
}