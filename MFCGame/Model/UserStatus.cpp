#include "pch.h"

#include "UserStatus.h"
#include "../Dialog/Main/StatusDlg.h"

void CUserStatus::SetBaseBasicStatus(int HP, int MP, int MaxHP, int MaxMP, int STR, int DEX, int INT, int LUK)
{
	m_cBaseBasicStatus = CBasicStatus(HP, MP, MaxHP, MaxMP, STR, DEX, INT, LUK);

	// 스테이터스 창의 value 업데이트를 위해 알림 전송
	UserStatusBarUpdate();
	BasicStatusUpdate();
}

void CUserStatus::AddBaseBasicStatus(int HP, int MP, int MaxHP, int MaxMP, int STR, int DEX, int INT, int LUK)
{
	int prevCurrentHP = m_cBaseBasicStatus.GetCurrentHealth();
	int prevCurrentMP = m_cBaseBasicStatus.GetCurrentMana();
	int prevMaxHP = m_cBaseBasicStatus.GetMaxHealth();
	int prevMaxMP = m_cBaseBasicStatus.GetMaxMana();

	m_cBaseBasicStatus = CBasicStatus(
		prevCurrentHP + HP < 0 ? 0 : prevCurrentHP + HP > prevMaxHP ? prevMaxHP : prevCurrentHP + HP,
		prevCurrentMP + MP < 0 ? 0 : prevCurrentMP + MP > prevMaxMP ? prevMaxMP : prevCurrentMP + MP,
		prevMaxHP + MaxHP,
		prevMaxMP + MaxMP,
		m_cBaseBasicStatus.GetStrength() + STR,
		m_cBaseBasicStatus.GetDexterity() + DEX,
		m_cBaseBasicStatus.GetIntelligence() + INT,
		m_cBaseBasicStatus.GetLuck() + LUK
	);

	// 스테이터스 창의 value 업데이트를 위해 알림 전송
	UserStatusBarUpdate();
	BasicStatusUpdate();
}

CBasicStatus CUserStatus::GetBaseBasicStatus()
{
	return m_cBaseBasicStatus;
}

void CUserStatus::SetCalculatedBasicStatus(int HP, int MP, int MaxHP, int MaxMP, int STR, int DEX, int INT, int LUK)
{
	m_cCalculatedBasicStatus = CBasicStatus(HP, MP, MaxHP, MaxMP, STR, DEX, INT, LUK);

	// 스테이터스 창의 value 업데이트를 위해 알림 전송
	UserStatusBarUpdate();
	BasicStatusUpdate();
}

void CUserStatus::AddCalculatedBasicStatus(int HP, int MP, int MaxHP, int MaxMP, int STR, int DEX, int INT, int LUK)
{
	int prevCurrentHP = m_cCalculatedBasicStatus.GetCurrentHealth();
	int prevCurrentMP = m_cCalculatedBasicStatus.GetCurrentMana();
	int prevMaxHP = m_cCalculatedBasicStatus.GetMaxHealth();
	int prevMaxMP = m_cCalculatedBasicStatus.GetMaxMana();

	m_cCalculatedBasicStatus = CBasicStatus(
		prevCurrentHP + HP < 0 ? 0 : prevCurrentHP + HP > prevMaxHP ? prevMaxHP : prevCurrentHP + HP,
		prevCurrentMP + MP < 0 ? 0 : prevCurrentMP + MP > prevMaxMP ? prevMaxMP : prevCurrentMP + MP,
		prevMaxHP + MaxHP,
		prevMaxMP + MaxMP,
		m_cCalculatedBasicStatus.GetStrength() + STR,
		m_cCalculatedBasicStatus.GetDexterity() + DEX,
		m_cCalculatedBasicStatus.GetIntelligence() + INT,
		m_cCalculatedBasicStatus.GetLuck() + LUK
	);

	// 스테이터스 창의 value 업데이트를 위해 알림 전송
	UserStatusBarUpdate();
	BasicStatusUpdate();
}

CBasicStatus CUserStatus::GetCalculatedBasicStatus()
{
	return m_cCalculatedBasicStatus;
}

void CUserStatus::SetBaseAdvancedStatus(int RegenerateHP, int RegenerateMP, int AttackPower, int MagicPower, int Defence, float MoveSpeed, float AttackSpeed, int CriticalRate, int CriticalDamage)
{
	m_cBaseAdvancedStatus = CAdvancedStatus(RegenerateHP, RegenerateMP, AttackPower, MagicPower, Defence, MoveSpeed, AttackSpeed, CriticalRate, CriticalDamage);

	// 스테이터스 창의 value 업데이트를 위해 알림 전송
	AdvancedStatusUpdate();
}

