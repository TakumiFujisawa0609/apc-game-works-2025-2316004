#pragma once
#include <map>
#include <string>
#include "../../Template/Singleton.h"
#include "Resource.h"

class ResourceManager : public Singleton<ResourceManager>
{

	friend class Singleton<ResourceManager>;

public:

	//リソース
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
		P_ATTACK_1_SHORT,	//攻撃1(近距離)
		P_ATTACK_1_MIDDLE,	//攻撃1(中距離)
		P_ATTACK_2,	//攻撃2
		P_ATTACK_3,	//攻撃3
		P_JUMP,		//ジャンプ
		P_DODGE,	//回避
		P_RELOAD,	//リロード
		P_DEATH,	//プレイヤー倒れ
		//敵
		E_IDLE,		//待機
		E_RUN,		//走る
		E_STOMP_ATK,	//横薙ぎ攻撃
		E_JUMP_ATK, //ジャンプ攻撃
		E_ROAR_ATK,	//咆哮攻撃
		E_ROLE_ATK,	//転がり攻撃
		E_KNOCK_DOWN,	//大ダウン
		REACT,
		E_DEATH,			//敵倒れる

		//画像
		TITLE_BACK_IMG,	//タイトル背景
		TITLE_LOGO,		//タイトルロゴ
		GAME_OVER_IMG,	//
		GAME_CLEAR_IMG,
		PLAYER_ATK_CARD_IMG,	//番号なしアタックカード
		ENEMY_ATK_CARD_IMG,//敵番号なしアタックカード
		RELOAD_CARD_IMG,//リロードカード
		RELOAD_FRAME,	//リロードカードのフレーム
		RELOAD_GAGE,	//リロードゲージ
		P_HP_ARCBAR_MASK,	//HPバーマスク(円形の部分)
		P_HP_ARCBAR_FRAME,	//円形ゲージフレーム
		P_HP_LINEBAR_MASK,	//HPバーマスク(線形の部分)
		P_HP_BAR_FRAME,	//HPフレーム
		P_HP_ARC_OUTLINE,	//円形アウトライン
		P_HP_LINE_OUT_LINE,	//線形アウトライン
		P_CARD_NUM_GAUGE_MASK,	//カード枚数ゲージマスク
		P_CARD_NUM_GAUGE_FRAME,	//カード枚数ゲージフレーム
		P_CARD_NUM_GAUGE_BACK,	//カード枚数ゲージ背景
		CARD_SELECT_FRAME_IMG,//カード枠画像
		E_HP_BAR_MASK,	//敵体力体力ゲージマスク
		E_HP_BAR_FRAME,	//敵体力体力ゲージフレーム
		E_HP_COVER,		//敵体力ゲージカバー
		SKIP_BUTTOM,		//スキップボタン画像
		SKIP_BUTTOM_MASK,		//スキップボタン画像
		CARD_REVOLVER_L_ARROW,	//カードリボルバー回転方向の左矢印
		CARD_REVOLVER_R_ARROW,	//カードリボルバー回転方向の右矢印

		INTENSIVE_LINE_1,	//集中線画像1(複数画像でアニメーション)
		INTENSIVE_LINE_2,	//集中線画像2(複数画像でアニメーション)

		//複数画像
		NUMBERS_IMGS,	//カード番号
		CONTROLLER_UI_IMGS,


		//ピクセルシェーダ
		CARD_PS,

		//エフェクト
		BLAST,
		KEY_BLADE_HIT_EFF,
		E_JUMP_CHARGE_EFF,
		E_DEATH_EFF,
		RELOAD_EFF,
		RELOAD_END_EFF,
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

	//コントローラーUI画像
	static constexpr int CONTROLLER_UI_NO_X = 10;
	static constexpr int CONTROLLER_UI_NO_Y = 10;
	static constexpr int CONTROLLER_UI_SIZE_X = 128;
	static constexpr int CONTROLLER_UI_SIZE_Y = 128;

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
