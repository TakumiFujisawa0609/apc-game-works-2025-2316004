#pragma once
#include <unordered_map>
#include <functional>
#include <memory>
#include "../Template/Singleton.h"

class ObjectBase;
class ObjectFactory :public Singleton<ObjectFactory>
{
	friend class Singleton<ObjectFactory>;
public:

	enum class OBJECT_TYPE
	{
		NONE = -1,
		CARD_DECK,
		PLAYER_CARD_UI,
		ENEMY_CARD_UI,
		ENEMY_CARD_ACTION,
		IDLE_ACTION,
		JUMP_ACTION,
		REACT_ACTION,
		RUN_ACTION,
		DODGE_ACTION,
		STAGE_OBJECT,

		MAX
	};

	/// @brief オブジェクト生成関数登録
	void RegisterFactoryFunction(OBJECT_TYPE type, std::function<std::unique_ptr<ObjectBase>(void)> func)
	{
		factoryMap_[type] = func;
	}


protected:
	/// @brief オブジェクト生成
	std::unique_ptr<ObjectBase> CreateObject(OBJECT_TYPE type);

private:
	std::unordered_map<OBJECT_TYPE, std::function<std::unique_ptr<ObjectBase>(void)>> factoryMap_;
};



