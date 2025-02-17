#pragma once
#include "../Interface/Subject.h"

#include "../Model/Item/NSW_Item.h"

class CEquipment : public ISubject
{
public:
	CEquipment()
	{
		m_vItems.resize(EQUIPMENT_PARTS_NUM, nullptr);
	}
	~CEquipment() = default;

	std::shared_ptr<CNSW_Item> GetItem(int iSlot);
	bool IsExist(int index);

private:
	friend class CUserCharacter;

	void InsertItem(std::shared_ptr<CNSW_Item> item, int index);
	void DeleteItem(int index);

	std::vector<std::shared_ptr<CNSW_Item>> m_vItems;
};