void CUserStatus::AddBaseAdvancedStatus(int RegenerateHP, int RegenerateMP, int AttackPower, int MagicPower, int Defence, float MoveSpeed, float AttackSpeed, int CriticalRate, int CriticalDamage)
{
	m_cBaseAdvancedStatus = CAdvancedStatus(
		m_cBaseAdvancedStatus.GetRegenerateHealth() + RegenerateHP,
		m_cBaseAdvancedStatus.GetRegenerateMana() + RegenerateMP,
		m_cBaseAdvancedStatus.GetAttackPower() + AttackPower,
		m_cBaseAdvancedStatus.GetMagicPower() + MagicPower,
		m_cBaseAdvancedStatus.GetDefence() + Defence,
		m_cBaseAdvancedStatus.GetMoveSpeed() + MoveSpeed,
		m_cBaseAdvancedStatus.GetAttackSpeed() + AttackSpeed,
		m_cBaseAdvancedStatus.GetCriticalRate() + CriticalRate,
		m_cBaseAdvancedStatus.GetCriticalDMGRate() + CriticalDamage
	);

	// 스테이터스 창의 value 업데이트를 위해 알림 전송
	AdvancedStatusUpdate();
}

CAdvancedStatus CUserStatus::GetBaseAdvancedStatus()
{
	return m_cBaseAdvancedStatus;
}

void CUserStatus::SetCalculatedAdvancedStatus(int RegenerateHP, int RegenerateMP, int AttackPower, int MagicPower, int Defence, float MoveSpeed, float AttackSpeed, int CriticalRate, int CriticalDamage)
{
	m_cCalculatedAdvancedStatus = CAdvancedStatus(RegenerateHP, RegenerateMP, AttackPower, MagicPower, Defence, MoveSpeed, AttackSpeed, CriticalRate, CriticalDamage);

	// 스테이터스 창의 value 업데이트를 위해 알림 전송
	AdvancedStatusUpdate();
}

void CUserStatus::AddCalculatedAdvancedStatus(int RegenerateHP, int RegenerateMP, int AttackPower, int MagicPower, int Defence, float MoveSpeed, float AttackSpeed, int CriticalRate, int CriticalDamage)
{
	m_cCalculatedAdvancedStatus = CAdvancedStatus(
		m_cCalculatedAdvancedStatus.GetRegenerateHealth() + RegenerateHP,
		m_cCalculatedAdvancedStatus.GetRegenerateMana() + RegenerateMP,
		m_cCalculatedAdvancedStatus.GetAttackPower() + AttackPower,
		m_cCalculatedAdvancedStatus.GetMagicPower() + MagicPower,
		m_cCalculatedAdvancedStatus.GetDefence() + Defence,
		m_cCalculatedAdvancedStatus.GetMoveSpeed() + MoveSpeed,
		m_cCalculatedAdvancedStatus.GetAttackSpeed() + AttackSpeed,
		m_cCalculatedAdvancedStatus.GetCriticalRate() + CriticalRate,
		m_cCalculatedAdvancedStatus.GetCriticalDMGRate() + CriticalDamage
	);

	// 스테이터스 창의 value 업데이트를 위해 알림 전송
	AdvancedStatusUpdate();
}

CAdvancedStatus CUserStatus::GetCalculatedAdvancedStatus()
{
	return m_cCalculatedAdvancedStatus;
}

void CUserStatus::CalculateStatus(std::vector<std::shared_ptr<CNSW_Item>>& vItems)
{
	m_cCalculatedBasicStatus = m_cBaseBasicStatus;
	m_cCalculatedAdvancedStatus = m_cBaseAdvancedStatus;

	for (auto& item : vItems) {
		std::shared_ptr<CNSW_Equipment> pEquipment = std::dynamic_pointer_cast<CNSW_Equipment>(item);

		// Additional Ability 옵션 처리 (Character Base Status에 비례해서 Rate 계산)
		std::shared_ptr<CAbilityData> pAdditionalData = pEquipment->GetAdditionalData();
		if (pAdditionalData != nullptr) {
			for (int i = 0; i < pAdditionalData->GetAbilitySize(); i++) {
				if (pAdditionalData->GetAbilityOption(i)->GetTargetType() == TargetType::Own) {
					AbilityType type = pAdditionalData->GetAbilityOption(i)->GetAbilityType();
					int value = pAdditionalData->GetAbilityOption(i)->GetValue();
					CalculateAbilityOption(type, value);
				}
			}
		}

		// Special Ability 옵션 처리 (Character Base Status에 비례해서 Rate 계산)
		std::shared_ptr<CAbilityData> pSpecialData = pEquipment->GetSpecialData();
		if (pSpecialData != nullptr) {
			for (int i = 0; i < pSpecialData->GetAbilitySize(); i++) {
				if (pSpecialData->GetAbilityOption(i)->GetTargetType() == TargetType::Own) {
					AbilityType type = pSpecialData->GetAbilityOption(i)->GetAbilityType();
					int value = pSpecialData->GetAbilityOption(i)->GetValue();
					CalculateAbilityOption(type, value);
				}
			}
		}

		// 방어구 아이템 기본 옵션 처리
		if (pEquipment->IsArmor()) {
			std::shared_ptr<CNSW_Armor> pArmor = std::dynamic_pointer_cast<CNSW_Armor>(pEquipment);
			m_cCalculatedAdvancedStatus.SetDefence(pArmor->GetDefence() + m_cCalculatedAdvancedStatus.GetDefence());
		}
		// 무기 아이템 기본 옵션 처리
		else if (pEquipment->IsWeapon()) {
			std::shared_ptr<CNSW_Weapon> pWeapon = std::dynamic_pointer_cast<CNSW_Weapon>(pEquipment);
			m_cCalculatedAdvancedStatus.SetAttackPower(pWeapon->GetAttackPower() + m_cCalculatedAdvancedStatus.GetAttackPower());
			m_cCalculatedAdvancedStatus.SetMagicPower(pWeapon->GetMagicPower() + m_cCalculatedAdvancedStatus.GetMagicPower());
			m_cCalculatedAdvancedStatus.SetAttackSpeed(pWeapon->GetAttackSpeed() * m_cCalculatedAdvancedStatus.GetAttackSpeed());
		}
	}

	// 스테이터스 창의 value 업데이트를 위해 알림 전송
	UserStatusBarUpdate();
	BasicStatusUpdate();
	AdvancedStatusUpdate();
}





