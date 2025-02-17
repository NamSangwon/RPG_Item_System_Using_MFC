#pragma once
#include "BaseCharacter.h"

#include "../Model/Inventory.h"
#include "../Model/Equipment.h"
#include "../Model/UserStatus.h"
#include "../Model/Item/NSW_Potion.h"


class CUserCharacter : public CBaseCharacter
{
public:
	CUserCharacter(std::shared_ptr<CInventory>& user_inventory, std::shared_ptr<CEquipment>& user_equipment, std::shared_ptr<CUserStatus>& user_status) : CBaseCharacter()
	{
		this->m_nGenderType = GenderType::Err;
		this->m_nCharacterType = CharacterType::User;
		this->m_strName = _T("Player");
		this->m_CharacterClass = CharacterClass::Citizen;

		m_pInventory = user_inventory;
		m_pEquipment = user_equipment;
		m_pUserStatus = user_status;
	}
	~CUserCharacter() = default;

	// User Interact
	void SetCharacterClass(CharacterClass type);
	CharacterClass GetCharacterClass();

	// Inventory Interact
	void AddItem(std::shared_ptr<CNSW_Item> item);
	void InsertItem(std::shared_ptr<CNSW_Item> item, int location_idx, int slot_idx);
	void SwapItem(int src_inven, int src_slot, int dest_inven, int dest_slot);
	std::map<std::pair<int, int>, std::shared_ptr<CNSW_Item>> GetInventoryItems();
	void DiscardItem(int location_idx, int slot_idx);

	// Equipment Interact
	void EquipItem(int location_idx, int slot_idx, int equipment_slot);
	void UnequipItem(int location_idx, int slot_idx, int equipment_slot);
	void InsertItem(std::shared_ptr<CNSW_Item> item, int slot_idx);
	std::map<std::pair<int, int>, std::shared_ptr<CNSW_Item>> GetEquipmentItems();
	void DiscardItem(int slot_idx);

	// Status Interact
	void InitializeBasicStatus(int HP, int MP, int MaxHP, int MaxMP, int STR, int DEX, int INT, int LUK);
	void InitializeAdvancedStatus(int RegenerateHP, int RegenerateMP, int AttackPower, int MagicPower, int Defence, float MoveSpeed, float AttackSpeed, int CriticalRate, int CriticalDamage);
	void AddBasicStatusValue(int HP, int MP, int MaxHP, int MaxMP, int STR, int DEX, int INT, int LUK);
	void AddAdvancedStatusValue(int RegenerateHP, int RegenerateMP, int AttackPower, int MagicPower, int Defence, float MoveSpeed, float AttackSpeed, int CriticalRate, int CriticalDamage);
	std::shared_ptr<CUserStatus> GetUserStatus();
	void UpdateStatus();

	// for Observer Pattern
	void RegistStatus(IObserver* observer);
	void RegistInventory(IObserver* observer);
	void RegistEquipment(IObserver* observer);

private:
	bool IsValidInventoryIndex(int location_idx, int slot_idx);
	bool IsValidEquipmentIndex(int index);
	bool IsValidEquipmentSlot(ItemType type, int slot);

	CharacterClass m_CharacterClass;

	std::shared_ptr<CInventory> m_pInventory;
	std::shared_ptr<CEquipment> m_pEquipment;
	std::shared_ptr<CUserStatus> m_pUserStatus;
};