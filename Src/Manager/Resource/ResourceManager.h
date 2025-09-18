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
		PLAYER,
		//アニメーション
		IDLE,
		RUN,
		P_ATTACK1,
	};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	void Init(void);

	/// <summary>
	/// 解放(シーン切替時に一旦解放)
	/// </summary>
	/// <param name=""></param>
	void SceneChangeRelease(void);

	/// <summary>
	/// リソースの完全解放
	/// </summary>
	/// <param name=""></param>
	void Release(void);

	/// <summary>
	/// リソースのロード
	/// </summary>
	/// <param name="src">読み込むリソース</param>
	/// <returns>リソース</returns>
	const Resource& Load(SRC src);

	/// <summary>
	/// リソースの複製ロード(モデル用)
	/// </summary>
	/// <param name="src">複製したいリソース</param>
	/// <returns>複製したリソース</returns>
	int LoadModelDuplicate(SRC src);

private:

	// リソース管理の対象
	std::map<SRC, std::unique_ptr<Resource>> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource&> loadedMap_;

	Resource dummy_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);
	~ResourceManager(void) = default;

	// 内部ロード
	Resource& _Load(SRC src);

};
