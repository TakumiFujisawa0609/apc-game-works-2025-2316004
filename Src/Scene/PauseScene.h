#pragma once
#include <string>
#include <unordered_map>
#include "SceneBase.h"

class PauseScene : public SceneBase
{
public:

	enum class LIST
	{
		RESUME,  // 続ける
		TITLE,   // タイトルに戻る
		MAX
	};

	//リスト
	static constexpr int LIST_MAX = static_cast<int>(LIST::MAX);

	// コンストラクタ
	PauseScene(void);

	// デストラクタ
	~PauseScene(void) override;

	//読み込み処理
	void Load(void)override;

	//初期化処理
	void Init(void)override;

	//更新関数
	void NormalUpdate(void)override;

	//描画関数
	void NormalDraw(void)override;

private:
	
	//画面のアルファ値
	static constexpr int PAUSE_ALPHA = 128; 

	//フォントサイズ
	static constexpr int FONT_SIZE = 32;
	static constexpr int FONT_THICK = 3; // フォントの太さ

	int imgTest_;

	//ポーズフォント
	int pauseFont_; 	

	//ポーズリストの選択
	int selectIndex_;

	//選択リスト
	std::wstring pasueList_[LIST_MAX] =
	{
		L"つづける",
		L"タイトルへ戻る"
	};

	//リスト選択テーブル
	std::unordered_map<LIST, std::function<void()>> listFuncTable_;
};

