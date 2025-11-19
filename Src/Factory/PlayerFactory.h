#pragma once
#include "CharacterFactory.h"
class Player;
class PlayerFactory :
    public CharacterFactory
{
protected:
	virtual std::unique_ptr<CharacterBase>CreateObject(void) override;
};

