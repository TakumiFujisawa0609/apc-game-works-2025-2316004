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

	/// @brief ロード
	/// @param  
	void Load(void);

	/// @brief 初期化
	/// @param  
	void Init(void);

	/// @brief 更新
	/// @param  
	void Update(void);

	/// @brief 演出時の更新
	/// @param  
	void DirectionUpdate(void);

	/// @brief 描画
	/// @param  
	void Draw(void);

	/// @brief 2D描画
	/// @param  
	void Draw2D(void);

	/// @brief 解放
	/// @param  
	void Release(void);

	/// @brief ゲームクリアシーン遷移条件(敵のHPによるもの)
	/// @param  
	/// @return 
	const bool IsSceneChageClearCondition(void)const;

	/// @brief ゲームオーバーシーン遷移
	/// @param  
	/// @return 
	const bool IsSceneChangeGameOverCondition(void)const;

	/// @brief 演出中の敵の咆哮状態取得
	/// @param  
	/// @return 
	const bool GetIsEnemyRoar(void)const;

private:
	CharacterManager(void);
	~CharacterManager(void)override;
	std::shared_ptr<Player>player_;
	std::shared_ptr<Enemy>enemy_;
	//std::vector<std::unique_ptr<CharacterBase>> characters_;
};

