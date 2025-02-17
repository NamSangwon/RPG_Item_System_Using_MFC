#pragma once

class CAbilityOption
{
public:
	//CAbilityOption() :
	//	m_AbilityType(AbilityType::Err),
	//	m_RateRange(std::make_pair(U_NONE, U_NONE)),
	//	m_ValueRange(std::make_pair(U_NONE, U_NONE)),
	//	m_nRate(U_NONE),
	//	m_nValue(U_NONE),
	//	m_nRateRank(U_NONE),
	//	m_nValueRank (U_NONE),
	//	m_TargetType(TargetType::Err),
	//	m_nActivationRange(U_NONE),
	//	m_nDuration(U_NONE),
	//	m_nActivationTimes(U_NONE),
	//	m_strComment() {}

	CAbilityOption(AbilityType ability_type, std::pair<int, int> rate_range, std::pair<int, int> value_range, int rate, int value, int rate_rank, int value_rank, TargetType target_type, int range, int duration, int times, std::string& comment) :
		m_AbilityType(ability_type),
		m_RateRange(rate_range),
		m_ValueRange(value_range),
		m_nRate(rate),
		m_nValue(value),
		m_nRateRank(rate_rank),
		m_nValueRank(value_rank),
		m_TargetType(target_type),
		m_nActivationRange(range),
		m_nDuration(duration),
		m_nActivationTimes(times),
		m_strComment(comment) {}

	void SetAbilityType(AbilityType type);
	AbilityType GetAbilityType();

	void SetRateRange(int min, int max);
	std::pair<int, int> GetRateRange();

	void SetValueRange(int min, int max);
	std::pair<int, int> GetValueRange();

	void SetRate(int rate);
	int GetRate();	
	
	void SetValue(int val);
	int GetValue();

	void SetRateRank(int Rank);
	int GetRateRank();

	void SetValueRank(int Rank);
	int GetValueRank();

	void SetTargetType(TargetType type);
	TargetType GetTargetType();

	void SetActivationRange(int range);
	int GetActivationRange();

	void SetDuration(int duration);
	int GetDuration();

	void SetActivationTimes(int times);
	int GetActivationTimes();

	void SetComment(std::string comment);
	// 템플릿 함수의 경우는 그 특성상 헤더에 정의를 해야 함
	template<typename... Args>
	std::string GetComment(Args&&... args)
	{
		return std::vformat(this->m_strComment, std::make_format_args(args...));
	}

	std::string GetAbilityInfo();

private:
	AbilityType m_AbilityType;
	std::pair<int, int> m_RateRange; // {min, max}
	std::pair<int, int> m_ValueRange; // {min, max}
	int m_nRate;
	int m_nValue;
	int m_nRateRank;
	int m_nValueRank;
	TargetType m_TargetType;
	int m_nActivationRange;
	int m_nDuration; // millisecond
	int m_nActivationTimes;
	std::string& m_strComment;
};

