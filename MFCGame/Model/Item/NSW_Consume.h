#pragma once
#include "NSW_Item.h"

class CNSW_Consume : public CNSW_Item
{
public:
	//CNSW_Consume() :
	//	CNSW_Item(),
	//	m_nAmount(U_NONE),
	//	m_nMaxCapacity(U_NONE) {}

	CNSW_Consume(const std::shared_ptr<CNSW_Consume>& consume) :
		CNSW_Item(consume->m_iItemIdentifier, consume->m_strFilePath, consume->m_strName, consume->m_nItemType, consume->m_nWeight, consume->m_nItemRank),
		m_nMaxCapacity(consume->m_nMaxCapacity)
	{
		SetAmount(consume->m_nAmount);
	}

	CNSW_Consume(std::pair<int, int> id,  std::string& file_path, std::string& name, ItemType type, int weight, int rank, int amount, int max) :
		CNSW_Item(id, file_path, name, type, weight, rank),
		m_nMaxCapacity(max)
	{
		SetAmount(amount);
	}

	virtual ~CNSW_Consume() = default;

	void SetAmount(int cnt);
	int AddAmountAndGetRemain(int amount);
	bool IsWithinInAmount(int amount);
	int GetAmount() const;

	void SetMaxCapacity(int max);
	int GetMaxCapacity();

	virtual std::string ShowInfo();

protected:
	int m_nMaxCapacity;
	int m_nAmount;
};

