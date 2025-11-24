#pragma once
#include<vector>
#include "../../Template/Singleton.h"

class CharacterBase;
class Player;
class Enemy;
class CharacterManager :public Singleton<CharacterManager>
{
	// シングルトンにだけ共有する
	friend class Singleton<CharacterManager>;
public:
	static constexpr int CHARA_MAX = 2;


	void Load(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	/// @brief ゲームクリアシーン遷移条件(敵のHPによるもの)
	/// @param  
	/// @return 
	const bool IsSceneChageClearCondition(void)const;

	/// @brief ゲームオーバーシーン遷移
	/// @param  
	/// @return 
	const bool IsSceneChangeGameOverCondition(void)const;

private:
	CharacterManager(void);
	~CharacterManager(void)override;
	std::unique_ptr<Player>player_;
	std::unique_ptr<Enemy>enemy_;
	//std::vector<std::unique_ptr<CharacterBase>> characters_;
};

