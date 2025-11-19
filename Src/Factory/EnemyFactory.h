#pragma once
#include "CharacterFactory.h"
class Enemy;

class EnemyFactory :
    public CharacterFactory
{
protected:
	virtual std::unique_ptr<CharacterBase>CreateObject(void) override;
};

