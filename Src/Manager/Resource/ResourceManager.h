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
		STAGE,
		PLAYER,
		ENEMY_1,
		//アニメーション
		//プレイヤー
		P_IDLE,
		P_RUN,
		P_ATTACK_1,
		P_ATTACK_2,
		P_ATTACK_3,
		//敵
		E_IDLE,
		E_RUN,
		E_ATTACK1,
		E_ATTACK2,
		E_ATTACK3,
		REACT,
		//画像
		NUMBERS_IMG,	//カード番号
		ATK_CARD_IMG,	//番号なしアタックカード
		RELOAD_CARD_IMG,//リロードカード
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
	~ResourceManager(void) = default;

	// 内部ロード
	ResourceData& _Load(SRC src);

};
