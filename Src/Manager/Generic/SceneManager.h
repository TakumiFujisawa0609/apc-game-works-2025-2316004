#pragma once
#include <memory>
#include <chrono>
#include <list>
#include "../../Template/Singleton.h"
#include "../../Common/Fader.h"

// 推奨しませんが、どうしても使いたい方は
#define mainCamera SceneManager::GetInstance().GetCamera().lock()

class SceneBase;
class Fader;
class Camera;

class SceneManager : public Singleton<SceneManager>
{
	//シングルトンにだけ共有する
	friend class Singleton<SceneManager>; 

public:

	// シーン管理用
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME
	};
	
	/// @brief 初期化
	/// @param  
	void Init(void);
	
	/// @brief 3D関連の初期化
	/// @param  
	void Init3D(void);

	/// @brief 更新
	/// @param  
	void Update(void);

	/// @brief 描画
	/// @param  
	void Draw(void);

	/// @brief  リソースの破棄
	/// @param  
	void Release(void);
	
	/// @brief 先頭の（Updataが呼ばれる）シーンを切り替える
	/// @param scene 切り替え先のシーン
	void CreateScene(std::shared_ptr<SceneBase> scene);
	
	/// @brief すべてのシーンを切り替える
	/// @param scene 切り替え先のシーン
	void ChangeAllScene(std::shared_ptr<SceneBase> scene);

	/// <summary>
	/// シーンをプッシュする。スタックの数が増える
	/// 一番上のシーンのUpdataしか呼ばれません。
	/// </summary>
	/// <param name="scene">積むシーン</param>
	
	/// @brief シーンをプッシュする。スタックの数が増える
	/// 一番上のシーンのUpdataしか呼ばれません。
	/// @param scene 
	void PushScene(std::shared_ptr<SceneBase> scene);
	
	/// @brief スタックの頭のシーンを削除する。
	/// ただし、スタック上にシーンが一つしかない場合は、削除しない。
	/// @param  
	void PopScene(void);
	
	/// @brief シーン遷移
	/// @param nextId 変更先のシーン
	void ChangeScene(SCENE_ID nextId);
	
	/// @brief フェードを始める
	/// @param  
	void StartFadeIn(void);
	
	/// @brief 現在のシーンIDを返す
	/// @param  
	/// @return 現在のシーンID
	inline SCENE_ID GetSceneID(void) const { return sceneId_; }
	
	/// @brief デルタタイムを返す
	/// @param  
	/// @return デルタタイム
	inline float GetDeltaTime(void) const { return deltaTime_; }
	
	/// @brief 経過時間を返す
	/// @param  
	/// @return 経過時間
	inline float GetTotalTime(void) const { return totalTime_; }
	
	/// @brief カメラを返す
	/// @param  
	/// @return カメラ
	std::weak_ptr<Camera> GetCamera(void) const { return camera_; }

private:

	SCENE_ID sceneId_;
	SCENE_ID waitSceneId_;

	// 各種シーン
	std::list<std::shared_ptr<SceneBase>> scenes_;

	// フェード
	std::unique_ptr<Fader> fader_;

	// カメラ
	std::shared_ptr<Camera> camera_;

	// シーン遷移中判定
	bool isSceneChanging_;

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;

	//経過時間
	float totalTime_;
	
	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SceneManager();

	// デストラクタも同様
	~SceneManager() = default;

	// デルタタイムをリセットする
	void ResetDeltaTime();

	// シーン遷移
	void DoChangeScene(SCENE_ID sceneId);

	// フェード
	void Fade();

};
