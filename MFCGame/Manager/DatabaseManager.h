#pragma once
#include "../Interface/SingletonTemplate.h"

#include "../Model/Item/NSW_Item.h"
#include "../Model/Ability/AbilityData.h"
#include "../Character/UserCharacter.h"

class CDatabaseManager : public SingletonTemplate<CDatabaseManager>
{
public:
	friend class SingletonTemplate<CDatabaseManager>;

	bool DatabaseConnection();
	bool DatabaseDisconnetion();

	bool Login(std::string user_id, std::string user_pw, std::vector<std::string>& characters); // Login (Get Characters of User)
	bool Register(std::string user_id, std::string user_pw, std::string user_name, int user_gender); // Register
	bool DeleteUser(std::string user_id, std::string user_pw); // Delete

	bool CreateCharacter(std::string user_id, std::string character_name, int character_class, int character_gender);
	bool DeleteCharacter(std::string character_name);

	bool SetCharacterInfo(std::string character_name, int character_class, int HP, int MP, int STR, int DEX, int INT, int LUK); // Store
	bool GetCharacterInfo(std::string character_name, int& character_class, int& gender, int& HP, int& MP, int& STR, int& DEX, int& INT, int& LUK); // Load Data by name After Login

	bool SetCharacterItems(std::string character_name, std::map<std::pair<int, int>, std::shared_ptr<CNSW_Item>>& item_map); // Store
	bool SetCharacterItem(std::string character_name, std::string item_id, int location, int slot, int file_idx, int item_idx, int defence, int amount, int durability, int attack_power, int magic_power);
	bool GetCharacterItem(std::string character_name, std::map<std::pair<int, int>, std::shared_ptr<CNSW_Item>>& item_map); // Load Data by name After Login

	bool SetAbilityData(std::string item_id, int ability_id, int ability_rate, int ability_value); // Store
	bool GetAbilityData(std::string item_id, std::shared_ptr<CAbilityData>& pAdditionalData, std::shared_ptr<CAbilityData>& pSpecialData);

	bool InitializeCharacterItem(std::string character_name); // Initialize Before Execusing Store

	bool LoadCharacterData(std::string character_name, std::shared_ptr<CUserCharacter>& user_character);
	bool StoreCharacterData(std::shared_ptr<CUserCharacter>& user_character);

private:
	//DB 연결은 DBCP (Database Connection Pool)을 통해서 관리하여야 하지만 
	//임의의로 생략하고 해당 프로그램이 시작되고 끝날 때, DB를 연결하고 해제하도록 함.
	CDatabaseManager()
	{
		mySQL = nullptr;
		DatabaseConnection();
	}
	~CDatabaseManager()
	{
		if (!mySQL) DatabaseDisconnetion();
	}

	MYSQL* mySQL;
};

