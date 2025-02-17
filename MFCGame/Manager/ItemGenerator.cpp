#include "pch.h"
#include "ItemGenerator.h"

#include "../Character/UserCharacter.h"

int CItemGenerator::uniform_random_value(int min, int max)
{
	std::random_device rd;

	std::mt19937 gen(rd());

	std::uniform_int_distribution<int> dist(min, max);
	return dist(gen);
}

int CItemGenerator::exp_random_value(int min, int max, float weight_decision) {
	std::random_device rd;

	std::mt19937 gen(rd());

	std::vector<float> values, weights;

	int devide = 5;
	float interval = (float)(max - min) / (float)devide;
	for (float i = 1; i <= devide; i ++) {
		values.push_back(min + (i - 1) * interval);
		weights.push_back(std::exp(-weight_decision * i));
	}
	if (values.back() != max)
		values[values.size() - 1] = max;

	std::piecewise_constant_distribution<float> dist{ values.begin(), values.end(), weights.begin() };

	int result = dist(gen);
	{
		float max_result = (float)result + 1 > max ? max : (float)result + 1;

		// float index = 4 * (float)(max - min) / (float)(result - min) + 1; 
		float total_weight = std::exp(-1) - std::exp(-5); // y=e^(-px) (p=1 / 1 <= x <= 5) 부분 적분 결과

		if (interval > 0) {
			// 적분 범위 (y == result convert to value in (1 ~ 5))
			std::pair<float, float> range( // (result-min : max-min = 5-1:y-1 (min <-> 1 && max <-> 5))
				result <= min ? 1 : 4 * (float)(result - min) / (float)(max - min) + 1,
				max_result >= max ? 5 : 4 * (float)(max_result - min) / (float)(max - min) + 1
			);
			float integral = std::exp(-range.first) - std::exp(-range.second); // 부분 적분 결과
			prob_for_print += std::format("(weight/total_weight) {:.2f} / {:.2f} = {:.2f}%\n", integral, total_weight, (integral / total_weight) * 100);
		}
		else
			prob_for_print += std::format("(weight/total_weight) {:.2f} / {:.2f} = {:.2f}%\n", total_weight, total_weight, 100.0f);
	}

	return result;
}

