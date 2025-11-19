#pragma once
#include "../Template/Singleton.h"

class CharacterBase;

class CharacterFactory :public Singleton<CharacterFactory>
{
private:
	enum class CHARACTER_TYPE
	{
		NONE = -1,
		PLAYER,
		ENEMY,
		MAX
	};

	/// @brief キャラクター生成
	/// @param type 作りたいキャラクターのタイプ
	/// @return 
	std::unique_ptr<CharacterBase> Create(CHARACTER_TYPE type);


protected:
	virtual std::unique_ptr<CharacterBase>CreateObject(void) = 0;

	std::unordered_map<CHARACTER_TYPE, std::function<std::unique_ptr<CharacterFactory>(void)>>characterMap_;
};


//#pragma once
//#include "CharacterFactory.h"
//class Player;
//class PlayerFactory :
//	public CharacterFactory
//{
//protected:
//	virtual std::unique_ptr<CharacterBase>CreateObject(void) override;
//};
//
//#include "CharacterFactory.h"
//class Enemy;
//class EnemyFactory :
//	public CharacterFactory
//{
//protected:
//	virtual std::unique_ptr<CharacterBase>CreateObject(void) override;
//};
