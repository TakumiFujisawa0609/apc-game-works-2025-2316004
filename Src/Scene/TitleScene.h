#pragma once
#include<memory>
#include "../Common/Easing.h"
#include "SceneBase.h"

class Easing;
class TitleScene : public SceneBase
{

public:
	enum class TITLE_STATE
	{
		START_GAME,
		TUTORIAL,
		EXIT_MENU,
		EASE_MENU,
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

	struct BTN
	{
		std::wstring btnStr;
		TITLE_BTN btnType;		//何のボタンか
		Vector2F startPos;		//イージング前の座標
		Vector2F curPos=startPos;//現在座標
		float easeCnt;			//イージング時間
		bool isEase = false;	//イージング中か
	};

	//イージング時間
	static constexpr float BUTTON_EASING_TIME = 1.0f;

	//タイトルロゴイージング時間
	static constexpr float LOGO_EASING_TIME = 1.5f;

	//次のボタンのイージングまでの間隔時間
	static constexpr float EASING_DIS_TIME = 0.1f;

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
	static constexpr float BUTTON_START_POS_X = 700.0f;
	static constexpr float BUTTON_START_POS_Y = 100.0f;

	//メニュー座標の間隔
	static constexpr float BUTTON_DISTANCE = 100;

	//////ゲーム終了確認メニューの始め座標
	//static constexpr int CHECK_EXIT_MENU_START_X = 100;
	//static constexpr int CHECK_EXIT_MENU_START_Y = 100;

	//ロゴサイズ
	static constexpr float LOGO_SCALE = 0.5f;
	static constexpr float LOGO_SIZE_X = 986.0f * LOGO_SCALE;
	static constexpr float LOGO_SIZE_Y = 759.0f * LOGO_SCALE;

	//ロゴイージング初期座標
	static constexpr Vector2F START_POS = { -LOGO_SIZE_X,-LOGO_SIZE_Y };
	static constexpr Vector2F GOAL_POS = { 0.0f,0.0f };

	//ゲーム終了確認メニューの大きさ
	static constexpr int CHECK_EXIT_MENU_SIZE_X = 600;
	static constexpr int CHECK_EXIT_MENU_SIZE_Y = 200;

	//終了するかの文字の確認メニューとの間隔
	static constexpr int QUESTION_OFFSET = 30;
	
	//はい、いいえの文字間隔
	static constexpr int YES_NO_DISTANCE = 100;
	
	//イージング
	std::unique_ptr<Easing>easing_;

	//各選択肢の更新
	std::function<void(void)>updateTitle_;
	std::unordered_map<TITLE_STATE, std::function<void(void)>> titleSelectFuncTable_;
	//文字列格納
	std::unordered_map<TITLE_BTN, std::wstring>buttonStrTable_;
	//YES,NOの文字列
	std::unordered_map<YES_NO, std::wstring>yesNoStrTable_;

	//ボタン格納配列
	std::list<BTN>buttons_;

	//タイトルロゴ座標
	Vector2F logoPos_;

	//セレクト中のボタン
	TITLE_BTN selectBtn_;
	TITLE_STATE selectState_;

	//タイトル背景
	int imgTitleBack;
	//タイトルロゴ
	int imgTitleLogo;
	//フォント
	int titleFont_;
	//現在選んでいるボタン
	int selectNum_;

	//イージング開始の間隔カウント
	float easeDistanceCnt_;
	//タイトルイージングカウント
	float logoEaseCnt_;

	//はいの選択肢を選んでいるか
	YES_NO yesNoState_;

	//ボタンごとでイージングを変えてみる
	Easing::EASING_TYPE DecideEase(TITLE_BTN _btn);

	//状態遷移
	void ChangeState(const TITLE_STATE& _state);

	//更新関数
	void NormalUpdate(void) override;

	//描画関数
	void NormalDraw(void) override;

	//処理の変更
	void ChangeNormal(void) override;

	//初めのイージング処理
	void UpdateEase(void);

	//メニュー処理
	void UpdateMenu(void);

	//チュートリアル
	void UpdateTutorial(void);

	//選択処理
	void UpdateSelectGame(void);

	//ゲーム終了確認メニュー
	void UpdateExitMenu(void);
};
