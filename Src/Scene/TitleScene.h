#pragma once
#include "SceneBase.h"


class TitleScene : public SceneBase
{

public:
	enum class TITLE_STATE
	{
		START_GAME,
		TUTORIAL,
		EXIT_MENU,
		MENU,
		EXIT,
		MAX
	};

	enum class TITLE_BTN
	{
		START_GAME,
		TUTORIAL,
		EXIT,
		MAX
	};

	enum class YES_NO
	{
		YES,
		NO
	};

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void);

	//読み込み処理
	void Load(void) override;

	//初期化処理
	void Init(void) override;

private:
	//メニュー始まりの座標
	static constexpr int BUTTON_START_POS_X = 700;
	static constexpr int BUTTON_START_POS_Y = 100;

	//メニュー座標の間隔
	static constexpr int BUTTON_DISTANCE = 100;

	//////ゲーム終了確認メニューの始め座標
	//static constexpr int CHECK_EXIT_MENU_START_X = 100;
	//static constexpr int CHECK_EXIT_MENU_START_Y = 100;

	//ゲーム終了確認メニューの大きさ
	static constexpr int CHECK_EXIT_MENU_SIZE_X = 600;
	static constexpr int CHECK_EXIT_MENU_SIZE_Y = 200;

	//終了するかの文字の確認メニューとの間隔
	static constexpr int QUESTION_OFFSET = 30;
	
	//はい、いいえの文字間隔
	static constexpr int YES_NO_DISTANCE = 100;
	

	//各選択肢の更新
	std::function<void(void)>updateTitle_;
	std::unordered_map<TITLE_STATE, std::function<void(void)>> titleSelectFuncTable_;
	//文字列格納
	std::unordered_map<TITLE_BTN, std::wstring>buttonStrTable_;
	//YES,NOの文字列
	std::unordered_map<YES_NO, std::wstring>yesNoStrTable_;
	//セレクト中のボタン
	TITLE_BTN selectBtn_;
	TITLE_STATE selectState_;
	//タイトル画像
	int titleImg_;

	//フォント
	int titleFont_;

	//現在選んでいるボタン
	int selectNum_;

	//はいの選択肢を選んでいるか
	YES_NO yesNoState_;
	//状態遷移
	void ChangeState(const TITLE_STATE& _state);

	//更新関数
	void NormalUpdate(void) override;

	//描画関数
	void NormalDraw(void) override;

	//処理の変更
	void ChangeNormal(void) override;

	//メニュー処理
	void UpdateMenu(void);

	//チュートリアル
	void UpdateTutorial(void);

	//選択処理
	void UpdateSelectGame(void);

	//ゲーム終了確認メニュー
	void UpdateExitMenu(void);
};
