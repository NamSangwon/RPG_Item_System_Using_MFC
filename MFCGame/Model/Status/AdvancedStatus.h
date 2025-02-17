#pragma once

class CAdvancedStatus
{
public:
	CAdvancedStatus() :
		m_nRegenerateHP(0),
		m_nRegenerateMP(0),
		m_nAttackPower(10),
		m_nMagicPower(10),
		m_nDefence(10),
		m_nMoveSpeed(1.0f),
		m_nAttackSpeed(1.0f),
		m_nCriticalRate(0),
		m_nCriticalDMGRate(0)  {}

	CAdvancedStatus(int RegenerateHP, int RegenerateMP, int AD, int AP, int DEF, float Speed, float AttackSpeed, int CRI, int CRI_DMG) :
		m_nRegenerateHP(RegenerateHP),
		m_nRegenerateMP(RegenerateMP),
		m_nAttackPower(AD),
		m_nMagicPower(AP),
		m_nDefence(DEF),
		m_nMoveSpeed(Speed),
		m_nAttackSpeed(AttackSpeed),
		m_nCriticalRate(CRI),
		m_nCriticalDMGRate(CRI_DMG)  {}

	~CAdvancedStatus() = default;

	void SetRegenerateHealth(int regenerate_health);
	int GetRegenerateHealth();

	void SetRegenerateMana(int regenerate_mana);
	int GetRegenerateMana();

	void SetAttackPower(int AttackPower);
	int GetAttackPower();

	void SetMagicPower(int MagicPower);
	int GetMagicPower();

	void SetDefence(int Defence);
	int GetDefence();

	void SetCriticalRate(int CriticalRate);
	int GetCriticalRate();

	void SetCriticalDMGRate(int CriticalDMGRate);
	int GetCriticalDMGRate();

	void SetMoveSpeed(float MoveSpeed);
	float GetMoveSpeed();

	void SetAttackSpeed(float AttackSpeed);
	float GetAttackSpeed();

private:
	int m_nRegenerateHP;
	int m_nRegenerateMP;
	int m_nAttackPower; // print absolute value
	int m_nMagicPower; // print absolute value
	int m_nDefence; // print absolute value
	int m_nCriticalRate; // print % value
	int m_nCriticalDMGRate; // print % value
	float m_nMoveSpeed; // print absolute value
	float m_nAttackSpeed; // print absolute value
};