std::shared_ptr<CNSW_Item> CItemGenerator::GenerateRandomItem()
{
		prob_for_print = "\n\n[[Generate Random Item]]\n";
	std::shared_ptr<CNSW_Item> pItem = nullptr;

	// temporary string pool test
	if (m_ItemStringPool.size() > 5) {
		for (auto& cell : m_ItemStringPool) {
			cell.second.name = "NAME TEST";
		}
	}
	if (m_AbilityCommentPool.size() > 15) {
		for (auto& cell : m_AbilityCommentPool) {
			cell.second = "ABILITY TEST";
		}
	}

	// 아이템 종류 선택 확률표 읽어 오기
	// 가중치를 통한 랜덤 선택 (Rank 1 - Equipment or Rank 2 - Equipment or Rank 0 - Consume)
		prob_for_print += "[Select Item File]\n";
	std::vector<std::vector<std::string>> vFileData = ReadAllItem("res\\data\\Generate.csv");
	std::vector<std::string> vItemData = RandomSelectItem(vFileData);
		prob_for_print += std::format("\t\t[Selected ({})]\n\n", vItemData[GenerateColumn::FilePath]);
	int generate_idx = stoi(vItemData[GenerateColumn::FileIndex]);

	// 아이템 확률표 읽어 오기
		prob_for_print += "[Select Shared Data of Item]\n";
	vFileData = ReadAllItem(vItemData[GenerateColumn::FilePath]);
	vItemData = RandomSelectItem(vFileData);
		prob_for_print += std::format("\t\t[Select ({})]\n\n", vItemData[ItemColumn::ItemName]);
	int item_idx = stoi(vItemData[ItemColumn::ItemIndex]);

	if (vItemData.empty()) return nullptr;

	// 생성할 아이템 타입의 범위 (Sword == Weapon && Chest == Armor)
	std::pair<ItemType, ItemType> item_type_range;

	// 장비 아이템인 경우
	ItemType type = static_cast<ItemType>(stoi(vItemData[ItemColumn::TypeOfItem]));
	if (type >= ItemType::Equipment && type < ItemType::Consume) {
		// 아이템에 부여될 기본 능력치의 랭크 구하기
		// 방어구 아이템인 경우
		if (type >= ItemType::Armor && type < ItemType::Weapon) {
			item_type_range = std::make_pair(ItemType::Armor, ItemType::Weapon);

				prob_for_print += std::format("[Generate Random Defence Value in ({} ~ {})]\n", vItemData[ItemColumn::MinDefence], vItemData[ItemColumn::MaxDefence]);
			int random_defence = exp_random_value(stoi(vItemData[ItemColumn::MinDefence]), stoi(vItemData[ItemColumn::MaxDefence]), m_fWeightDecision);
				prob_for_print += std::format("\t\t[Select (Defence = {})]\n\n", random_defence);

			pItem = GenerateArmor(generate_idx, item_idx, vItemData, random_defence, U_NONE);
		}
		// 무기 아이템인 경우
		else if (type >= ItemType::Weapon && type < ItemType::Consume) {
			item_type_range = std::make_pair(ItemType::Weapon, ItemType::Consume);

				prob_for_print += std::format("[Generate Random Attack Power Value in ({} ~ {})]\n", vItemData[ItemColumn::MinAttackPower], vItemData[ItemColumn::MaxAttackPower]);
			int random_attack_power = exp_random_value(stoi(vItemData[ItemColumn::MinAttackPower]), stoi(vItemData[ItemColumn::MaxAttackPower]), m_fWeightDecision);
				prob_for_print += std::format("\t\t[Select (Attack Power = {})]\n\n", random_attack_power);

				prob_for_print += std::format("[Generate Random Magic Power Value in ({} ~ {})]\n", vItemData[ItemColumn::MinMagicPower], vItemData[ItemColumn::MaxMagicPower]);
			int random_magic_power = exp_random_value(stoi(vItemData[ItemColumn::MinMagicPower]), stoi(vItemData[ItemColumn::MaxMagicPower]), m_fWeightDecision);
				prob_for_print += std::format("\t\t[Select (Magic Power = {})]\n\n", random_magic_power);

			pItem = GenerateWeapon(generate_idx, item_idx, vItemData, random_attack_power, random_magic_power, U_NONE);
		}

		// 타입 캐스팅
		std::shared_ptr<CNSW_Equipment> pEquipment = std::dynamic_pointer_cast<CNSW_Equipment>(pItem);
		int item_rank = pEquipment->GetItemRank();

		// 장비 아이템 속성 생성
			prob_for_print += "[Generate Random Ability]\n";

		std::shared_ptr<CAbilityData> pAdditionalData = nullptr;
		std::shared_ptr<CAbilityData> pSpecialData = nullptr;
		GenerateRandomAbility(item_type_range, pAdditionalData, pSpecialData);

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

		// 아이템 랭크 계산
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

		pItem = std::move(pEquipment);
	}
	// 소비 아이템인 경우
	else if (type >= ItemType::Consume && type < ItemType::Others) {
		// 포션 아이템인 경우
		if (type >= ItemType::Potion && type < ItemType::Others) {

				prob_for_print += "[Generate Random Amount Value in (1 ~ 10)]\n";
			int random_amount = exp_random_value(1, 10, m_fWeightDecision);
				prob_for_print += std::format("\t\t[Select (Amount = {})]\n\n", random_amount);

			pItem = GeneratePotion(generate_idx, item_idx, vItemData, random_amount);
		}
	}

		AfxMessageBox(prob_for_print.c_str(), IDOK);

	return pItem;
}

