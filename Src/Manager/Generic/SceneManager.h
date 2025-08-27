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

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 3D関連の初期化
	/// </summary>
	void Init3D();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// リソースの破棄
	/// </summary>
	void Release();

	/// <summary>
	/// 先頭の（Updataが呼ばれる）シーンを切り替える
	/// </summary>
	/// <param name="scene">切り替え先のシーン</param>
	void CreateScene(std::shared_ptr<SceneBase> scene);

	/// <summary>
	/// すべてのシーンを切り替える
	/// </summary>
	/// <param name="scene">切り替え先のシーン</param>
	void ChangeAllScene(std::shared_ptr<SceneBase> scene);

	/// <summary>
	/// シーンをプッシュする。スタックの数が増える
	/// 一番上のシーンのUpdataしか呼ばれません。
	/// </summary>
	/// <param name="scene">積むシーン</param>
	void PushScene(std::shared_ptr<SceneBase> scene);

	/// <summary>
	/// スタックの頭のシーンを削除する。
	/// ただし、スタック上にシーンが一つしかない場合は、削除しない。
	/// </summary>
	void PopScene();

	/// <summary>
	/// シーン遷移
	/// </summary>
	/// <param name="nextId">変更先のシーン</param>
	void ChangeScene(SCENE_ID nextId);
	
	/// <summary>
	/// フェードを始める
	/// </summary>
	void StartFadeIn();

	/// <summary>
	/// 現在のシーンIDを返す
	/// </summary>
	/// <returns>現在のシーンID</returns>
	inline SCENE_ID GetSceneID() const { return sceneId_; }

	/// <summary>
	/// デルタタイムを返す
	/// </summary>
	/// <returns>デルタタイム</returns>
	inline float GetDeltaTime() const { return deltaTime_; }

	/// <summary>
	/// 経過時間を返す
	/// </summary>
	/// <returns>経過時間</returns>
	inline float GetTotalTime() const { return totalTime_; }

	/// <summary>
	/// カメラを返す
	/// </summary>
	/// <returns>カメラ</returns>
	std::weak_ptr<Camera> GetCamera() const { return camera_; }

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