void CUserStatus::CalculateAbilityOption(AbilityType type, int value)
{
	int increase_value = U_NONE;

	switch (type) {
		case AbilityType::HP_Rate:
			increase_value = value * m_cBaseBasicStatus.GetMaxHealth() / 100; // 증가량
			m_cCalculatedBasicStatus.SetMaxHealth(m_cCalculatedBasicStatus.GetMaxHealth() + increase_value);
			m_cCalculatedBasicStatus.SetCurrentHealth(m_cCalculatedBasicStatus.GetCurrentHealth() + increase_value);
			break;
		case AbilityType::HP_Val:
			m_cCalculatedBasicStatus.SetMaxHealth(m_cCalculatedBasicStatus.GetMaxHealth() + value);
			m_cCalculatedBasicStatus.SetCurrentHealth(m_cCalculatedBasicStatus.GetCurrentHealth() + value);
			break;
		case AbilityType::MP_Rate:
			increase_value = value * m_cBaseBasicStatus.GetMaxMana() / 100; // 증가량
			m_cCalculatedBasicStatus.SetMaxMana(m_cCalculatedBasicStatus.GetMaxMana() + increase_value);
			m_cCalculatedBasicStatus.SetCurrentMana(m_cCalculatedBasicStatus.GetCurrentMana() + increase_value);
			break;
		case AbilityType::MP_Val:
			m_cCalculatedBasicStatus.SetMaxMana(m_cCalculatedBasicStatus.GetMaxMana() + value);
			m_cCalculatedBasicStatus.SetCurrentMana(m_cCalculatedBasicStatus.GetCurrentMana() + value);
			break;
		case AbilityType::STR_Val:
			m_cCalculatedBasicStatus.SetStrength(m_cCalculatedBasicStatus.GetStrength() + value);
			break;
		case AbilityType::DEX_Val:
			m_cCalculatedBasicStatus.SetDexterity(m_cCalculatedBasicStatus.GetDexterity() + value);
			break;
		case AbilityType::INT_Val:
			m_cCalculatedBasicStatus.SetIntelligence(m_cCalculatedBasicStatus.GetIntelligence() + value);
			break;
		case AbilityType::LUK_Val:
			m_cCalculatedBasicStatus.SetLuck(m_cCalculatedBasicStatus.GetLuck() + value);
			break;
		case AbilityType::STR_Rate:
			m_cCalculatedBasicStatus.SetStrength(m_cCalculatedBasicStatus.GetStrength() + value * m_cBaseBasicStatus.GetStrength() / 100);
			break;
		case AbilityType::DEX_Rate:
			m_cCalculatedBasicStatus.SetDexterity(m_cCalculatedBasicStatus.GetDexterity() + value * m_cBaseBasicStatus.GetDexterity() / 100);
			break;
		case AbilityType::INT_Rate:
			m_cCalculatedBasicStatus.SetIntelligence(m_cCalculatedBasicStatus.GetIntelligence() + value * m_cBaseBasicStatus.GetIntelligence() / 100);
			break;
		case AbilityType::LUK_Rate:
			m_cCalculatedBasicStatus.SetLuck(m_cCalculatedBasicStatus.GetLuck() + value * m_cBaseBasicStatus.GetLuck() / 100);
			break;

		case AbilityType::MoveSpeed:
			m_cCalculatedAdvancedStatus.SetMoveSpeed(m_cCalculatedAdvancedStatus.GetMoveSpeed() + value * m_cBaseAdvancedStatus.GetMoveSpeed() / 100);
			break;
		case AbilityType::DEF_Rate:
			m_cCalculatedAdvancedStatus.SetDefence(m_cCalculatedAdvancedStatus.GetDefence() + value * m_cBaseAdvancedStatus.GetDefence() / 100);
			break;
		case AbilityType::DEF_Val:
			m_cCalculatedAdvancedStatus.SetDefence(m_cCalculatedAdvancedStatus.GetDefence() + value);
			break;
		case AbilityType::ATK_Rate:
			m_cCalculatedAdvancedStatus.SetAttackPower(m_cCalculatedAdvancedStatus.GetAttackPower() + value * m_cBaseAdvancedStatus.GetAttackPower() / 100);
			break;
		case AbilityType::ATK_Val:
			m_cCalculatedAdvancedStatus.SetAttackPower(m_cCalculatedAdvancedStatus.GetAttackPower() + value);
			break;
		case AbilityType::Magic_ATK_Rate:
			m_cCalculatedAdvancedStatus.SetMagicPower(m_cCalculatedAdvancedStatus.GetMagicPower() + value * m_cBaseAdvancedStatus.GetMagicPower() / 100);
			break;
		case AbilityType::Magic_ATK_Val:
			m_cCalculatedAdvancedStatus.SetMagicPower(m_cCalculatedAdvancedStatus.GetMagicPower() + value);
			break;
		case AbilityType::AttackSpeed:
			m_cCalculatedAdvancedStatus.SetAttackSpeed(m_cCalculatedAdvancedStatus.GetAttackSpeed() + value * m_cBaseAdvancedStatus.GetAttackSpeed() / 100);
			break;
		case AbilityType::CriticalRate:
			m_cCalculatedAdvancedStatus.SetCriticalRate(m_cCalculatedAdvancedStatus.GetCriticalRate() + value);
			break;
		case AbilityType::CriticalDamage:
			m_cCalculatedAdvancedStatus.SetCriticalDMGRate(m_cCalculatedAdvancedStatus.GetCriticalDMGRate() + value);
			break;
		case AbilityType::RegenerateHP:
			m_cCalculatedAdvancedStatus.SetRegenerateHealth(m_cCalculatedAdvancedStatus.GetRegenerateHealth() + value);
			break;
		case AbilityType::RegenerateMP:
			m_cCalculatedAdvancedStatus.SetRegenerateMana(m_cCalculatedAdvancedStatus.GetRegenerateMana() + value);
			break;
	}
}

