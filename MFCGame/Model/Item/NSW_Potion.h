#pragma once
#include "NSW_Consume.h"

class CNSW_Potion : public CNSW_Consume
{
public:
	//CNSW_Potion() :
	//	CNSW_Consume(),
	//	m_nRecoverAmount(U_NONE) {}

	CNSW_Potion(const std::shared_ptr<CNSW_Potion>& potion) :
		CNSW_Consume(potion->m_iItemIdentifier, potion->m_strFilePath, potion->m_strName, potion->m_nItemType, potion->m_nWeight, potion->m_nItemRank, potion->m_nAmount, potion->m_nMaxCapacity),
		m_nRecoverAmount(potion->m_nRecoverAmount) {}

	CNSW_Potion(std::pair<int, int> id,  std::string& file_path, std::string& name, ItemType type, int weight, int rank, int amount, int max, int recover_amount) :
		CNSW_Consume(id, file_path, name, type, weight, rank, amount, max),
		m_nRecoverAmount(recover_amount) {}

	virtual ~CNSW_Potion() = default;

	void SetRecoverAmount(int recover_amount);
	int GetRecoverAmount();

	virtual std::string ShowInfo();

private:
	int m_nRecoverAmount;
};

