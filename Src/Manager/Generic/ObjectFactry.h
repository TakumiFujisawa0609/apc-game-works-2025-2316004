#pragma once
#include <map>
#include <functional>
#include "../../Template/Singleton.h"
class ObjectFactry
{
	friend class Singleton<ObjectFactry>;
public:
	enum class OBJECT_TYPE
	{
		PLAYER,
		PLAYER_LOGIC,
		PLAYER_CARD_UI,
		PLAYER_CARD_ACTION,
		ENEMY_CARD_UI,
		ENEMY,
		ENEMY_LOGIC,
		ENEMY_CARD_ACTION,
		ONHIT,
	};
	void Create()

private:
	std::map<OBJECT_TYPE, std::function<void(void)>>fuctryTable_;
};

