#pragma once
#include "Status/BasicStatus.h"
#include "Status/AdvancedStatus.h"
#include "../Interface/Subject.h"

#include "../Model/Item/NSW_Armor.h"
#include "../Model/Item/NSW_Weapon.h"

class CUserStatus : public ISubject
{
public:
	CUserStatus() = default;
	~CUserStatus() = default;

	CBasicStatus GetBaseBasicStatus();
	CBasicStatus GetCalculatedBasicStatus();
	CAdvancedStatus GetBaseAdvancedStatus();
	CAdvancedStatus GetCalculatedAdvancedStatus();

private:
	friend class CUserCharacter;

	void SetBaseBasicStatus(int HP, int MP, int MaxHP, int MaxMP, int STR, int DEX, int INT, int LUK);
	void AddBaseBasicStatus(int HP, int MP, int MaxHP, int MaxMP, int STR, int DEX, int INT, int LUK);

	void SetCalculatedBasicStatus(int HP, int MP, int MaxHP, int MaxMP, int STR, int DEX, int INT, int LUK);
	void AddCalculatedBasicStatus(int HP, int MP, int MaxHP, int MaxMP, int STR, int DEX, int INT, int LUK);

	void SetBaseAdvancedStatus(int RegenerateHP, int RegenerateMP, int AttackPower, int MagicPower, int Defence, float MoveSpeed, float AttackSpeed, int CriticalRate, int CriticalDamage);
	void AddBaseAdvancedStatus(int RegenerateHP, int RegenerateMP, int AttackPower, int MagicPower, int Defence, float MoveSpeed, float AttackSpeed, int CriticalRate, int CriticalDamage);

	void SetCalculatedAdvancedStatus(int RegenerateHP, int RegenerateMP, int AttackPower, int MagicPower, int Defence, float MoveSpeed, float AttackSpeed, int CriticalRate, int CriticalDamage);
	void AddCalculatedAdvancedStatus(int RegenerateHP, int RegenerateMP, int AttackPower, int MagicPower, int Defence, float MoveSpeed, float AttackSpeed, int CriticalRate, int CriticalDamage);

	void CalculateStatus(std::vector<std::shared_ptr<CNSW_Item>>& vItems);
	void CalculateAbilityOption(AbilityType type, int value);

	void UserStatusBarUpdate();
	void BasicStatusUpdate();
	void AdvancedStatusUpdate();

	CBasicStatus m_cBaseBasicStatus; // �⺻ ���� �������ͽ� (ex. ���� ���� ���� �������� �ɷ�ġ ��� ��� ���� ��ȣ�ۿ�)
	CBasicStatus m_cCalculatedBasicStatus; // ���� ���� �������ͽ� (ex. ������ �ɷ�ġ ���, ����, ȸ�� ��� ���� ��ȣ�ۿ�)

	CAdvancedStatus m_cBaseAdvancedStatus; // �⺻ �߰� �������ͽ� (ex. ���� ���� ���� �������� �ɷ�ġ ��� ��� ���� ��ȣ�ۿ�)
	CAdvancedStatus m_cCalculatedAdvancedStatus; // ���� �߰� �������ͽ� (ex. ������ �ɷ�ġ ���, ����, ȸ�� ��� ���� ��ȣ�ۿ�)
};