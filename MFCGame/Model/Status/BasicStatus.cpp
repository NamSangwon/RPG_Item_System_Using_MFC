#include "pch.h"
#include "BasicStatus.h"

void CBasicStatus::SetCurrentHealth(int health)
{
	this->m_nCurrentHealth = health;
}
int CBasicStatus::GetCurrentHealth()
{
	return this->m_nCurrentHealth;
}

void CBasicStatus::SetCurrentMana(int mana)
{
	this->m_nCurrentMana = mana;
}

int CBasicStatus::GetCurrentMana()
{
	return this->m_nCurrentMana;
}
void CBasicStatus::SetMaxHealth(int health)
{
	this->m_nMaxHealth = health;
}
int CBasicStatus::GetMaxHealth()
{
	return this->m_nMaxHealth;
}

void CBasicStatus::SetMaxMana(int mana)
{
	this->m_nMaxMana = mana;
}

int CBasicStatus::GetMaxMana()
{
	return this->m_nMaxMana;
}

void CBasicStatus::SetStrength(int strength)
{
	this->m_nStrength = strength;
}

int CBasicStatus::GetStrength()
{
	return this->m_nStrength;
}

void CBasicStatus::SetDexterity(int dexterity)
{
	this->m_nDexterity = dexterity;
}

int CBasicStatus::GetDexterity()
{
	return this->m_nDexterity;
}

void CBasicStatus::SetIntelligence(int intelligence)
{
	this->m_nIntelligence = intelligence;
}

int CBasicStatus::GetIntelligence()
{
	return this->m_nIntelligence;
}

void CBasicStatus::SetLuck(int luck)
{
	this->m_nLuck = luck;
}

int CBasicStatus::GetLuck()
{
	return this->m_nLuck;
}