#include "pch.h"
#include "UserCharacter.h"

void CUserCharacter::SetCharacterClass(CharacterClass type)
{
	if (type >= CharacterClass::Citizen && type <= CharacterClass::Rogue) {
		m_CharacterClass = type;
	}
}

CharacterClass CUserCharacter::GetCharacterClass()
{
	return m_CharacterClass;
}


void CUserCharacter::AddItem(std::shared_ptr<CNSW_Item> item)
{
	std::shared_ptr<CNSW_Item> pRemainItem = nullptr;

	 pRemainItem = m_pInventory->AddItem(item);

	// 인벤토리가 모두 차서 아이템이 남는 경우, 입력 받은 아이템 삭제
	pRemainItem.reset();
}

void CUserCharacter::InsertItem(std::shared_ptr<CNSW_Item> item, int location_idx, int slot_idx)
{
	if (item != nullptr && IsValidInventoryIndex(location_idx, slot_idx)) {
		int index = location_idx * INVENTORY_COL_NUM * INVENTORY_ROW_NUM + slot_idx;

		m_pInventory->InsertItem(std::move(item), index);
	}
}

void CUserCharacter::SwapItem(int src_inven, int src_slot, int dest_inven, int dest_slot)
{
	if (!IsValidInventoryIndex(src_inven, src_slot) || !IsValidInventoryIndex(dest_inven, dest_slot)) return;

	int src_idx = src_inven * INVENTORY_COL_NUM * INVENTORY_ROW_NUM + src_slot;
	std::shared_ptr<CNSW_Item> pSrcItem = m_pInventory->GetItem(src_idx);

	int dext_idx = dest_inven * INVENTORY_COL_NUM * INVENTORY_ROW_NUM + dest_slot;
	std::shared_ptr<CNSW_Item> pDestItem = m_pInventory->GetItem(dext_idx);

	m_pInventory->DeleteItem(src_idx);
	m_pInventory->DeleteItem(dext_idx);

	m_pInventory->InsertItem(std::move(pDestItem), src_idx);
	m_pInventory->InsertItem(std::move(pSrcItem), dext_idx);
}

std::map<std::pair<int, int>, std::shared_ptr<CNSW_Item>> CUserCharacter::GetInventoryItems()
{
	std::map<std::pair<int, int>, std::shared_ptr<CNSW_Item>> inventory_items;

	for (int i = 0; i < INVENTORY_WINDOW_NUM * INVENTORY_COL_NUM * INVENTORY_ROW_NUM; i++) {
		std::shared_ptr<CNSW_Item> pItem = m_pInventory->GetItem(i);

		if (pItem != nullptr) {
			int location_idx = i / (INVENTORY_COL_NUM * INVENTORY_ROW_NUM);
			int slot_idx = i % (INVENTORY_COL_NUM * INVENTORY_ROW_NUM);
			inventory_items[std::make_pair(location_idx, slot_idx)] = pItem;
		}
	}

	return inventory_items;
}

void CUserCharacter::DiscardItem(int location_idx, int slot_idx)
{
	// Inventory Interact
	if (IsValidInventoryIndex(location_idx, slot_idx)) {
		int index = location_idx * INVENTORY_COL_NUM * INVENTORY_ROW_NUM + slot_idx;

		m_pInventory->DeleteItem(index);
	}
}




void CUserCharacter::EquipItem(int location_idx, int inven_slot, int equipment_slot)
{
	if (!IsValidInventoryIndex(location_idx, inven_slot) || !IsValidEquipmentIndex(equipment_slot)) return;

	int index = location_idx * INVENTORY_COL_NUM * INVENTORY_ROW_NUM + inven_slot;
	std::shared_ptr<CNSW_Item> pItem = std::move(m_pInventory->GetItem(index));

	// 장착하고자 하는 아이템이 올바른 아이템 칸에 들어가는 지
	if (IsValidEquipmentSlot(pItem->GetItemType(), equipment_slot)) {
		m_pInventory->DeleteItem(index);

		m_pEquipment->InsertItem(std::move(pItem), equipment_slot);

		UpdateStatus();
	}
	else {
		m_pInventory->InsertItem(pItem, index);
	}
}

void CUserCharacter::UnequipItem(int location_idx, int inven_slot, int equipment_slot)
{

	if (!IsValidInventoryIndex(location_idx, inven_slot) || !IsValidEquipmentIndex(equipment_slot)) return;

	std::shared_ptr<CNSW_Item> pItem = std::move(m_pEquipment->GetItem(equipment_slot));

	m_pEquipment->DeleteItem(equipment_slot);

	int index = location_idx * INVENTORY_COL_NUM * INVENTORY_ROW_NUM + inven_slot;
	m_pInventory->InsertItem(std::move(pItem), index);

	UpdateStatus();

}

