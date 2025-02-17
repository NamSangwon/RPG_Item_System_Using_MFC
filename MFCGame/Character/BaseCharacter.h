#pragma once
class CBaseCharacter
{
public:
	CBaseCharacter() 
	{
		this->m_strName = "Character";
		this->m_nCharacterType = CharacterType::Err;
		this->m_nGenderType = GenderType::Err;
	}

	CBaseCharacter(const CBaseCharacter& character) : 
		CBaseCharacter(character.m_strName, character.m_nCharacterType, character.m_nGenderType) {}

	CBaseCharacter(std::string name, CharacterType character_type, GenderType gender_type) : m_strName(name)
	{
		SetCharacterType(character_type);
		SetGenderType(gender_type);
	}

	~CBaseCharacter();

	void SetName(std::string name);
	std::string GetName();
	void SetCharacterType(CharacterType type);
	CharacterType GetCharacterType();
	void SetGenderType(GenderType type);
	GenderType GetGenderType();

protected:
	std::string m_strName;
	CharacterType m_nCharacterType;
	GenderType m_nGenderType;
};


