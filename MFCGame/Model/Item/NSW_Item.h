#pragma once
class CNSW_Item {
public:
	//CNSW_Item() :m_iItemIdentifier(std::make_pair(U_NONE, U_NONE)), m_strFilePath(ERROR_IMAGE),
	//	m_strName("Item"), m_nItemType(ItemType::Err), m_nWeight(U_NONE), m_nItemRank(U_NONE) {}

	CNSW_Item(const std::shared_ptr<CNSW_Item>& item) :m_iItemIdentifier(item->m_iItemIdentifier), m_strFilePath(item->m_strFilePath),
		m_strName(item->m_strName), m_nItemType(item->m_nItemType), m_nWeight(item->m_nWeight), m_nItemRank(item->m_nItemRank) {}

	CNSW_Item(std::pair<int,int> identifier, std::string& file_path, std::string& name, ItemType type, int weight, int rank) :
		m_iItemIdentifier(identifier), m_strFilePath(file_path),
		m_strName(name), m_nItemType(type), m_nWeight(weight), m_nItemRank(rank) {}

	virtual ~CNSW_Item() = default;


	void SetItemIdentifier(std::pair<int, int> pair);
	std::pair<int, int> GetItemIdentifier();

	void SetFilePath(std::string file_path);
	std::string GetFilePath();

	void SetItemName(std::string name);
	std::string GetItemName();

	void SetItemType(ItemType type);
	ItemType GetItemType();

	void SetWeight(int weight);
	int GetWeight();

	void SetItemRank(int rank);
	int GetItemRank();

	virtual std::string ShowInfo();

	bool IsEquipment();
	bool IsArmor();
	bool IsWeapon();
	bool IsConsume();
	bool IsPotion();

protected:
	std::pair<int, int> m_iItemIdentifier; // Using for ID of <GenerateIndex, ItemIndex>
	std::string& m_strFilePath;
	std::string& m_strName;
	ItemType m_nItemType;
	int m_nWeight;
	int m_nItemRank;
};
