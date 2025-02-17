#include "pch.h"
#include "Inventory.h"

#include "../Model/Item/NSW_Potion.h"

std::shared_ptr<CNSW_Item> CInventory::AddItem(std::shared_ptr<CNSW_Item> item)
{
	if (item != nullptr) {
		int index = U_NONE;

		// �κ��丮�� �߰��� �Һ� �����۰� ���� �Һ� �������� �ִ� ���
		if (item->IsConsume()) {
			index = FindEqualItemSlot(item);
			if (IsValidSlotIndex(index)) {
				std::shared_ptr<CNSW_Consume> pConsume = std::dynamic_pointer_cast<CNSW_Consume>(item);
				std::shared_ptr<CNSW_Consume> pOwnConsume = std::dynamic_pointer_cast<CNSW_Consume>(m_vItems[index]);

				// �κ��丮 ���� �Һ� �������� ���� ���ϱ�
				int remain = U_NONE;
				do {
					remain = pOwnConsume->AddAmountAndGetRemain(pConsume->GetAmount());
					pConsume->SetAmount(remain);
					index = FindEqualItemSlot(item);
				} while (remain > 0 && IsValidSlotIndex(index));

				// �Է� ���� �������� ��� �߰��Ͽ��� ���, [����]
				if (remain <= 0) {
					item.reset();
					return nullptr;
				}
			}
		}

		// �κ��丮�� �� ������ �ִ� �� ���� Ȯ�� ��, �������� ���Կ� �߰� [����]
		index = FindEmptySlot();
		if (IsValidSlotIndex(index)) {
			m_vItems[index] = std::move(item);

			// �κ��丮 â�� �̹��� ������Ʈ�� ���� �˸� ����
			int location_idx = index / (INVENTORY_ROW_NUM * INVENTORY_COL_NUM);
			int slot_idx = index % (INVENTORY_ROW_NUM * INVENTORY_COL_NUM);

			NotifyUpdateItem(location_idx, slot_idx);
		}
	}

	// �������� �κ��丮�� ���� ���� ���ϴ� ���, [����]
	// return false;
	return item;
}

void CInventory::InsertItem(std::shared_ptr<CNSW_Item> item, int index)
{
	if (IsValidSlotIndex(index) && !IsExist(index)) {
		m_vItems[index] = std::move(item);

		// �κ��丮 â�� �̹��� ������Ʈ�� ���� �˸� ����
		int location_idx = index / (INVENTORY_ROW_NUM * INVENTORY_COL_NUM);
		int slot_idx = index % (INVENTORY_ROW_NUM * INVENTORY_COL_NUM);

		NotifyUpdateItem(location_idx, slot_idx);
	}
}

void CInventory::DeleteItem(int index)
{
	if (IsValidSlotIndex(index) && IsExist(index)) {
		m_vItems[index].reset();

		// �κ��丮 â�� �̹��� ������Ʈ�� ���� �˸� ����
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
	return (index >= 0 && index < INVENTORY_COL_NUM * INVENTORY_ROW_NUM * INVENTORY_WINDOW_NUM); // 0 ~ 119 (120�� = 5 * 24��)
}

bool CInventory::IsExist(int index)
{
	if (IsValidSlotIndex(index))
		return (m_vItems[index] != nullptr);

	return false;
}