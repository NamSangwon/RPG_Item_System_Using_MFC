#pragma once
#include "../Interface/Subject.h"

#include "../Model/Item/NSW_Item.h"

class CInventory : public ISubject
{
public:
	CInventory() 
	{
		m_vItems.resize(INVENTORY_COL_NUM * INVENTORY_ROW_NUM * INVENTORY_WINDOW_NUM, nullptr); // 24 * 인벤토리 창 개수
	}
	~CInventory() = default;

	std::shared_ptr<CNSW_Item> GetItem(int index);
	bool IsExist(int index);

private:
	friend class CUserCharacter;

	std::shared_ptr<CNSW_Item> AddItem(std::shared_ptr<CNSW_Item> item);
	void InsertItem(std::shared_ptr<CNSW_Item> item, int index);
	void DeleteItem(int index);

	int FindEmptySlot();
	int FindEqualItemSlot(std::shared_ptr<CNSW_Item> item);
	bool IsValidSlotIndex(int index);

	std::vector<std::shared_ptr<CNSW_Item>> m_vItems;
};