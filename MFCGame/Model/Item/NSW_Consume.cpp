#include "pch.h"
#include "NSW_Consume.h"

void CNSW_Consume::SetAmount(int cnt)
{
	this->m_nAmount = (cnt > m_nMaxCapacity) ? this->m_nAmount = m_nMaxCapacity : cnt;
}

int CNSW_Consume::AddAmountAndGetRemain(int amount)
{
	int nextAmount = m_nAmount + amount;
	SetAmount(nextAmount);

	return (nextAmount > m_nMaxCapacity) ? m_nMaxCapacity : 0;
}

bool CNSW_Consume::IsWithinInAmount(int amount)
{
	return (amount > 0 && amount <= this->m_nAmount);
}

int CNSW_Consume::GetAmount() const
{
	return this->m_nAmount;
}

void CNSW_Consume::SetMaxCapacity(int max)
{
}

int CNSW_Consume::GetMaxCapacity()
{
	return 0;
}

std::string CNSW_Consume::ShowInfo()
{
	std::string result;
	result += ("[ Name ]\n");
	result += m_strName;
	result += "\n\n[ Rank ]\n";
	result += GetItemRankText(this->m_nItemRank);
	result += "\n\n[ Type ]\n";
	result += GetItemTypeText(m_nItemType);
	result += "\n\n[ Amount ]\n";
	result += std::to_string(m_nAmount);
	result += "\n\n[ Weight ]\n";
	result += std::to_string(m_nWeight);

	return result;
}