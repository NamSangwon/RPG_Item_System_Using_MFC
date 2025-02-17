#pragma once

class CBasicStatus
{
public:
	CBasicStatus():
		m_nCurrentHealth(U_NONE),
		m_nCurrentMana(U_NONE),
		m_nMaxHealth(U_NONE),
		m_nMaxMana(U_NONE),
		m_nStrength(U_NONE),
		m_nDexterity(U_NONE),
		m_nIntelligence(U_NONE),
		m_nLuck(U_NONE) {}

	CBasicStatus(int HP, int MP, int MaxHP, int MaxMP, int STR, int DEX, int INT, int LUK) :
		m_nCurrentHealth(HP),
		m_nCurrentMana(MP),
		m_nMaxHealth(MaxHP),
		m_nMaxMana(MaxMP),
		m_nStrength(STR),
		m_nDexterity(DEX),
		m_nIntelligence(INT),
		m_nLuck(LUK) {}

	~CBasicStatus() = default;

	void SetCurrentHealth(int health);
	int GetCurrentHealth();

	void SetCurrentMana(int mana);
	int GetCurrentMana();

	void SetMaxHealth(int health);
	int GetMaxHealth();

	void SetMaxMana(int mana);
	int GetMaxMana();

	void SetStrength(int strength);
	int GetStrength();

	void SetDexterity(int dexterity);
	int GetDexterity();

	void SetIntelligence(int intelligence);
	int GetIntelligence();

	void SetLuck(int luck);
	int GetLuck();

private:
	int m_nCurrentHealth;
	int m_nCurrentMana;
	int m_nMaxHealth;
	int m_nMaxMana;
	int m_nStrength;
	int m_nDexterity;
	int m_nIntelligence;
	int m_nLuck;
}; 

