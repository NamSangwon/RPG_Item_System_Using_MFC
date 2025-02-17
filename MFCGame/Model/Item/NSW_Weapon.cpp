#include "pch.h"
#include "NSW_Weapon.h"

CNSW_Weapon::~CNSW_Weapon()
{
}

void CNSW_Weapon::SetAttackPower(int attack_power)
{
	this->m_nAttackPower = attack_power;
}

int CNSW_Weapon::GetAttackPower()
{
	return this->m_nAttackPower;
}

void CNSW_Weapon::SetMagicPower(int magic_power)
{
	this->m_nMagicPower = magic_power;
}

int CNSW_Weapon::GetMagicPower()
{
	return this->m_nMagicPower;
}

void CNSW_Weapon::SetRange(int range)
{
	this->m_nRange = range;
}

int CNSW_Weapon::GetRange()
{
	return this->m_nRange;
}

void CNSW_Weapon::SetAttackSpeed(float AttackSpeed)
{
	this->m_fAttackSpeed = AttackSpeed;
}
float CNSW_Weapon::GetAttackSpeed()
{
	return this->m_fAttackSpeed;
}

std::string CNSW_Weapon::ShowInfo() {
	std::string result;
	result += ("[ Name ]\n");
	result += m_strName;
	result += "\n\n[ Rank ]\n";
	result += GetItemRankText(this->m_nItemRank);
	result += "\n\n[ Type ]\n";
	result += GetItemTypeText(m_nItemType);
	result += "\n\n[ Durability ]\n";
	result += std::to_string(m_nCurrentDurability) + " / " + std::to_string(m_nTotalDurability);
	result += "\n\n[ Attack Power ]\n";
	result += std::to_string(m_nAttackPower);
	result += "\n\n[ Magic Power ]\n";
	result += std::to_string(m_nMagicPower);
	result += "\n\n[ Range ]\n";
	result += std::to_string(m_nRange);
	result += "\n\n[ Weight ]\n";
	result += std::to_string(m_nWeight);

	return result;
}