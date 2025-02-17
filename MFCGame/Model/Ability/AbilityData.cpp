#include "pch.h"
#include "AbilityData.h"

void CAbilityData::AddAbilityOption(std::shared_ptr<CAbilityOption> Ability_option)
{
	m_vAbilityOptions.push_back(std::move(Ability_option));
}

std::shared_ptr<CAbilityOption> CAbilityData::GetAbilityOption(int idx)
{
	return this->m_vAbilityOptions[idx];
}

int CAbilityData::GetAbilitySize()
{
	return m_vAbilityOptions.size();
}

bool CAbilityData::IsExistedType(int type)
{
	AbilityType ability_type = static_cast<AbilityType>(type);

	for (auto& cell : m_vAbilityOptions) {
		if (cell->GetAbilityType() == ability_type)
			return true;
	}

	return false;
}

std::string CAbilityData::ShowInfo()
{
	std::string result;

	for (int i = 0; i < m_vAbilityOptions.size(); i++) {
		int ability_type = static_cast<int>(m_vAbilityOptions[i]->GetAbilityType());

		std::vector<int> ranks;
		int rate_rank = GetAbilityOption(i)->GetRateRank();
		int value_rank = GetAbilityOption(i)->GetValueRank();

		// Rate 값이 있을 시 추가
		if (rate_rank > 0) 
			ranks.push_back(rate_rank);
		// Value 값이 있을 시 추가
		if (value_rank > 0)
			ranks.push_back(value_rank);

		// Ability Rate 값과 Value 값의 옵션 평균
		int option_rank = 0;
		for (auto& cell : ranks)
			option_rank += cell;
		option_rank /= (float)ranks.size();

		result += std::format("[ Tier {} ]\n", option_rank);
		result += m_vAbilityOptions[i]->GetAbilityInfo() + "\n\n";
	}

	return result;
}
