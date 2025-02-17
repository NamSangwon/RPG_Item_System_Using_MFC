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

	CBasicStatus m_cBaseBasicStatus; // 기본 기초 스테이터스 (ex. 레벨 업을 통한 영구적인 능력치 향상 등과 같은 상호작용)
	CBasicStatus m_cCalculatedBasicStatus; // 변형 기초 스테이터스 (ex. 아이템 능력치 계산, 피해, 회복 등과 같은 상호작용)

	CAdvancedStatus m_cBaseAdvancedStatus; // 기본 추가 스테이터스 (ex. 레벨 업을 통한 영구적인 능력치 향상 등과 같은 상호작용)
	CAdvancedStatus m_cCalculatedAdvancedStatus; // 변형 추가 스테이터스 (ex. 아이템 능력치 계산, 피해, 회복 등과 같은 상호작용)
};