std::shared_ptr<CNSW_Item> CItemGenerator::ConstructItem(int generate_idx, int item_idx, int defence, int amount, int durability, int attack_power, int magic_power)
{
	std::shared_ptr<CNSW_Item> pItem = nullptr;

	// 아이템 종류 선택해서 읽어 오기 (Rank 1 - Equipment or Rank 2 - Equipment or Rank 0 - Consume)
	std::vector<std::string> vItemData = ReadSelectedItem("res\\data\\Generate.csv", generate_idx);

	// 아이템을 선택해서 읽어 오기
	vItemData = ReadSelectedItem(vItemData[GenerateColumn::FilePath], item_idx);

	if (vItemData.empty()) return nullptr;

	// 아이템 타입에 맞는 아이템 생성
	ItemType item_type = static_cast<ItemType>(stoi(vItemData[ItemColumn::TypeOfItem]));
	if (item_type >= ItemType::Armor && item_type < ItemType::Weapon) {
		// 방어구 생성 후 반환
		pItem = GenerateArmor(generate_idx, item_idx, vItemData, defence, durability);
	}
	else if (item_type >= ItemType::Weapon && item_type < ItemType::Consume) {
		// 무기 생성 후 반환
		pItem = GenerateWeapon(generate_idx, item_idx, vItemData, attack_power, magic_power, durability);
	}
	else if (item_type >= ItemType::Potion && item_type < ItemType::Others) {
		pItem = GeneratePotion(generate_idx, item_idx, vItemData, amount);
	}

	return std::move(pItem);
}

std::shared_ptr<CNSW_Armor> CItemGenerator::GenerateArmor(int generate_idx, int item_idx, std::vector<std::string> vItemData, int defence, int curr_durability)
{
	std::pair<int, int> item_identifier = std::make_pair(generate_idx, item_idx);

	if (m_ItemStringPool.find(item_identifier) == m_ItemStringPool.end()) {
		m_ItemStringPool[item_identifier] = ItemStringData{ vItemData[ItemColumn::ItemName], vItemData[ItemColumn::ImagePath] };
	}

	// 현재 내구도 입력 여부 ( 입력 X == 풀 내구도 && 입력 O == 입력 받은 내구도)
	curr_durability = curr_durability <= U_NONE ? stoi(vItemData[ItemColumn::Durability]) : curr_durability;

	return std::make_shared<CNSW_Armor>(
		item_identifier,
		m_ItemStringPool[item_identifier].image_path,
		m_ItemStringPool[item_identifier].name,
		static_cast<ItemType>(stoi(vItemData[ItemColumn::TypeOfItem])),
		stoi(vItemData[ItemColumn::Weight]),
		stoi(vItemData[ItemColumn::Rank]),
		stoi(vItemData[ItemColumn::Durability]),
		curr_durability,
		defence
	);
}

std::shared_ptr<CNSW_Weapon> CItemGenerator::GenerateWeapon(int generate_idx, int item_idx, std::vector<std::string> vItemData, int attack_power, int magic_power, int curr_durability)
{
	std::pair<int, int> item_identifier = std::make_pair(generate_idx, item_idx);

	if (m_ItemStringPool.find(item_identifier) == m_ItemStringPool.end()) {
		m_ItemStringPool[item_identifier] = ItemStringData{ vItemData[ItemColumn::ItemName], vItemData[ItemColumn::ImagePath] };
	}

	// 현재 내구도 입력 여부 ( 입력 X == 풀 내구도 && 입력 O == 입력 받은 내구도)
	curr_durability = curr_durability < 0 ? stoi(vItemData[ItemColumn::Durability]) : curr_durability;

	return std::make_shared<CNSW_Weapon>(item_identifier,
				m_ItemStringPool[item_identifier].image_path,
				m_ItemStringPool[item_identifier].name,
				static_cast<ItemType>(stoi(vItemData[ItemColumn::TypeOfItem])),
				stoi(vItemData[ItemColumn::Weight]),
				stoi(vItemData[ItemColumn::Rank]),
				stoi(vItemData[ItemColumn::Durability]),
				curr_durability,
				attack_power,
				magic_power,
				stoi(vItemData[ItemColumn::Range]),
				stof(vItemData[ItemColumn::AttackSpeed])
			);
}

