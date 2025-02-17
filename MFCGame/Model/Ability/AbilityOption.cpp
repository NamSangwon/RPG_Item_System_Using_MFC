#include "pch.h"
#include "AbilityOption.h"

void CAbilityOption::SetAbilityType(AbilityType type)
{
	this->m_AbilityType = type;
}

AbilityType CAbilityOption::GetAbilityType()
{
	return this->m_AbilityType;
}

void CAbilityOption::SetRateRange(int min, int max)
{
	this->m_RateRange = std::make_pair(min, max);
}

std::pair<int, int> CAbilityOption::GetRateRange()
{
	return this->m_RateRange;
}

void CAbilityOption::SetValueRange(int min, int max)
{
	this->m_ValueRange = std::make_pair(min, max);
}

std::pair<int, int> CAbilityOption::GetValueRange()
{
	return this->m_ValueRange;
}

void CAbilityOption::SetRate(int rate)
{
	this->m_nRate = rate;
}

int CAbilityOption::GetRate()
{
	return this->m_nRate;
}

void CAbilityOption::SetValue(int val)
{
	this->m_nValue = val;
}

int CAbilityOption::GetValue()
{
	return this->m_nValue;
}

void CAbilityOption::SetRateRank(int Rank)
{
	this->m_nRateRank = Rank;
}

int CAbilityOption::GetRateRank()
{
	return this->m_nRateRank;
}

void CAbilityOption::SetValueRank(int Rank)
{
	this->m_nValueRank = Rank;
}

int CAbilityOption::GetValueRank()
{
	return this->m_nValueRank;
}

void CAbilityOption::SetTargetType(TargetType type)
{
	this->m_TargetType = type;
}

TargetType CAbilityOption::GetTargetType()
{
	return this->m_TargetType;
}

void CAbilityOption::SetActivationRange(int range)
{
	this->m_nActivationRange = range;
}

int CAbilityOption::GetActivationRange()
{
	return this->m_nActivationRange;
}

void CAbilityOption::SetDuration(int duration)
{
	this->m_nDuration = duration;
}

int CAbilityOption::GetDuration()
{
	return this->m_nDuration;
}

void CAbilityOption::SetActivationTimes(int times)
{
	this->m_nActivationTimes = times;
}

int CAbilityOption::GetActivationTimes()
{
	return this->m_nActivationTimes;
}

void CAbilityOption::SetComment(std::string comment)
{
	this->m_strComment = comment;
}

std::string CAbilityOption::GetAbilityInfo()
{
	std::string result;

	int value_range_each_rank = (m_ValueRange.second - m_ValueRange.first) / MAX_ABILITY_RANK;
	int base_value_in_rank = m_ValueRange.first + value_range_each_rank * (m_nValueRank - 1);
	int min_value_in_rank = base_value_in_rank - value_range_each_rank / 2 < m_ValueRange.first
		? m_ValueRange.first : base_value_in_rank - value_range_each_rank / 2;
	int max_value_in_rank = base_value_in_rank + value_range_each_rank / 2 > m_ValueRange.second
		? m_ValueRange.second : base_value_in_rank + value_range_each_rank / 2;

	int rate_range_each_rank = (m_RateRange.second - m_RateRange.first) / MAX_ABILITY_RANK;
	int base_rate_in_rank = m_RateRange.first + rate_range_each_rank * (m_nRateRank - 1);
	int min_rate_in_rank = base_rate_in_rank - rate_range_each_rank / 2 < m_RateRange.first
		? m_RateRange.first : base_rate_in_rank - rate_range_each_rank / 2;
	int max_rate_in_rank = base_rate_in_rank + rate_range_each_rank / 2 > m_RateRange.second
		? m_RateRange.second : base_rate_in_rank + rate_range_each_rank / 2;

	// Range of (min ~ max) in Rank
	// Have Both Value & Rate Data
	// parameters == 0: curr_rate, 1:min_rate, 2:max_rate, 3: curr_value, 4:min_value, 5:max_value
	if (m_nRate > U_NONE && m_nValue > U_NONE) {
		result = GetComment(
			this->m_nRate,
			min_rate_in_rank,
			max_rate_in_rank,
			this->m_nValue,
			min_value_in_rank,
			max_value_in_rank
		);
	}
	else {
		// Only Have Rate Data
		// parameters == 0: curr_value, 1:min_rate, 2:max_rate
		if (m_nRate > U_NONE) {
			result = GetComment(
				this->m_nRate,
				min_rate_in_rank,
				max_rate_in_rank
			);
		}
		// Only Have Value Data
		// parameters == 0: curr_value, 1:min_value, 2:max_value
		else if (m_nValue > U_NONE) {
			result = GetComment(
				this->m_nValue,
				min_value_in_rank,
				max_value_in_rank
			);
		}
	}

	return result;
}