void CUserCharacter::InsertItem(std::shared_ptr<CNSW_Item> item, int slot_idx)
{
	if (item != nullptr && IsValidEquipmentIndex(slot_idx)) {
		if (IsValidEquipmentSlot(item->GetItemType(), slot_idx))
			m_pEquipment->InsertItem(std::move(item), slot_idx);

		UpdateStatus();
	}
}

std::map<std::pair<int, int>, std::shared_ptr<CNSW_Item>> CUserCharacter::GetEquipmentItems()
{
	std::map<std::pair<int, int>, std::shared_ptr<CNSW_Item>> equiped_items;

	for (int i = 0; i < EQUIPMENT_PARTS_NUM; i++) {
		std::shared_ptr<CNSW_Item> pItem = m_pEquipment->GetItem(i);

		if (pItem != nullptr) {
			int location_idx = ID_EQUIPMENT;
			int slot_idx = i;
			equiped_items[std::make_pair(location_idx, slot_idx)] = pItem;
		}
	}

	return equiped_items;
}

void CUserCharacter::DiscardItem(int slot_idx)
{
	// Equipment Interact
	if (IsValidEquipmentIndex(slot_idx)) {
		m_pEquipment->DeleteItem(slot_idx);

		UpdateStatus();
	}
}




void CUserCharacter::InitializeBasicStatus(int HP, int MP, int MaxHP, int MaxMP, int STR, int DEX, int INT, int LUK)
{
	m_pUserStatus->SetBaseBasicStatus(HP, MP, MaxHP, MaxMP, STR, DEX, INT, LUK);
	m_pUserStatus->SetCalculatedBasicStatus(HP, MP, MaxHP, MaxMP, STR, DEX, INT, LUK);
}

void CUserCharacter::InitializeAdvancedStatus(int RegenerateHP, int RegenerateMP, int AttackPower, int MagicPower, int Defence, float MoveSpeed, float AttackSpeed, int CriticalRate, int CriticalDamage)
{
	m_pUserStatus->SetBaseAdvancedStatus(RegenerateHP, RegenerateMP, AttackPower, MagicPower, Defence, MoveSpeed, AttackSpeed, CriticalRate, CriticalDamage);
	m_pUserStatus->SetCalculatedAdvancedStatus(RegenerateHP, RegenerateMP, AttackPower, MagicPower, Defence, MoveSpeed, AttackSpeed, CriticalRate, CriticalDamage);
}

void CUserCharacter::AddBasicStatusValue(int HP, int MP, int MaxHP, int MaxMP, int STR, int DEX, int INT, int LUK)
{
	m_pUserStatus->AddCalculatedBasicStatus(HP, MP, MaxHP, MaxMP, STR, DEX, INT, LUK);
}

void CUserCharacter::AddAdvancedStatusValue(int RegenerateHP, int RegenerateMP, int AttackPower, int MagicPower, int Defence, float MoveSpeed, float AttackSpeed, int CriticalRate, int CriticalDamage)
{
	m_pUserStatus->AddCalculatedAdvancedStatus(RegenerateHP, RegenerateMP, AttackPower, MagicPower, Defence, MoveSpeed, AttackSpeed, CriticalRate, CriticalDamage);
}

std::shared_ptr<CUserStatus> CUserCharacter::GetUserStatus()
{
	return this->m_pUserStatus;
}

void CUserCharacter::UpdateStatus()
{
	std::vector<std::shared_ptr<CNSW_Item>> vEquipments;

	for (int i = 0; i < EQUIPMENT_PARTS_NUM; i++) {
		std::shared_ptr<CNSW_Item> pItem = m_pEquipment->GetItem(i);
		if (pItem != nullptr)
			vEquipments.push_back(pItem);
	}

	m_pUserStatus->CalculateStatus(vEquipments);
}




void CUserCharacter::RegistStatus(IObserver* observer)
{
	m_pUserStatus->RegistObserver(observer);
}

void CUserCharacter::RegistInventory(IObserver* observer)
{
	m_pInventory->RegistObserver(observer);
}

void CUserCharacter::RegistEquipment(IObserver* observer)
{
	m_pEquipment->RegistObserver(observer);
}



bool CUserCharacter::IsValidInventoryIndex(int location_idx, int slot_idx)
{
	return (
		(location_idx >= ID_INVENTORY && location_idx < INVENTORY_WINDOW_NUM)
		&& (slot_idx >= 0 && slot_idx < INVENTORY_COL_NUM * INVENTORY_ROW_NUM)
	);
}

bool CUserCharacter::IsValidEquipmentIndex(int index)
{
	return (index >= 0 && index < EQUIPMENT_PARTS_NUM);
}

bool CUserCharacter::IsValidEquipmentSlot(ItemType type, int slot)
{
	if (type == ItemType::Head) return slot == 0;
	else if (type == ItemType::Chest) return slot == 1;
	else if (type == ItemType::Pants) return slot == 2;
	else if (type == ItemType::Gloves) return slot == 3;
	else if (type == ItemType::Boots) return slot == 4;
	else if (type >= ItemType::Weapon && type < ItemType::Consume) return slot == 5;
	else return false;
}