std::shared_ptr<CNSW_Potion> CItemGenerator::GeneratePotion(int generate_idx, int item_idx, std::vector<std::string> vItemData, int amount)
{
	std::pair<int, int> item_identifier = std::make_pair(generate_idx, item_idx);

	if (m_ItemStringPool.find(item_identifier) == m_ItemStringPool.end()) {
		m_ItemStringPool[item_identifier] = ItemStringData{ vItemData[ItemColumn::ItemName], vItemData[ItemColumn::ImagePath] };
	}

	// 1 ~ 10개 사이의 개수를 지니는 랜덤한 포션 생성
	return std::make_shared<CNSW_Potion>(
		item_identifier,
		m_ItemStringPool[item_identifier].image_path,
		m_ItemStringPool[item_identifier].name,
		static_cast<ItemType>(stoi(vItemData[ItemColumn::TypeOfItem])),
		stoi(vItemData[ItemColumn::Weight]),
		stoi(vItemData[ItemColumn::Rank]),
		amount,
		CONSUME_MAX_CAPACITY,
		stoi(vItemData[ItemColumn::RecoverAmount])
	);
}

void CItemGenerator::GenerateRandomAbility(std::pair<ItemType, ItemType> item_type_range, std::shared_ptr<CAbilityData>& additional, std::shared_ptr<CAbilityData>& special)
{
		prob_for_print += std::format("[Generate Random Ability Count (0 ~ {}) ]\n", MAX_ABILITY_NUM, m_fWeightDecision);
	int iAbilitySize = exp_random_value(0, MAX_ABILITY_NUM, m_fWeightDecision);
		prob_for_print += std::format("\t\t[Select (Ability Count = {})]\n\n", iAbilitySize);

	std::vector<std::vector<std::string>> vFileData = ReadAllItem("res\\data\\AbilityList.csv");
	
	// ability_type (== Additional or Special) 에 해당하는 값을 제외한 나머지
	// && 추가할 아이템 타입에 맞는 Ability를 제외하고 삭제
	for (int i = 0; i < vFileData.size(); i++) {
		ItemType type = static_cast<ItemType>(stoi(vFileData[i][AbilityColumn::AvailableItemType]));

		if ((type < item_type_range.first || type >= item_type_range.second) && type != ItemType::Equipment) {
			vFileData.erase(vFileData.begin() + i);
			i--;
			continue;
		}
	}

	// Ability 추가
	for (int i = 0; i < iAbilitySize; i++) {
			prob_for_print += std::format("[Select Random Ability]\n");
			std::vector<std::string> vItemData = RandomSelectItem(vFileData);

		// [[ 랭크 계산 ]]
		// ex. value -> 0 ~ 20
		// [rank_range] (by floor() = 20 / MAX_ABILITY_RANK(=4) = 5)
		// 0 ~ 4 : 0 (by module 5)
		// 5 ~ 9 : 1 (by module 5)
		// 10 ~ 14 : 2 (by module 5)
		// 15 ~ 19 : 3 (by module 5)
		// 20 : 4 (by module 5)
		//  [rank range] (by round())
		// 0 : 0 ~ 2 
		// 1 : 3 ~ 7
		// 2 : 8 ~ 12
		// 3 : 13 ~ 17
		// 4 : 18 ~ 20

		// Ability 랜덤 값 부여 (Rate == 발동 확률, Value = 계수)
		// 확률형 능력 여부 확인
		int iValue = U_NONE, iRate = U_NONE;
			prob_for_print += std::format("[Generate Values of {}]\n", vItemData[AbilityColumn::AbilityName]);

		// 확률형 X = Regenerate HP, Attack Speed, ...
		if (!vItemData[AbilityColumn::MinValue].empty()) {
				prob_for_print += std::format("[Generate Random Ability Value ({} ~ {}) ]\n", vItemData[AbilityColumn::MinValue], vItemData[AbilityColumn::MaxValue], m_fWeightDecision);
			iValue = exp_random_value(stoi(vItemData[AbilityColumn::MinValue]), stoi(vItemData[AbilityColumn::MaxValue]), m_fWeightDecision);
				prob_for_print += std::format("\t\t[Select (Value = {})]\n\n", iValue);
		}

		// 확률형 O = Dodge, BlockDamage, Freeze, ...
		if (!vItemData[AbilityColumn::MinRate].empty()) {
				prob_for_print += std::format("[Generate Random Ability Rate ({} ~ {}) ]\n", vItemData[AbilityColumn::MinRate], vItemData[AbilityColumn::MaxRate], m_fWeightDecision);
			iRate = exp_random_value(stoi(vItemData[AbilityColumn::MinRate]), stoi(vItemData[AbilityColumn::MaxRate]), m_fWeightDecision);
				prob_for_print += std::format("\t\t[Select (Rate = {} %)]\n\n", iRate);
		}

		// Ability Option Generate
		std::shared_ptr<CAbilityOption> pOption = GenerateAbilityOption(vItemData, iValue, iRate);

		// Ability 옵션 부여
		AbilityType ability_type = static_cast<AbilityType>(stoi(vItemData[AbilityColumn::TypeOfAbility]));
		if (ability_type == AbilityType::AdditionalAbility) {
			if (additional == nullptr)
				additional = std::make_shared<CAbilityData>();

			additional->AddAbilityOption(std::move(pOption));
		}
		else {
			if (special == nullptr)
				special = std::make_shared<CAbilityData>();

			special->AddAbilityOption(std::move(pOption));
		}
	}
}

