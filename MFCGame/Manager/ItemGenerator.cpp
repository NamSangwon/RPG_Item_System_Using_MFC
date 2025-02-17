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
		float total_weight = std::exp(-1) - std::exp(-5); // y=e^(-px) (p=1 / 1 <= x <= 5) �κ� ���� ���

		if (interval > 0) {
			// ���� ���� (y == result convert to value in (1 ~ 5))
			std::pair<float, float> range( // (result-min : max-min = 5-1:y-1 (min <-> 1 && max <-> 5))
				result <= min ? 1 : 4 * (float)(result - min) / (float)(max - min) + 1,
				max_result >= max ? 5 : 4 * (float)(max_result - min) / (float)(max - min) + 1
			);
			float integral = std::exp(-range.first) - std::exp(-range.second); // �κ� ���� ���
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

	// ������ ���� ���� Ȯ��ǥ �о� ����
	// ����ġ�� ���� ���� ���� (Rank 1 - Equipment or Rank 2 - Equipment or Rank 0 - Consume)
		prob_for_print += "[Select Item File]\n";
	std::vector<std::vector<std::string>> vFileData = ReadAllItem("res\\data\\Generate.csv");
	std::vector<std::string> vItemData = RandomSelectItem(vFileData);
		prob_for_print += std::format("\t\t[Selected ({})]\n\n", vItemData[GenerateColumn::FilePath]);
	int generate_idx = stoi(vItemData[GenerateColumn::FileIndex]);

	// ������ Ȯ��ǥ �о� ����
		prob_for_print += "[Select Shared Data of Item]\n";
	vFileData = ReadAllItem(vItemData[GenerateColumn::FilePath]);
	vItemData = RandomSelectItem(vFileData);
		prob_for_print += std::format("\t\t[Select ({})]\n\n", vItemData[ItemColumn::ItemName]);
	int item_idx = stoi(vItemData[ItemColumn::ItemIndex]);

	if (vItemData.empty()) return nullptr;

	// ������ ������ Ÿ���� ���� (Sword == Weapon && Chest == Armor)
	std::pair<ItemType, ItemType> item_type_range;

	// ��� �������� ���
	ItemType type = static_cast<ItemType>(stoi(vItemData[ItemColumn::TypeOfItem]));
	if (type >= ItemType::Equipment && type < ItemType::Consume) {
		// �����ۿ� �ο��� �⺻ �ɷ�ġ�� ��ũ ���ϱ�
		// �� �������� ���
		if (type >= ItemType::Armor && type < ItemType::Weapon) {
			item_type_range = std::make_pair(ItemType::Armor, ItemType::Weapon);

				prob_for_print += std::format("[Generate Random Defence Value in ({} ~ {})]\n", vItemData[ItemColumn::MinDefence], vItemData[ItemColumn::MaxDefence]);
			int random_defence = exp_random_value(stoi(vItemData[ItemColumn::MinDefence]), stoi(vItemData[ItemColumn::MaxDefence]), m_fWeightDecision);
				prob_for_print += std::format("\t\t[Select (Defence = {})]\n\n", random_defence);

			pItem = GenerateArmor(generate_idx, item_idx, vItemData, random_defence, U_NONE);
		}
		// ���� �������� ���
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

		// Ÿ�� ĳ����
		std::shared_ptr<CNSW_Equipment> pEquipment = std::dynamic_pointer_cast<CNSW_Equipment>(pItem);
		int item_rank = pEquipment->GetItemRank();

		// ��� ������ �Ӽ� ����
			prob_for_print += "[Generate Random Ability]\n";

		std::shared_ptr<CAbilityData> pAdditionalData = nullptr;
		std::shared_ptr<CAbilityData> pSpecialData = nullptr;
		GenerateRandomAbility(item_type_range, pAdditionalData, pSpecialData);

		auto CalculateMeanRank = [](int rate_rank, int value_rank) {
			std::vector<int> ranks;

			// Rate ���� ���� �� �߰�
			if (rate_rank > 0)
				ranks.push_back(rate_rank);
			// Value ���� ���� �� �߰�
			if (value_rank > 0)
				ranks.push_back(value_rank);

			// Ability Rate ���� Value ���� �ɼ� ���
			int option_rank = 0;
			for (auto& cell : ranks)
				option_rank += cell;
			option_rank /= (float)ranks.size();

			return option_rank;
		};

		// ������ ��ũ ���
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

		// ��� �����ۿ� �Ӽ� �ο�
		pEquipment->SetAdditionalData(std::move(pAdditionalData));
		pEquipment->SetSpecialData(std::move(pSpecialData));

		// ����� ������ ��ũ ����
		pEquipment->SetItemRank(item_rank);

		pItem = std::move(pEquipment);
	}
	// �Һ� �������� ���
	else if (type >= ItemType::Consume && type < ItemType::Others) {
		// ���� �������� ���
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

	// ������ ���� �����ؼ� �о� ���� (Rank 1 - Equipment or Rank 2 - Equipment or Rank 0 - Consume)
	std::vector<std::string> vItemData = ReadSelectedItem("res\\data\\Generate.csv", generate_idx);

	// �������� �����ؼ� �о� ����
	vItemData = ReadSelectedItem(vItemData[GenerateColumn::FilePath], item_idx);

	if (vItemData.empty()) return nullptr;

	// ������ Ÿ�Կ� �´� ������ ����
	ItemType item_type = static_cast<ItemType>(stoi(vItemData[ItemColumn::TypeOfItem]));
	if (item_type >= ItemType::Armor && item_type < ItemType::Weapon) {
		// �� ���� �� ��ȯ
		pItem = GenerateArmor(generate_idx, item_idx, vItemData, defence, durability);
	}
	else if (item_type >= ItemType::Weapon && item_type < ItemType::Consume) {
		// ���� ���� �� ��ȯ
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

	// ���� ������ �Է� ���� ( �Է� X == Ǯ ������ && �Է� O == �Է� ���� ������)
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

	// ���� ������ �Է� ���� ( �Է� X == Ǯ ������ && �Է� O == �Է� ���� ������)
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

	// 1 ~ 10�� ������ ������ ���ϴ� ������ ���� ����
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
	
	// ability_type (== Additional or Special) �� �ش��ϴ� ���� ������ ������
	// && �߰��� ������ Ÿ�Կ� �´� Ability�� �����ϰ� ����
	for (int i = 0; i < vFileData.size(); i++) {
		ItemType type = static_cast<ItemType>(stoi(vFileData[i][AbilityColumn::AvailableItemType]));

		if ((type < item_type_range.first || type >= item_type_range.second) && type != ItemType::Equipment) {
			vFileData.erase(vFileData.begin() + i);
			i--;
			continue;
		}
	}

	// Ability �߰�
	for (int i = 0; i < iAbilitySize; i++) {
			prob_for_print += std::format("[Select Random Ability]\n");
			std::vector<std::string> vItemData = RandomSelectItem(vFileData);

		// [[ ��ũ ��� ]]
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

		// Ability ���� �� �ο� (Rate == �ߵ� Ȯ��, Value = ���)
		// Ȯ���� �ɷ� ���� Ȯ��
		int iValue = U_NONE, iRate = U_NONE;
			prob_for_print += std::format("[Generate Values of {}]\n", vItemData[AbilityColumn::AbilityName]);

		// Ȯ���� X = Regenerate HP, Attack Speed, ...
		if (!vItemData[AbilityColumn::MinValue].empty()) {
				prob_for_print += std::format("[Generate Random Ability Value ({} ~ {}) ]\n", vItemData[AbilityColumn::MinValue], vItemData[AbilityColumn::MaxValue], m_fWeightDecision);
			iValue = exp_random_value(stoi(vItemData[AbilityColumn::MinValue]), stoi(vItemData[AbilityColumn::MaxValue]), m_fWeightDecision);
				prob_for_print += std::format("\t\t[Select (Value = {})]\n\n", iValue);
		}

		// Ȯ���� O = Dodge, BlockDamage, Freeze, ...
		if (!vItemData[AbilityColumn::MinRate].empty()) {
				prob_for_print += std::format("[Generate Random Ability Rate ({} ~ {}) ]\n", vItemData[AbilityColumn::MinRate], vItemData[AbilityColumn::MaxRate], m_fWeightDecision);
			iRate = exp_random_value(stoi(vItemData[AbilityColumn::MinRate]), stoi(vItemData[AbilityColumn::MaxRate]), m_fWeightDecision);
				prob_for_print += std::format("\t\t[Select (Rate = {} %)]\n\n", iRate);
		}

		// Ability Option Generate
		std::shared_ptr<CAbilityOption> pOption = GenerateAbilityOption(vItemData, iValue, iRate);

		// Ability �ɼ� �ο�
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

	// �ش� Ability�� ���� �����Ͱ� ���� �� ���� ���� �� ����
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

	// Ability �ɼ� ��ȯ
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
	// Ability �ε����� ���� �� �о� ����
	std::vector<std::string> vItemData = ReadSelectedItem("res\\data\\AbilityList.csv", ability_index);

	// ����
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
		// �ּ� �� �� �� pass
		if (line.empty() || line.front() == '#') continue;

		//// ���� ����
		//line.erase(remove(line.begin(), line.end(), ' '), line.end());

		// �ӽ� ���� ���� (^)�� \n�� ����
		std::size_t pos = line.find("^");
		while (pos != std::string::npos) {
			line.replace(pos, 1, "\n");
			pos = line.find("^");
		}

		// ���ڿ� �ڸ���
		std::vector<std::string> row_data = SplitString(line, ',');

		// ���� ������ ������ ���ڿ� ���� ����
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
		// �ּ� �� �� �� pass
		if (line.empty() || line.front() == '#') continue;

		//// ���� ����
		// line.erase(remove(line.begin(), line.end(), ' '), line.end());

		// �ӽ� ���� ���� (^)�� \n�� ����
		std::size_t pos = line.find("^");
		while (pos != std::string::npos) {
			line.replace(pos, 1, "\n");
			pos = line.find("^");
		}

		// ���ڿ� �ڸ���
		std::vector<std::string> data = SplitString(line, ',');

		// ã���� �ϴ� ���̸�, ��� �� �ٷ� ��ȯ
		if (idx == stoi(data.front())) {
			result = data;
			break;
		}
	}

	return result;
}

std::vector<std::string> CItemGenerator::RandomSelectItem(std::vector<std::vector<std::string>>& file_data)
{
	// ���� ���� ��� �������� ����ġ ��
	int total_weight = 0;
	for (auto& cell : file_data)
		total_weight += stoi(cell.back());

	// �ش� ���� ���� ��� �������� ����ġ�� �������� ������ ����
	// (Prob = �ش� �������� ����ġ / ���� ���� ��� �������� ����ġ ��)
	int iItemData = uniform_random_value(0, total_weight);

	// (Prob = �ش� �������� ����ġ / ���� ���� ��� �������� ����ġ ��)
	// �� Ȯ���� �ش�Ǵ� ������ ������ ��ȯ
	int weight = 0;
	std::vector<std::string> result;
	for (auto& cell : file_data) {
		weight += stoi(cell.back());
		if (weight >= iItemData) {
			result = cell;
			// �ߺ� ���Ÿ� ���� ������ ����
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