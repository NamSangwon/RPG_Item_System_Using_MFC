#include "pch.h"
#include "DatabaseManager.h"

#include "ItemGenerator.h"

bool CDatabaseManager::DatabaseConnection()
{
	// MYSQL 라이브러리 초기화
	if (mysql_library_init(0, NULL, NULL)) {
		fprintf(stderr, "could not initialize MySQL client library\n");	
		DatabaseDisconnetion();
		exit(1);
	}

	// MYSQL 연결 초기화
	mySQL = mysql_init(mySQL);
	if (!mySQL) {
		OutputDebugString(_T("Init failed, out of memory?\n"));
		DatabaseDisconnetion();
		exit(1);
	}

	// MYSQL 연결 시도
	if (!mysql_real_connect(mySQL, DB_ADDRESS, DB_ID, DB_PW, DB_NAME, DB_PORT, NULL, CLIENT_FOUND_ROWS)) 
	{
		OutputDebugString(_T("Connect failed\n"));
		DatabaseDisconnetion();
		exit(1);
	}

	return EXIT_SUCCESS;
}

bool CDatabaseManager::DatabaseDisconnetion()
{
	mysql_close(mySQL);

	mysql_library_end();

	return EXIT_SUCCESS;
}

bool CDatabaseManager::Login(std::string user_id, std::string user_pw, std::vector<std::string>& characters)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND bind[2];

	std::vector<char*> user_info_type_str = {
		const_cast<char*>(user_id.c_str()),
		const_cast<char*>(user_pw.c_str()),
	};

	stmt = mysql_stmt_init(mySQL);

	if (mysql_stmt_prepare(stmt, SP_LOGIN, strlen(SP_LOGIN))) {
		OutputDebugString(_T("mysql_stmt_prepare(), SP_LOGIN FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_param_count(stmt) != 2) {
		OutputDebugString(_T("mysql_stmt_param_count(), NOT EQUAL PARAM COUNT\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	memset(bind, 0, sizeof(bind));

	for (int i = 0; i < user_info_type_str.size(); i++) {
		bind[i].buffer_type = MYSQL_TYPE_STRING;
		bind[i].buffer = user_info_type_str[i];
		bind[i].buffer_length = strlen(user_info_type_str[i]);
	}

	if (mysql_stmt_bind_param(stmt, bind)) {
		OutputDebugString(_T("mysql_stmt_bind_param(), PARAM BINDING FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_execute(stmt)) {
		OutputDebugString(_T("[Login] mysql_stmt_execute(), Statement Execute FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	// Fetch result metadata
	MYSQL_RES* result = mysql_stmt_result_metadata(stmt);
	if (!result) {
		OutputDebugString(_T("mysql_stmt_result_metadata() FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	int col_cnt = mysql_num_fields(result);
	if (col_cnt != 1) {
		OutputDebugString(_T("invalid column count returned by MySQL\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	MYSQL_BIND output_bind[1];
	memset(output_bind, 0, sizeof(output_bind));
	char user_character[STRING_SIZE];

	// Bind output parameters
	output_bind[0].buffer_type = MYSQL_TYPE_STRING;
	output_bind[0].buffer = user_character;
	output_bind[0].buffer_length = STRING_SIZE;

	if (mysql_stmt_bind_result(stmt, output_bind)) {
		OutputDebugString(_T("mysql_stmt_bind_result() failed\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_store_result(stmt)) {
		OutputDebugString(_T("mysql_stmt_store_result() failed\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	bool isExist = false;
	while (!mysql_stmt_fetch(stmt)) {
		characters.push_back(std::string(user_character));
		isExist = true;
	}
	
	mysql_free_result(result);
	mysql_stmt_free_result(stmt);
	mysql_stmt_close(stmt);

	if (isExist)
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;
}

bool CDatabaseManager::Register(std::string user_id, std::string user_pw, std::string user_name, int user_gender)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND bind[4];

	std::vector<char*> user_info_type_str = {
		const_cast<char*>(user_id.c_str()),
		const_cast<char*>(user_pw.c_str()),
		const_cast<char*>(user_name.c_str())
	};

	stmt = mysql_stmt_init(mySQL);

	if (mysql_stmt_prepare(stmt, SP_REGISTER, strlen(SP_REGISTER))) {
		OutputDebugString(_T("mysql_stmt_prepare(), SP_REGISTER FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_param_count(stmt) != 4) {
		OutputDebugString(_T("mysql_stmt_param_count(), NOT EQUAL PARAM COUNT\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	memset(bind, 0, sizeof(bind));

	for (int i = 0; i < user_info_type_str.size(); i++) {
		bind[i].buffer_type = MYSQL_TYPE_STRING;
		bind[i].buffer = user_info_type_str[i];
		bind[i].buffer_length = strlen(user_info_type_str[i]);
	}

	int idx = user_info_type_str.size();
	bind[idx].buffer_type = MYSQL_TYPE_TINY;
	bind[idx].buffer = &user_gender;
	bind[idx].buffer_length = sizeof(user_gender);

	if (mysql_stmt_bind_param(stmt, bind)) {
		OutputDebugString(_T("mysql_stmt_bind_param(), PARAM BINDING FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_execute(stmt)) {
		OutputDebugString(_T("[Register] mysql_stmt_execute(), Statement Execute FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	mysql_stmt_close(stmt);

	return EXIT_SUCCESS;
}

bool CDatabaseManager::DeleteUser(std::string user_id, std::string user_pw)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND bind[2];
	stmt = mysql_stmt_init(mySQL);

	char* input_id = const_cast<char*>(user_id.c_str());
	char* input_pw = const_cast<char*>(user_pw.c_str());

	if (mysql_stmt_prepare(stmt, SP_DELETE_USER, strlen(SP_DELETE_USER))) {
		OutputDebugString(_T("mysql_stmt_prepare(), SP_DELETE FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_param_count(stmt) != 2) {
		OutputDebugString(_T("mysql_stmt_param_count(), NOT EQUAL PARAM COUNT\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer = input_id;
	bind[0].buffer_length = strlen(input_id);

	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer = input_pw;
	bind[1].buffer_length = strlen(input_pw);

	if (mysql_stmt_bind_param(stmt, bind)) {
		OutputDebugString(_T("mysql_stmt_bind_param(), PARAM BINDING FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_execute(stmt)) {
		OutputDebugString(_T("[DeleteUser] mysql_stmt_execute(), Statement Execute FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	mysql_stmt_close(stmt);

	return EXIT_SUCCESS;
}

bool CDatabaseManager::CreateCharacter(std::string user_id, std::string character_name, int character_class, int character_gender)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND bind[4];

	std::vector<char*> user_data_type_str = {
		const_cast<char*>(user_id.c_str()),
		const_cast<char*>(character_name.c_str())
	};

	stmt = mysql_stmt_init(mySQL);

	if (mysql_stmt_prepare(stmt, SP_CREATE_CHARACTER, strlen(SP_CREATE_CHARACTER))) {
		OutputDebugString(_T("mysql_stmt_prepare(), SP_CREATE_CHARACTER FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_param_count(stmt) != 4) {
		OutputDebugString(_T("mysql_stmt_param_count(), NOT EQUAL PARAM COUNT\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	memset(bind, 0, sizeof(bind));

	for (int i = 0; i < user_data_type_str.size(); i++) {
		bind[i].buffer_type = MYSQL_TYPE_STRING;
		bind[i].buffer = user_data_type_str[i];
		bind[i].buffer_length = strlen(user_data_type_str[i]);
	}

	int idx = user_data_type_str.size();
	bind[idx].buffer_type = MYSQL_TYPE_TINY;
	bind[idx].buffer = &character_gender;
	bind[idx].buffer_length = sizeof(character_gender);

	idx++;
	bind[idx].buffer_type = MYSQL_TYPE_TINY;
	bind[idx].buffer = &character_class;
	bind[idx].buffer_length = sizeof(character_class);

	if (mysql_stmt_bind_param(stmt, bind)) {
		OutputDebugString(_T("mysql_stmt_bind_param(), PARAM BINDING FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_execute(stmt)) {
		OutputDebugString(_T("[CreateCharacter] mysql_stmt_execute(), Statement Execute FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	mysql_stmt_close(stmt);

	return EXIT_SUCCESS;
}

bool CDatabaseManager::DeleteCharacter(std::string character_name)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND bind[1];
	stmt = mysql_stmt_init(mySQL);

	char* input_id = const_cast<char*>(character_name.c_str());

	if (mysql_stmt_prepare(stmt, SP_DELETE_CHARACTER, strlen(SP_DELETE_CHARACTER))) {
		OutputDebugString(_T("mysql_stmt_prepare(), SP_DELETE_CHARACTER FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_param_count(stmt) != 1) {
		OutputDebugString(_T("mysql_stmt_param_count(), NOT EQUAL PARAM COUNT\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer = input_id;
	bind[0].buffer_length = strlen(input_id);

	if (mysql_stmt_bind_param(stmt, bind)) {
		OutputDebugString(_T("mysql_stmt_bind_param(), PARAM BINDING FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_execute(stmt)) {
		OutputDebugString(_T("[DeleteCharacter] mysql_stmt_execute(), Statement Execute FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	mysql_stmt_close(stmt);

	return EXIT_SUCCESS;
}

bool CDatabaseManager::SetCharacterInfo(std::string character_name, int character_class, int HP, int MP, int STR, int DEX, int INT, int LUK)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND bind[8];

	char* input_id = const_cast<char*>(character_name.c_str());
	std::vector<int> user_status_type_int = { HP, MP, STR, DEX, INT, LUK, character_class };

	stmt = mysql_stmt_init(mySQL);

	if (mysql_stmt_prepare(stmt, SP_SET_CHARACTER_INFO, strlen(SP_SET_CHARACTER_INFO))) {
		OutputDebugString(_T("mysql_stmt_prepare(), SP_SET_CHARACTER_INFO FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_param_count(stmt) != 8) {
		OutputDebugString(_T("mysql_stmt_param_count(), NOT EQUAL PARAM COUNT\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	memset(bind, 0, sizeof(bind));

	int idx = 0;
	bind[idx].buffer_type = MYSQL_TYPE_STRING;
	bind[idx].buffer = input_id;
	bind[idx].buffer_length = strlen(input_id);

	idx++;
	for (int i = 0; i < user_status_type_int.size(); i++) {
		bind[idx + i].buffer_type = MYSQL_TYPE_LONG;
		bind[idx + i].buffer = &user_status_type_int[i];
		bind[idx + i].buffer_length = sizeof(int);
	}

	if (mysql_stmt_bind_param(stmt, bind)) {
		OutputDebugString(_T("mysql_stmt_bind_param(), PARAM BINDING FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_execute(stmt)) {
		OutputDebugString(_T("[SetCharacterInfo] mysql_stmt_execute(), Statement Execute FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	mysql_stmt_close(stmt);

	return EXIT_SUCCESS;
}

bool CDatabaseManager::GetCharacterInfo(std::string character_name, int& character_class, int& gender, int& HP, int& MP, int& STR, int& DEX, int& INT, int& LUK)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND input_bind[1];

	char* input_name = const_cast<char*>(character_name.c_str());

	stmt = mysql_stmt_init(mySQL);

	// Prepare the statement with SP_GET_USER_INFO
	if (mysql_stmt_prepare(stmt, SP_GET_CHARACTER_INFO, strlen(SP_GET_CHARACTER_INFO))) {
		OutputDebugString(_T("mysql_stmt_prepare(), SP_GET_CHARACTER_INFO FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_param_count(stmt) != 1) {
		OutputDebugString(_T("mysql_stmt_param_count(), NOT EQUAL PARAM COUNT\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	memset(input_bind, 0, sizeof(input_bind));

	// Bind parameters
	input_bind[0].buffer_type = MYSQL_TYPE_STRING;
	input_bind[0].buffer = input_name;
	input_bind[0].buffer_length = strlen(input_name);

	if (mysql_stmt_bind_param(stmt, input_bind)) {
		OutputDebugString(_T("mysql_stmt_bind_param(), PARAM BINDING FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_execute(stmt)) {
		OutputDebugString(_T("[GetCharacterInfo] mysql_stmt_execute(), Statement Execute FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	// Fetch result metadata
	MYSQL_RES* result = mysql_stmt_result_metadata(stmt);
	if (!result) {
		OutputDebugString(_T("mysql_stmt_result_metadata() FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	int col_cnt = mysql_num_fields(result);
	if (col_cnt != 8) {
		OutputDebugString(_T("invalid column count returned by MySQL\n"));
		return EXIT_FAILURE;
	}

	MYSQL_BIND output_bind[8];
	memset(output_bind, 0, sizeof(output_bind));

	std::vector<int> user_status_type_int(8, U_NONE);
	// signed char user_gender = U_NONE;

	// Bind output parameters
	for (int i = 0; i < user_status_type_int.size(); i++) {
		output_bind[i].buffer_type = MYSQL_TYPE_LONG;
		output_bind[i].buffer = &user_status_type_int[i];
		output_bind[i].buffer_length = sizeof(int);
	}

	if (mysql_stmt_bind_result(stmt, output_bind)) {
		OutputDebugString(_T("mysql_stmt_bind_result() failed\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_store_result(stmt)) {
		OutputDebugString(_T("mysql_stmt_store_result() failed\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	enum class STATUS_TYPE_INT { HP, MP, STR, DEX, INT, LUK, GENDER, CLASS };

	bool isExistedUser = false;
	while (!mysql_stmt_fetch(stmt)) {
		HP = user_status_type_int[static_cast<int>(STATUS_TYPE_INT::HP)];
		MP = user_status_type_int[static_cast<int>(STATUS_TYPE_INT::MP)];
		STR = user_status_type_int[static_cast<int>(STATUS_TYPE_INT::STR)];
		DEX = user_status_type_int[static_cast<int>(STATUS_TYPE_INT::DEX)];
		INT = user_status_type_int[static_cast<int>(STATUS_TYPE_INT::INT)];
		LUK = user_status_type_int[static_cast<int>(STATUS_TYPE_INT::LUK)];
		gender = user_status_type_int[static_cast<int>(STATUS_TYPE_INT::GENDER)];
		character_class = user_status_type_int[static_cast<int>(STATUS_TYPE_INT::CLASS)];

		isExistedUser = true;
	}

	mysql_free_result(result);
	mysql_stmt_free_result(stmt);
	mysql_stmt_close(stmt);

	if (isExistedUser) return EXIT_SUCCESS;
	else return EXIT_FAILURE;
}

bool CDatabaseManager::InitializeCharacterItem(std::string character_name)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND bind[1];
	stmt = mysql_stmt_init(mySQL);

	char* input_id = const_cast<char*>(character_name.c_str());

	// Prepare the statement with SP_INITIALIZE_CHARACTER_ITEM
	if (mysql_stmt_prepare(stmt, SP_INITIALIZE_CHARACTER_ITEM, strlen(SP_INITIALIZE_CHARACTER_ITEM))) {
		OutputDebugString(_T("mysql_stmt_prepare(), SP_INITIALIZE_CHARACTER_ITEM FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_param_count(stmt) != 1) {
		OutputDebugString(_T("mysql_stmt_param_count(), NOT EQUAL PARAM COUNT\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	memset(bind, 0, sizeof(bind));

	// Bind parameters
	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer = input_id;
	bind[0].buffer_length = strlen(input_id);

	if (mysql_stmt_bind_param(stmt, bind)) {
		OutputDebugString(_T("mysql_stmt_bind_param(), PARAM BINDING FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_execute(stmt)) {
		OutputDebugString(_T("[InitializeCharacterItem] mysql_stmt_execute(), Statement Execute FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	mysql_stmt_close(stmt);

	return EXIT_SUCCESS;
}



bool CDatabaseManager::SetCharacterItems(std::string character_name, std::map<std::pair<int, int>, std::shared_ptr<CNSW_Item>>& item_map)
{
	for (auto& cell : item_map) {
		int file_idx = cell.second->GetItemIdentifier().first;
		int item_idx = cell.second->GetItemIdentifier().second;

		// 아이템 ID = 소유자 명 + 아이템 공통 데이터 아이디 + Location(아이템이 위치한 창) + Index(아이템이 위치한 슬롯)
		std::string item_id = character_name + std::to_string(file_idx) + std::to_string(item_idx) + std::to_string(cell.first.first) + std::to_string(cell.first.second);

		if (cell.second->IsEquipment()) {
			std::shared_ptr<CNSW_Equipment> pEquipment = std::dynamic_pointer_cast<CNSW_Equipment>(cell.second);

			if (pEquipment != nullptr) {
				if (cell.second->IsArmor()) {
					std::shared_ptr<CNSW_Armor> pArmor = std::dynamic_pointer_cast<CNSW_Armor>(pEquipment);

					// 개별 방어구 아이템 데이터 데이터베이스에 저장
					if (pArmor != nullptr
						&& SetCharacterItem(
								character_name,
								item_id,
								cell.first.first, // ItemStoredLocation
								cell.first.second, // ItemStoredIndex
								file_idx, // Index of item_list_path in file
								item_idx, // index of item in item_list
								pArmor->GetDefence(),
								U_NONE,
								pArmor->GetCurrentDurability(),
								U_NONE,
								U_NONE
							) == EXIT_FAILURE
						)
						return EXIT_FAILURE;
				}
				else if (cell.second->IsWeapon()) {
					std::shared_ptr<CNSW_Weapon> pWeapon = std::dynamic_pointer_cast <CNSW_Weapon>(pEquipment);

					// 개별 무기 아이템 데이터 데이터베이스에 저장
					if (pWeapon != nullptr
						&& SetCharacterItem(
							character_name,
							item_id,
							cell.first.first, // ItemStoredLocation
							cell.first.second, // ItemStoredIndex
							file_idx, // Index of item_list_path in file
							item_idx, // index of item in item_list
							U_NONE,
							U_NONE,
							pWeapon->GetCurrentDurability(),
							pWeapon->GetAttackPower(),
							pWeapon->GetMagicPower()
						) == EXIT_FAILURE
						)
						return EXIT_FAILURE;
				}

				// 해당 아이템에 대한 Additional Ability 데이터 데이터베이스에 저장
				if (pEquipment->GetAdditionalData() != nullptr) {
					for (int i = 0; i < pEquipment->GetAdditionalData()->GetAbilitySize(); i++) {
						std::shared_ptr<CAbilityOption> pAbilityOption = pEquipment->GetAdditionalData()->GetAbilityOption(i);
						if (SetAbilityData(
							item_id,
							static_cast<int>(pAbilityOption->GetAbilityType()),
							pAbilityOption->GetRate(),
							pAbilityOption->GetValue()
						) == EXIT_FAILURE)
							return EXIT_FAILURE;
					}
				}

				// 해당 아이템에 대한 Special Ability 데이터 데이터베이스에 저장
				if (pEquipment->GetSpecialData() != nullptr) {
					for (int i = 0; i < pEquipment->GetSpecialData()->GetAbilitySize(); i++) {
						std::shared_ptr<CAbilityOption> pAbilityOption = pEquipment->GetSpecialData()->GetAbilityOption(i);
						if (SetAbilityData(
							item_id,
							static_cast<int>(pAbilityOption->GetAbilityType()),
							pAbilityOption->GetRate(),
							pAbilityOption->GetValue()
						) == EXIT_FAILURE)
							return EXIT_FAILURE;
					}
				}
			}
		}
		else if (cell.second->IsPotion()) {
			std::shared_ptr<CNSW_Potion> pPotion = std::dynamic_pointer_cast<CNSW_Potion>(cell.second);

			// 개별 포션 아이템 데이터 데이터베이스에 저장
				if (pPotion != nullptr
					&& SetCharacterItem(
							character_name,
							item_id,
							cell.first.first, // ItemStoredLocation
							cell.first.second, // ItemStoredIndex
							file_idx, // Index of item_list_path in file
							item_idx, // index of item in item_list
							U_NONE,
							pPotion->GetAmount(), 
							U_NONE,
							U_NONE,
							U_NONE
						) == EXIT_FAILURE
			)
				return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}


bool CDatabaseManager::SetCharacterItem(std::string character_name, std::string item_id, int location, int slot, int file_idx, int item_idx, int defence, int amount, int durability, int attack_power, int magic_power)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND bind[11];
	my_bool isNull = true;

	std::vector<char*> item_data_type_str = { 
		const_cast<char*>(character_name.c_str()),
		const_cast<char*>(item_id.c_str())
	};
	std::vector<int> item_data_type_int = { location, slot, file_idx, item_idx, defence, amount, durability, attack_power, magic_power };

	stmt = mysql_stmt_init(mySQL);

	if (mysql_stmt_prepare(stmt, SP_SET_CHARACTER_ITEM, strlen(SP_SET_CHARACTER_ITEM))) {
		OutputDebugString(_T("mysql_stmt_prepare(), SP_SET_CHARACTER_ITEM FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_param_count(stmt) != 11) {
		OutputDebugString(_T("mysql_stmt_param_count(), NOT EQUAL PARAM COUNT\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	memset(bind, 0, sizeof(bind));

	int idx = 0;
	for (int i = 0; i < item_data_type_str.size(); i++) {
		bind[i].buffer_type = MYSQL_TYPE_STRING;
		bind[i].buffer = item_data_type_str[i];
		bind[i].buffer_length = strlen(item_data_type_str[i]);
	}

	idx += item_data_type_str.size();
	for (int i = 0; i < item_data_type_int.size(); i++) {
		bind[idx + i].buffer_type = MYSQL_TYPE_LONG;
		bind[idx + i].buffer = &item_data_type_int[i];
		bind[idx + i].buffer_length = sizeof(int);

		// 아이템 속성 값이 0인 경우, NULL 값으로 추가
		if (item_data_type_int[i] <= U_NONE) {
			bind[idx + i].is_null = &isNull;
		}
	}

	if (mysql_stmt_bind_param(stmt, bind)) {
		OutputDebugString(_T("mysql_stmt_bind_param(), PARAM BINDING FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_execute(stmt)) {
		OutputDebugString(_T("[SetCharacterItem] mysql_stmt_execute(), Statement Execute FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	mysql_stmt_close(stmt);

	return EXIT_SUCCESS;
}

bool CDatabaseManager::GetCharacterItem(std::string character_name, std::map<std::pair<int, int>, std::shared_ptr<CNSW_Item>>& item_map)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND input_bind[1];
	char* input_id = const_cast<char*>(character_name.c_str());
	stmt = mysql_stmt_init(mySQL);

	// Prepare the statement with SP_GET_CHARACTER_ITEM
	if (mysql_stmt_prepare(stmt, SP_GET_CHARACTER_ITEM, strlen(SP_GET_CHARACTER_ITEM))) {
		OutputDebugString(_T("mysql_stmt_prepare(), SP_GET_CHARACTER_ITEM FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_param_count(stmt) != 1) {
		OutputDebugString(_T("mysql_stmt_param_count(), NOT EQUAL PARAM COUNT\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	memset(input_bind, 0, sizeof(input_bind));

	// Bind parameters
	input_bind[0].buffer_type = MYSQL_TYPE_STRING;
	input_bind[0].buffer = input_id;
	input_bind[0].buffer_length = strlen(input_id);

	if (mysql_stmt_bind_param(stmt, input_bind)) {
		OutputDebugString(_T("mysql_stmt_bind_param(), PARAM BINDING FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_execute(stmt)) {
		OutputDebugString(_T("[GetCharacterItem] mysql_stmt_execute(), Statement Execute FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	// Fetch result metadata
	MYSQL_RES* result = mysql_stmt_result_metadata(stmt);
	if (!result) {
		OutputDebugString(_T("mysql_stmt_result_metadata() FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	int col_cnt = mysql_num_fields(result);
	if (col_cnt != 10) {
		OutputDebugString(_T("invalid column count returned by MySQL\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	MYSQL_BIND output_bind[10];
	memset(output_bind, 0, sizeof(output_bind));

	std::vector<int> item_data_type_int(9, U_NONE);
	char item_id[STRING_SIZE];

	// Bind output parameters
	int idx = 0;
	output_bind[idx].buffer_type = MYSQL_TYPE_STRING;
	output_bind[idx].buffer = item_id;
	output_bind[idx].buffer_length = STRING_SIZE;

	idx++;
	for (int i = 0; i < item_data_type_int.size(); i++) {
		output_bind[idx + i].buffer_type = MYSQL_TYPE_LONG;
		output_bind[idx + i].buffer = &item_data_type_int[i];
		output_bind[idx + i].buffer_length = sizeof(int);
	}

	if (mysql_stmt_bind_result(stmt, output_bind))
	{
		OutputDebugString(_T("mysql_stmt_bind_result() failed\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_store_result(stmt))
	{
		OutputDebugString(_T("mysql_stmt_store_result() failed\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	enum ITEM_TYPE_INT { Location, Slot, GenerateIndex, ItemIndex, AttackPower, MagicPower, Defence, Durability, Amount };
	enum ITEM_TYPE_STR { ItemID };
	std::map<std::pair<int, int>, std::string> ItemIdentifierMap;

	while (!mysql_stmt_fetch(stmt)) {
		// 아이템이 저장될 윈도우 위치(Location)과 아이템 슬롯(index)로 아이템 구분
		std::pair<int, int> item_location = std::make_pair(item_data_type_int[ITEM_TYPE_INT::Location], item_data_type_int[ITEM_TYPE_INT::Slot]);

		// 지금 읽어 온 아이템 데이터가 전에 읽어 오지 않았던 데이터라면,
		if (item_map.find(item_location) == item_map.end()) {
			// 아이템 생성 후 저장
			item_map[item_location] =
				CItemGenerator::GetInstance().ConstructItem(
					item_data_type_int[ITEM_TYPE_INT::GenerateIndex],
					item_data_type_int[ITEM_TYPE_INT::ItemIndex],
					item_data_type_int[ITEM_TYPE_INT::Defence],
					item_data_type_int[ITEM_TYPE_INT::Amount],
					item_data_type_int[ITEM_TYPE_INT::Durability],
					item_data_type_int[ITEM_TYPE_INT::AttackPower],
					item_data_type_int[ITEM_TYPE_INT::MagicPower]
				);

			ItemIdentifierMap[item_location] = item_id;
		}
	}

	mysql_free_result(result);
	mysql_stmt_free_result(stmt);
	mysql_stmt_close(stmt);

	// 읽어 온 아이템들을 인벤토리 및 장비 창에 배치
	for (auto& cell : item_map) {
		int iLocation = cell.first.first;
		int iSlot = cell.first.second;

		// 입력 받은 아이템이 Ability를 가지고 있는 장비 아이템이면
		if (cell.second->IsEquipment()) {
			std::shared_ptr<CAbilityData> pAdditionalData = nullptr;
			std::shared_ptr<CAbilityData> pSpecialData = nullptr;
				
			// 해당 아이템의 Ability를 읽어 와서
			if (GetAbilityData(ItemIdentifierMap[std::make_pair(iLocation, iSlot)], pAdditionalData, pSpecialData) == EXIT_FAILURE) {
				return EXIT_FAILURE;
			}
				
			// 타입 캐스팅
			std::shared_ptr<CNSW_Equipment> pEquipment = std::dynamic_pointer_cast<CNSW_Equipment>(cell.second);

			// 아이템 랭크 계산
			auto CalculateMeanRank = [](int rate_rank, int value_rank) {
				std::vector<int> ranks;

				// Rate 값이 있을 시 추가
				if (rate_rank > 0)
					ranks.push_back(rate_rank);
				// Value 값이 있을 시 추가
				if (value_rank > 0)
					ranks.push_back(value_rank);

				// Ability Rate 값과 Value 값의 옵션 평균
				int option_rank = 0;
				for (auto& cell : ranks)
					option_rank += cell;
				option_rank /= (float)ranks.size();

				return option_rank;
			};

			int item_rank = pEquipment->GetItemRank();
			if (pAdditionalData != nullptr) {
				for (int i = 0; i < pAdditionalData->GetAbilitySize(); i++) {
					int rate_rank = pAdditionalData->GetAbilityOption(i)->GetRateRank();
					int value_rank = pAdditionalData->GetAbilityOption(i)->GetValueRank();

					item_rank += CalculateMeanRank(rate_rank, value_rank);
				}
			}
			if (pSpecialData != nullptr) {
				for (int i = 0; i < pSpecialData->GetAbilitySize(); i++) {
					int rate_rank = pSpecialData->GetAbilityOption(i)->GetRateRank();
					int value_rank = pSpecialData->GetAbilityOption(i)->GetValueRank();

					item_rank += CalculateMeanRank(rate_rank, value_rank);
				}
			}

			// 장비 아이템에 속성 부여
			pEquipment->SetAdditionalData(std::move(pAdditionalData));
			pEquipment->SetSpecialData(std::move(pSpecialData));

			// 계산한 아이템 랭크 적용
			pEquipment->SetItemRank(item_rank);

			cell.second = std::move(pEquipment);
		}
	}

	return EXIT_SUCCESS;
}

bool CDatabaseManager::SetAbilityData(std::string item_id, int ability_id, int ability_rate, int ability_value)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND bind[4];
	my_bool isNull = true;

	char* input_id = const_cast<char*>(item_id.c_str());
	std::vector<int> item_data_type_int = { ability_id, ability_rate, ability_value };

	stmt = mysql_stmt_init(mySQL);

	if (mysql_stmt_prepare(stmt, SP_SET_ABILITY_DATA, strlen(SP_SET_ABILITY_DATA))) {
		OutputDebugString(_T("mysql_stmt_prepare(), SP_SET_ADIITIONAL_ABILITY_DATA FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_param_count(stmt) != 4) {
		OutputDebugString(_T("mysql_stmt_param_count(), NOT EQUAL PARAM COUNT\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	memset(bind, 0, sizeof(bind));

	int idx = 0;
	bind[idx].buffer_type = MYSQL_TYPE_STRING;
	bind[idx].buffer = input_id;
	bind[idx].buffer_length = strlen(input_id);

	idx++;
	for (int i = 0; i < item_data_type_int.size(); i++) {
		bind[idx + i].buffer_type = MYSQL_TYPE_LONG;
		bind[idx + i].buffer = &item_data_type_int[i];
		bind[idx + i].buffer_length = sizeof(int);

		// 아이템 속성 값이 0인 경우, NULL 값으로 추가
		if (item_data_type_int[i] <= U_NONE) {
			bind[idx + i].is_null = &isNull;
		}
	}

	if (mysql_stmt_bind_param(stmt, bind)) {
		OutputDebugString(_T("mysql_stmt_bind_param(), PARAM BINDING FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_execute(stmt)) {
		OutputDebugString(_T("[SetAbilityData] mysql_stmt_execute(), Statement Execute FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	mysql_stmt_close(stmt);

	return EXIT_SUCCESS;
}

bool CDatabaseManager::GetAbilityData(std::string item_id, std::shared_ptr<CAbilityData>& pAdditionalData, std::shared_ptr<CAbilityData>& pSpecialData)
{
	MYSQL_STMT* stmt;
	MYSQL_BIND input_bind[1];
	char* input_id = const_cast<char*>(item_id.c_str());;
	stmt = mysql_stmt_init(mySQL);

	// Prepare the statement with SP_GET_ABILITY_DATA
	if (mysql_stmt_prepare(stmt, SP_GET_ABILITY_DATA, strlen(SP_GET_ABILITY_DATA))) {
		OutputDebugString(_T("mysql_stmt_prepare(), SP_GET_ABILITY_DATA FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_param_count(stmt) != 1) {
		OutputDebugString(_T("mysql_stmt_param_count(), NOT EQUAL PARAM COUNT\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	memset(input_bind, 0, sizeof(input_bind));

	// Bind parameters
	input_bind[0].buffer_type = MYSQL_TYPE_STRING;
	input_bind[0].buffer = input_id;
	input_bind[0].buffer_length = strlen(input_id);

	if (mysql_stmt_bind_param(stmt, input_bind)) {
		OutputDebugString(_T("mysql_stmt_bind_param(), PARAM BINDING FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_execute(stmt)) {
		OutputDebugString(_T("[GetAbilityData] mysql_stmt_execute(), Statement Execute FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	// Fetch result metadata
	MYSQL_RES* result = mysql_stmt_result_metadata(stmt);
	if (!result) {
		OutputDebugString(_T("mysql_stmt_result_metadata() FAILED\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	int col_cnt = mysql_num_fields(result);
	if (col_cnt != 3) {
		OutputDebugString(_T("invalid column count returned by MySQL\n"));
		return EXIT_FAILURE;
	}

	MYSQL_BIND output_bind[3];
	memset(output_bind, 0, sizeof(output_bind));

	std::vector<int> ability_data_type_int(3, U_NONE);

	// Bind output parameters
	for (int i = 0; i < ability_data_type_int.size(); i++) {
		output_bind[i].buffer_type = MYSQL_TYPE_LONG;
		output_bind[i].buffer = &ability_data_type_int[i];
		output_bind[i].buffer_length = sizeof(int);
	}

	if (mysql_stmt_bind_result(stmt, output_bind))
	{
		OutputDebugString(_T("mysql_stmt_bind_result() failed\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	if (mysql_stmt_store_result(stmt))
	{
		OutputDebugString(_T("mysql_stmt_store_result() failed\n"));
		OutputDebugString(CA2CT(mysql_stmt_error(stmt)));
		return EXIT_FAILURE;
	}

	enum ABILITY_TYPE_INT { AbilityID, AbilityRate, AbilityValue };
	while (!mysql_stmt_fetch(stmt)) {
		// 추가 속성을 읽어 온 경우
		if (ability_data_type_int[ABILITY_TYPE_INT::AbilityID] >= static_cast<int>(AbilityType::AdditionalAbility)
			&& ability_data_type_int[ABILITY_TYPE_INT::AbilityID] < static_cast<int>(AbilityType::SpecialAbility)) {
				std::shared_ptr<CAbilityOption> pAbilityOption = std::move(
					CItemGenerator::GetInstance().ConstructAbilityOption(
					ability_data_type_int[ABILITY_TYPE_INT::AbilityID],
					ability_data_type_int[ABILITY_TYPE_INT::AbilityRate],
					ability_data_type_int[ABILITY_TYPE_INT::AbilityValue]
				)
			);

			if (pAbilityOption != nullptr) {
				if (pAdditionalData == nullptr)
					pAdditionalData = std::make_shared<CAbilityData>();

				pAdditionalData->AddAbilityOption(std::move(pAbilityOption));
			}
		}
		// 특수 속성을 읽어 온 경우
		else if (ability_data_type_int[ABILITY_TYPE_INT::AbilityID] >= static_cast<int>(AbilityType::SpecialAbility)
			&& ability_data_type_int[ABILITY_TYPE_INT::AbilityID] < static_cast<int>(AbilityType::Others)) {
			std::shared_ptr<CAbilityOption> pAbilityOption = std::move(
				CItemGenerator::GetInstance().ConstructAbilityOption(
					ability_data_type_int[ABILITY_TYPE_INT::AbilityID],
					ability_data_type_int[ABILITY_TYPE_INT::AbilityRate],
					ability_data_type_int[ABILITY_TYPE_INT::AbilityValue]
				)
			);

			if (pAbilityOption != nullptr) {
				if (pSpecialData == nullptr)
					pSpecialData = std::make_shared<CAbilityData>();

				pSpecialData->AddAbilityOption(std::move(pAbilityOption));
			}
		}
	}

	mysql_free_result(result);
	mysql_stmt_free_result(stmt);
	mysql_stmt_close(stmt);

	return EXIT_SUCCESS;
}

bool CDatabaseManager::LoadCharacterData(std::string character_name, std::shared_ptr<CUserCharacter>& user_character)
{
	// 유저의 스테이터스 정보 저장
	int character_class = U_NONE, character_gender = U_NONE, HP = U_NONE, MP = U_NONE, STR = U_NONE, DEX = U_NONE, INT = U_NONE, LUK = U_NONE;
	if (GetCharacterInfo(character_name, character_class, character_gender, HP, MP, STR, DEX, INT, LUK) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	// 유저가 소유한 아이템들을 저장
	std::map<std::pair<int, int>, std::shared_ptr<CNSW_Item>> ItemLocationMap;
	if (GetCharacterItem(character_name, ItemLocationMap) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	// Set Character Info
	user_character->SetCharacterClass(static_cast<CharacterClass>(character_class));
	user_character->SetCharacterType(CharacterType::User);
	user_character->SetGenderType(static_cast<GenderType>(character_gender));
	user_character->SetName(character_name);

	// Initialize Character Status (temporary)
	user_character->InitializeBasicStatus(HP, MP, HP, MP, STR, DEX, INT, LUK);
	user_character->InitializeAdvancedStatus(0, 0, 10, 10, 10, 1.0f, 1.0f, 0, 0);

	// Set Character Items
	// cell { (location_idx, slot_idx), item }
	for (auto& cell : ItemLocationMap) {
		int location_idx = cell.first.first;
		int slot_idx = cell.first.second;
		std::shared_ptr<CNSW_Item> pItem = cell.second;

		// Item In Inventory
		if (location_idx >= ID_INVENTORY && location_idx < INVENTORY_WINDOW_NUM)
			user_character->InsertItem(pItem, location_idx, slot_idx);
		// Equiped Item
		else if (location_idx == ID_EQUIPMENT)
			user_character->InsertItem(pItem, slot_idx);
	}

	return EXIT_SUCCESS;
}

bool CDatabaseManager::StoreCharacterData(std::shared_ptr<CUserCharacter>& user_character)
{
	std::string character_name = user_character->GetName();

	// 오토 커밋 설정 해제
	if (mysql_autocommit(mySQL, false) != 0) {
		return EXIT_FAILURE;
	}

	// 데이터베이스에 저장된 유저의 아이템 데이터 삭제
	if (InitializeCharacterItem(character_name) == EXIT_FAILURE) {
		mysql_rollback(mySQL);
		return EXIT_FAILURE;
	}

	CBasicStatus pUserBasicStatus = user_character->GetUserStatus()->GetBaseBasicStatus();
	CAdvancedStatus pUserAdvancedStatus = user_character->GetUserStatus()->GetBaseAdvancedStatus();
	// 유저의 스테이터스 정보 저장
	if (SetCharacterInfo(
			character_name, 
			static_cast<int>(user_character->GetCharacterClass()), //character_class
			pUserBasicStatus.GetMaxHealth(), //HP,
			pUserBasicStatus.GetMaxMana(), //MP
			pUserBasicStatus.GetStrength(), //STR
			pUserBasicStatus.GetDexterity(), //DEX
			pUserBasicStatus.GetIntelligence(), //INT
			pUserBasicStatus.GetLuck() //LUK
		) == EXIT_FAILURE) {
		mysql_rollback(mySQL);
		return EXIT_FAILURE;
	}

	std::map<std::pair<int, int>, std::shared_ptr<CNSW_Item>> item_map;

	// 유저가 소유한 인벤토리 아이템들을 저장
	item_map = user_character->GetInventoryItems();
	if (SetCharacterItems(character_name, item_map) == EXIT_FAILURE) {
		mysql_rollback(mySQL);
		return EXIT_FAILURE;
	}	
	
	// 유저가 장착한 아이템들을 저장
	item_map = user_character->GetEquipmentItems();
	if (SetCharacterItems(character_name, item_map) == EXIT_FAILURE) {
		mysql_rollback(mySQL);
		return EXIT_FAILURE;
	}

	// 커밋 처리
	if (mysql_commit(mySQL) != 0) {
		mysql_rollback(mySQL);
		return EXIT_FAILURE;
	}

	// 오토 커밋 설정 허용
	if (mysql_autocommit(mySQL, true) != 0) {
		mysql_rollback(mySQL);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}