std::shared_ptr<CAbilityOption> CItemGenerator::GenerateAbilityOption(std::vector<std::string> vItemData, int value, int rate)
{
	int ability_index = stoi(vItemData[AbilityColumn::AbilityIndex]);
	if (m_AbilityCommentPool.find(ability_index) == m_AbilityCommentPool.end()) {
		m_AbilityCommentPool[ability_index] = vItemData[AbilityColumn::Comment];
	}

	int nRateRank = U_NONE, nValueRank = U_NONE;

	// Calculated Normal Value Rank
	if (!vItemData[AbilityColumn::MinValue].empty()) {
		int iValueRange = stoi(vItemData[AbilityColumn::MaxValue]) - stoi(vItemData[AbilityColumn::MinValue]);
		
		float rank_ratio = (float)(value - stoi(vItemData[AbilityColumn::MinValue])) / (float)(iValueRange / MAX_ABILITY_RANK) + 1; // change rank_range (0~4) to (1~5)

		nValueRank = round(rank_ratio);
	}

	// Calculated Rate Value Rank
	if (!vItemData[AbilityColumn::MinRate].empty()) {
		int iRateRange = stoi(vItemData[AbilityColumn::MaxRate]) - stoi(vItemData[AbilityColumn::MinRate]);

		float rank_ratio = (float)(rate - stoi(vItemData[AbilityColumn::MinRate])) / (float)(iRateRange / MAX_ABILITY_RANK) + 1; // change rank_range (0~4) to (1~5

		nRateRank = round(rank_ratio);
	}

	// 해당 Ability의 공유 데이터가 없을 시 새로 생성 후 저장
	AbilityType option_type = static_cast<AbilityType>(stoi(vItemData[AbilityColumn::AbilityIndex]));

	// Min & Max of Rate Value
	std::pair<int, int> rate_total_range(U_NONE, U_NONE);
	if (!vItemData[AbilityColumn::MinRate].empty())
		rate_total_range = std::make_pair(stoi(vItemData[AbilityColumn::MinRate]), stoi(vItemData[AbilityColumn::MaxRate]));

	// Min & Max of Normal Value
	std::pair<int, int> value_total_range(U_NONE, U_NONE);
	if (!vItemData[AbilityColumn::MinValue].empty())
		value_total_range = std::make_pair(stoi(vItemData[AbilityColumn::MinValue]), stoi(vItemData[AbilityColumn::MaxValue]));

	// Target Type
	TargetType target_type = vItemData[AbilityColumn::Target].empty() ? TargetType::Err : static_cast<TargetType>(stoi(vItemData[AbilityColumn::Target]));

	// Activation Variable (range, duration, times)
	int iActivationRange = vItemData[AbilityColumn::ActivationRange].empty() ? U_NONE : stoi(vItemData[AbilityColumn::ActivationRange]);
	int iActivationDuration = vItemData[AbilityColumn::ActivationDuration].empty() ? U_NONE : stoi(vItemData[AbilityColumn::ActivationDuration]);
	int iActivationTimes = vItemData[AbilityColumn::ActivationTimes].empty() ? U_NONE : stoi(vItemData[AbilityColumn::ActivationTimes]);

	// Ability 옵션 반환
	return std::make_shared<CAbilityOption>(
		option_type,
		rate_total_range,
		value_total_range,
		rate,
		value,
		nRateRank,
		nValueRank,
		target_type,
		iActivationRange,
		iActivationDuration,
		iActivationTimes,
		m_AbilityCommentPool[ability_index]
	);
}

