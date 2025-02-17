#include "pch.h"
#include "Equipment.h"

void CEquipment::InsertItem(std::shared_ptr<CNSW_Item> item, int index)
{
	if (!IsExist(index)) {
		m_vItems[index] = std::move(item);

		// ��� â�� �̹��� ������Ʈ�� ���� �˸� ����
		NotifyUpdateItem(ID_EQUIPMENT, index);
	}
}

std::shared_ptr<CNSW_Item> CEquipment::GetItem(int iSlot)
{
	return m_vItems[iSlot];
}

void CEquipment::DeleteItem(int index)
{
	if (IsExist(index)) {
		m_vItems[index].reset();

		// ��� â�� �̹��� ������Ʈ�� ���� �˸� ����
		NotifyUpdateItem(ID_EQUIPMENT, index);
	}
}

bool CEquipment::IsExist(int index)
{
	return (m_vItems[index] != nullptr);
}
