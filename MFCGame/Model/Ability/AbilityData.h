#pragma once

#include "AbilityOption.h"

class CAbilityData
{
public:
	CAbilityData() {}

	CAbilityData(const std::shared_ptr<CAbilityData> data) :
		m_vAbilityOptions(data->m_vAbilityOptions)  {}

	~CAbilityData()
	{
	}

	void AddAbilityOption(std::shared_ptr<CAbilityOption> Ability_option);
	std::shared_ptr<CAbilityOption> GetAbilityOption(int idx);

	int GetAbilitySize();

	bool IsExistedType(int type);

	std::string ShowInfo();

private:
	std::vector<std::shared_ptr<CAbilityOption>> m_vAbilityOptions;
};