std::shared_ptr<CAbilityOption> CItemGenerator::ConstructAbilityOption(int ability_index, int ability_rate, int ability_value)
{
	// Ability 인덱스를 통해 값 읽어 오기
	std::vector<std::string> vItemData = ReadSelectedItem("res\\data\\AbilityList.csv", ability_index);

	// 오류
	if (vItemData.empty()) return nullptr;

	return GenerateAbilityOption(vItemData, ability_value, ability_rate);
}

std::vector<std::vector<std::string>> CItemGenerator::ReadAllItem(std::string file_path)
{
	std::ifstream file(file_path);

	if (!file.is_open()) {
		OutputDebugString(_T("Error opening file!\n"));
		return std::vector<std::vector<std::string>>();
	}

	// Read Row Data in File 
	std::string line;
	std::vector<std::vector<std::string>> result;

	while (std::getline(file, line)) {
		// 주석 및 빈 줄 pass
		if (line.empty() || line.front() == '#') continue;

		//// 공백 제거
		//line.erase(remove(line.begin(), line.end(), ' '), line.end());

		// 임시 개행 문자 (^)를 \n로 변경
		std::size_t pos = line.find("^");
		while (pos != std::string::npos) {
			line.replace(pos, 1, "\n");
			pos = line.find("^");
		}

		// 문자열 자르기
		std::vector<std::string> row_data = SplitString(line, ',');

		// 원소 단위로 나눠진 문자열 벡터 저장
		result.push_back(row_data);
		row_data.clear();
	}

	return result;
}

std::vector<std::string> CItemGenerator::ReadSelectedItem(std::string file_path, int idx)
{
	std::ifstream file(file_path);

	if (!file.is_open()) {
		OutputDebugString(_T("Error opening file!\n"));
		return std::vector<std::string>();
	}

	// Read Row Data in File 
	std::string line;
	std::vector<std::string> result;

	while (std::getline(file, line)) {
		// 주석 및 빈 줄 pass
		if (line.empty() || line.front() == '#') continue;

		//// 공백 제거
		// line.erase(remove(line.begin(), line.end(), ' '), line.end());

		// 임시 개행 문자 (^)를 \n로 변경
		std::size_t pos = line.find("^");
		while (pos != std::string::npos) {
			line.replace(pos, 1, "\n");
			pos = line.find("^");
		}

		// 문자열 자르기
		std::vector<std::string> data = SplitString(line, ',');

		// 찾고자 하는 값이면, 결과 값 바로 반환
		if (idx == stoi(data.front())) {
			result = data;
			break;
		}
	}

	return result;
}

std::vector<std::string> CItemGenerator::RandomSelectItem(std::vector<std::vector<std::string>>& file_data)
{
	// 파일 내의 모든 아이템의 가중치 합
	int total_weight = 0;
	for (auto& cell : file_data)
		total_weight += stoi(cell.back());

	// 해당 파일 내의 모든 아이템의 가중치를 기준으로 아이템 선택
	// (Prob = 해당 아이템의 가중치 / 파일 내의 모든 아이템의 가중치 합)
	int iItemData = uniform_random_value(0, total_weight);

	// (Prob = 해당 아이템의 가중치 / 파일 내의 모든 아이템의 가중치 합)
	// 위 확률에 해당되는 아이템 데이터 반환
	int weight = 0;
	std::vector<std::string> result;
	for (auto& cell : file_data) {
		weight += stoi(cell.back());
		if (weight >= iItemData) {
			result = cell;
			// 중복 제거를 위한 아이템 삭제
			file_data.erase(remove(file_data.begin(), file_data.end(), cell), file_data.end());
			break;
		}
	}

		float prob = (stoi(result.back()) / (float)total_weight) * 100;
		prob_for_print += std::format(" (weight / total_weight) {} / {} == {:.2f}%\n", result.back(), total_weight, prob);

	return result;
}

std::vector<std::string> CItemGenerator::SplitString(std::string str, char Delimiter)
{
	std::istringstream iss(str);
	std::string buffer;
	std::vector<std::string> result;

	while (getline(iss, buffer, Delimiter)) {
		result.push_back(buffer); 
	}

	return result;
}