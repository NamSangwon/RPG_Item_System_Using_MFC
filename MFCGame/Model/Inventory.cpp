#include "pch.h"
#include "Inventory.h"

#include "../Model/Item/NSW_Potion.h"

std::shared_ptr<CNSW_Item> CInventory::AddItem(std::shared_ptr<CNSW_Item> item)
{
	if (item != nullptr) {
		int index = U_NONE;

		// 인벤토리에 추가할 소비 아이템과 같은 소비 아이템이 있는 경우
		if (item->IsConsume()) {
			index = FindEqualItemSlot(item);
			if (IsValidSlotIndex(index)) {
				std::shared_ptr<CNSW_Consume> pConsume = std::dynamic_pointer_cast<CNSW_Consume>(item);
				std::shared_ptr<CNSW_Consume> pOwnConsume = std::dynamic_pointer_cast<CNSW_Consume>(m_vItems[index]);

				// 인벤토리 내의 소비 아이템의 개수 더하기
				int remain = U_NONE;
				do {
					remain = pOwnConsume->AddAmountAndGetRemain(pConsume->GetAmount());
					pConsume->SetAmount(remain);
					index = FindEqualItemSlot(item);
				} while (remain > 0 && IsValidSlotIndex(index));

				// 입력 받은 아이템을 모두 추가하였을 경우, [성공]
				if (remain <= 0) {
					item.reset();
					return nullptr;
				}
			}
		}

		// 인벤토리에 빈 슬롯이 있는 지 여부 확인 후, 아이템을 슬롯에 추가 [성공]
		index = FindEmptySlot();
		if (IsValidSlotIndex(index)) {
			m_vItems[index] = std::move(item);

			// 인벤토리 창의 이미지 업데이트를 위해 알림 전송
			int location_idx = index / (INVENTORY_ROW_NUM * INVENTORY_COL_NUM);
			int slot_idx = index % (INVENTORY_ROW_NUM * INVENTORY_COL_NUM);

			NotifyUpdateItem(location_idx, slot_idx);
		}
	}

	// 아이템을 인벤토리에 전부 넣지 못하는 경우, [실패]
	// return false;
	return item;
}

void CInventory::InsertItem(std::shared_ptr<CNSW_Item> item, int index)
{
	if (IsValidSlotIndex(index) && !IsExist(index)) {
		m_vItems[index] = std::move(item);

		// 인벤토리 창의 이미지 업데이트를 위해 알림 전송
		int location_idx = index / (INVENTORY_ROW_NUM * INVENTORY_COL_NUM);
		int slot_idx = index % (INVENTORY_ROW_NUM * INVENTORY_COL_NUM);

		NotifyUpdateItem(location_idx, slot_idx);
	}
}

void CInventory::DeleteItem(int index)
{
	if (IsValidSlotIndex(index) && IsExist(index)) {
		m_vItems[index].reset();

		// 인벤토리 창의 이미지 업데이트를 위해 알림 전송
		int location_idx = index / (INVENTORY_ROW_NUM * INVENTORY_COL_NUM);
		int slot_idx = index % (INVENTORY_ROW_NUM * INVENTORY_COL_NUM);

		NotifyUpdateItem(location_idx, slot_idx);
	}
}

std::shared_ptr<CNSW_Item> CInventory::GetItem(int index)
{
	return m_vItems[index];
}

int CInventory::FindEmptySlot()
{
	for (int i = 0; i < INVENTORY_COL_NUM * INVENTORY_ROW_NUM; i++)
		if (m_vItems[i] == nullptr) {
			return i;
		}

	return U_NONE;
}

int CInventory::FindEqualItemSlot(std::shared_ptr<CNSW_Item> item)
{
	for (int i = 0; i < INVENTORY_COL_NUM * INVENTORY_ROW_NUM; i++) {
		if (m_vItems[i] != nullptr && m_vItems[i]->GetItemIdentifier() == item->GetItemIdentifier()) {
			return i;
		}
	}

	return U_NONE;
}

bool CInventory::IsValidSlotIndex(int index)
{
	return (index >= 0 && index < INVENTORY_COL_NUM * INVENTORY_ROW_NUM * INVENTORY_WINDOW_NUM); // 0 ~ 119 (120개 = 5 * 24개)
}

bool CInventory::IsExist(int index)
{
	if (IsValidSlotIndex(index))
		return (m_vItems[index] != nullptr);

	return false;
}