void CUserStatus::UserStatusBarUpdate()
{
	NotifyUpdateBasicStatus(
		m_cCalculatedBasicStatus.GetCurrentHealth(),
		m_cCalculatedBasicStatus.GetCurrentMana(),
		m_cCalculatedBasicStatus.GetMaxHealth(),
		m_cCalculatedBasicStatus.GetMaxMana(),
		m_cCalculatedBasicStatus.GetStrength(),
		m_cCalculatedBasicStatus.GetDexterity(),
		m_cCalculatedBasicStatus.GetIntelligence(),
		m_cCalculatedBasicStatus.GetLuck()
	);
}

void CUserStatus::BasicStatusUpdate()
{
	// Basic Status Update
	NotifyUpdateBasicStatus(
		m_cCalculatedBasicStatus.GetCurrentHealth(),
		m_cCalculatedBasicStatus.GetCurrentMana(), 
		m_cCalculatedBasicStatus.GetMaxHealth(),
		m_cCalculatedBasicStatus.GetMaxMana(),
		m_cCalculatedBasicStatus.GetStrength(),
		m_cCalculatedBasicStatus.GetDexterity(),
		m_cCalculatedBasicStatus.GetIntelligence(),
		m_cCalculatedBasicStatus.GetLuck()
	);
}

void CUserStatus::AdvancedStatusUpdate()
{
	// Advanced Update
	NotifyUpdateAdvancedStatus(
		m_cCalculatedAdvancedStatus.GetRegenerateHealth(),
		m_cCalculatedAdvancedStatus.GetRegenerateMana(),
		m_cCalculatedAdvancedStatus.GetAttackPower(),
		m_cCalculatedAdvancedStatus.GetMagicPower(),
		m_cCalculatedAdvancedStatus.GetDefence(),
		m_cCalculatedAdvancedStatus.GetMoveSpeed(),
		m_cCalculatedAdvancedStatus.GetAttackSpeed(),
		m_cCalculatedAdvancedStatus.GetCriticalRate(),
		m_cCalculatedAdvancedStatus.GetCriticalDMGRate()
	);
}
