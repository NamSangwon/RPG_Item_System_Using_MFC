// Item.cpp: 구현 파일

#include "pch.h"
#include "NSW_Item.h"

void CNSW_Item::SetItemIdentifier(std::pair<int, int> pair)
{
	this->m_iItemIdentifier = pair;
}

std::pair<int, int> CNSW_Item::GetItemIdentifier()
{
	return this->m_iItemIdentifier;
}

void CNSW_Item::SetFilePath(std::string file_path)
{
	this->m_strFilePath = file_path;
}

std::string CNSW_Item::GetFilePath()
{
	return this->m_strFilePath;
}

void CNSW_Item::SetItemName(std::string name)
{
	this->m_strName = name;
}

std::string CNSW_Item::GetItemName()
{
	return this->m_strName;
}

void CNSW_Item::SetItemType(ItemType type)
{
	if (type >= ItemType::Equipment && type <= ItemType::Others)
		this->m_nItemType = type;

	else
		this->m_nItemType = ItemType::Err;
}

ItemType CNSW_Item::GetItemType()
{
	return this->m_nItemType;
}

void CNSW_Item::SetWeight(int weight) {
	this->m_nWeight = weight;
}

int CNSW_Item::GetWeight() {
	return this->m_nWeight;
}

void CNSW_Item::SetItemRank(int rank)
{
	m_nItemRank = rank;
}

int CNSW_Item::GetItemRank()
{
	return m_nItemRank;
}

std::string CNSW_Item::ShowInfo()
{
	std::string result;
	result += ("[ Name ]\n");
	result += m_strName;
	result += "\n\n[ Rank ]\n";
	result += GetItemRankText(this->m_nItemRank);
	result += "\n\n[ Type ]\n";
	result += GetItemTypeText(m_nItemType);
	result += "\n\n[ Weight ]\n";
	result += std::to_string(m_nWeight);

	return result;
}

bool CNSW_Item::IsEquipment()
{
	return this->m_nItemType >= ItemType::Equipment && this->m_nItemType < ItemType::Consume;
}

bool CNSW_Item::IsArmor()
{
	return this->m_nItemType >= ItemType::Armor && this->m_nItemType < ItemType::Weapon;
}

bool CNSW_Item::IsWeapon()
{
	return this->m_nItemType >= ItemType::Weapon && this->m_nItemType < ItemType::Consume;
}

bool CNSW_Item::IsConsume()
{
	return this->m_nItemType >= ItemType::Consume && this->m_nItemType < ItemType::Others;
}

bool CNSW_Item::IsPotion()
{
	return this->m_nItemType >= ItemType::Potion && this->m_nItemType < ItemType::Others;
}