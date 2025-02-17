#include "pch.h"
#include "AdvancedStatus.h"

void CAdvancedStatus::SetRegenerateHealth(int regenerate_health)
{
	this->m_nRegenerateHP = regenerate_health;
}
int CAdvancedStatus::GetRegenerateHealth()
{
	return this->m_nRegenerateHP;
}

void CAdvancedStatus::SetRegenerateMana(int regenerate_mana)
{
	this->m_nRegenerateMP = regenerate_mana;
}

int CAdvancedStatus::GetRegenerateMana()
{
	return this->m_nRegenerateMP;
}

void CAdvancedStatus::SetAttackPower(int AttackPower)
{
	this->m_nAttackPower = AttackPower;
}

int CAdvancedStatus::GetAttackPower()
{
	return this->m_nAttackPower;
}

void CAdvancedStatus::SetMagicPower(int MagicPower)
{
	this->m_nMagicPower = MagicPower;
}

int CAdvancedStatus::GetMagicPower()
{
	return this->m_nMagicPower;
}

void CAdvancedStatus::SetDefence(int Defence)
{
	this->m_nDefence = Defence;
}

int CAdvancedStatus::GetDefence()
{
	return this->m_nDefence;
}

void CAdvancedStatus::SetCriticalRate(int CriticalRate)
{
	this->m_nCriticalRate = CriticalRate;
}

int CAdvancedStatus::GetCriticalRate()
{
	return this->m_nCriticalRate;
}

void CAdvancedStatus::SetCriticalDMGRate(int CriticalDMGRate)
{
	this->m_nCriticalDMGRate = CriticalDMGRate;
}

int CAdvancedStatus::GetCriticalDMGRate()
{
	return this->m_nCriticalDMGRate;
}

void CAdvancedStatus::SetMoveSpeed(float MoveSpeed)
{
	this->m_nMoveSpeed = MoveSpeed;
}

float CAdvancedStatus::GetMoveSpeed()
{
	return this->m_nMoveSpeed;
}

void CAdvancedStatus::SetAttackSpeed(float AttackSpeed)
{
	this->m_nAttackSpeed = AttackSpeed;
}

float CAdvancedStatus::GetAttackSpeed()
{
	return this->m_nAttackSpeed;
}
