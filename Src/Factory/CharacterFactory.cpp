#include "../pch.h"
#include "../Object/Character/Base/CharacterBase.h"
#include "CharacterFactory.h"

std::unique_ptr<CharacterBase> CharacterFactory::Create(CHARACTER_TYPE type)
{
	auto it = characterMap_.find(type);
	if (it != characterMap_.end())
	{
		return it->second()->CreateObject();
	}
	return nullptr;
}
