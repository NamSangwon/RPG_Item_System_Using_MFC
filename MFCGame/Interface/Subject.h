#pragma once
#include "Observer.h"

class ISubject
{
public:
	void RegistObserver(IObserver* o) {
		m_vObservers.push_back(o);
	}

	void RemoveObserver(IObserver* o) {
		m_vObservers.erase(std::remove(m_vObservers.begin(), m_vObservers.end(), o), m_vObservers.end());
	}

	void NotifyUpdateItem(int location_idx, int slot_idx) {
		for (auto& o : m_vObservers) {
			o->UpdateItem(location_idx, slot_idx);
		}
	}

	void NotifyUpdateBasicStatus(int HP, int MP, int MaxHP, int MaxMP, int STR, int DEX, int INT, int LUK) {
		for (auto& o : m_vObservers) {
			o->UpdateBasicStatus(HP, MP, MaxHP, MaxMP, STR, DEX, INT, LUK);
		}
	}	
	
	void NotifyUpdateAdvancedStatus(int RegenerateHP, int RegenerateMP, int AttackPower, int MagicPower, int Defence, float MoveSpeed, float AttackSpeed, int CriticalRate, int CriticalDamage) {
		for (auto& o : m_vObservers) {
			o->UpdateAdvancedStatus(RegenerateHP, RegenerateMP, AttackPower, MagicPower, Defence, MoveSpeed, AttackSpeed, CriticalRate, CriticalDamage);
		}
	}

protected:
	std::vector<IObserver*> m_vObservers;
};

