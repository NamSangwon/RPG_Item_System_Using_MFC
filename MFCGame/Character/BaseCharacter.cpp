#include "pch.h"
#include "BaseCharacter.h"

CBaseCharacter::~CBaseCharacter()
{
}

void CBaseCharacter::SetName(std::string name)
{
	this->m_strName = name;
}

std::string CBaseCharacter::GetName()
{
	return this->m_strName;
}

void CBaseCharacter::SetCharacterType(CharacterType type)
{
	if (type >= CharacterType::User && type <= CharacterType::Enemy) this->m_nCharacterType = type;
	else this->m_nCharacterType = CharacterType::Err;
}

CharacterType CBaseCharacter::GetCharacterType()
{
	return this->m_nCharacterType;
}

void CBaseCharacter::SetGenderType(GenderType type)
{
	if (type >= GenderType::Male && type <= GenderType::Female) this->m_nGenderType = type;
	else this->m_nGenderType = GenderType::Err;
}

GenderType CBaseCharacter::GetGenderType()
{
	return this->m_nGenderType;
}
