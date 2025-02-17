#pragma once
#include "../Model/Item/NSW_Item.h"

class IObserver
{
public:
	virtual void UpdateItem(int location_idx, int slot_idx) = 0;
	virtual void UpdateBasicStatus(int HP, int MP, int MaxHP, int MaxMP, int STR, int DEX, int INT, int LUK) = 0;
	virtual void UpdateAdvancedStatus(int RegenerateHP, int RegenerateMP, int AttackPower, int MagicPower, int Defence, float MoveSpeed, float AttackSpeed, int CriticalRate, int CriticalDamage) = 0;
};