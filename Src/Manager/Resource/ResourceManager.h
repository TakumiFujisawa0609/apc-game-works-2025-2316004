#pragma once
#include <map>
#include <string>
#include "../../Template/Singleton.h"
#include "Resource.h"

class ResourceManager : public Singleton<ResourceManager>
{

	friend class Singleton<ResourceManager>;

public:

	/// <summary>
	/// リソース名
	/// </summary>
	enum class SRC
	{
		NONE,
		TEST,
		//モデル
		STAGE,		//ステージ
		PLAYER,		//プレイヤー
		ENEMY_1,	//敵1
		CARD_MDL,		//カード
		SKY_DOME,	//スカイドーム
		SPHERE_ROCK,		//岩
		KEY_BLADE,	//鍵剣
		//アニメーション
		//プレイヤー
		P_IDLE,		//待機
		P_RUN,		//走る
		P_ATTACK_1,	//攻撃1
		P_ATTACK_2,	//攻撃2
		P_ATTACK_3,	//攻撃3
		P_JUMP,		//ジャンプ
		P_DODGE,	//回避
		P_RELOAD,	//リロード
		//敵
		E_IDLE,		//待機
		E_RUN,		//走る
		E_STOMP_ATK,	//横薙ぎ攻撃
		E_JUMP_ATK, //ジャンプ攻撃
		E_ROAR_ATK,	//咆哮攻撃
		E_ROLE_ATK,	//転がり攻撃
		E_KNOCK_DOWN,	//大ダウン
		REACT,

		//画像
		TITLE_BACK_IMG,	//タイトル背景
		TITLE_LOGO,		//タイトルロゴ
		GAME_OVER_IMG,	//
		GAME_CLEAR_IMG,
		NUMBERS_IMG,	//カード番号
		PLAYER_ATK_CARD_IMG,	//番号なしアタックカード
		ENEMY_ATK_CARD_IMG,//敵番号なしアタックカード
		RELOAD_CARD_IMG,//リロードカード
		RELOAD_FRAME,	//リロードカードのフレーム
		RELOAD_GAGE,	//リロードゲージ
		CARD_SELECT_FRAME_IMG,//カード枠画像


		//ピクセルシェーダ
		CARD_PS,

		//エフェクト
		BLAST,
		KEY_BLADE_HIT_EFF
	};
	
	/// @brief 初期化
	/// @param  
	void Init(void);
	
	/// @brief 解放(シーン切替時に一旦解放)
	/// @param  
	void SceneChangeRelease(void);
	
	/// @brief リソースの完全解放
	/// @param  
	void Release(void);
	
	/// @brief リソースのロード
	/// @param src 読み込むリソース
	/// @return リソース
	const ResourceData& Load(SRC src);
	
	/// @brief リソースの複製ロード(モデル用)
	/// @param src 複製したいリソース
	/// @return 複製したリソース
	int LoadModelDuplicate(SRC src);

private:
	//カード番号画像
	static constexpr int CARD_NO_X = 5;	//横の数
	static constexpr int CARD_NO_Y = 2;	//縦の数
	static constexpr int CARD_NO_SIZE_X = 180;
	static constexpr int CARD_NO_SIZE_Y = 222;

	// リソース管理の対象
	std::map<SRC, std::unique_ptr<ResourceData>> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, ResourceData&> loadedMap_;

	ResourceData dummy_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);
	~ResourceManager(void);

	// 内部ロード
	ResourceData& _Load(SRC src);

};
