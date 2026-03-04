#pragma once
#include "../Common/Vector2.h"
#include "../Common/Easing.h"

class Easing;
class MenuController
{
public:

	struct BTN_INFO
	{
		std::wstring btnStr;				//ボタンの文字
		//TITLE_BTN btnType;				//何のボタンか
		Vector2 startPos;					//イージング前の座標
		Vector2 curPos = startPos;			//現在座標
		float easeCnt=0.0f;					//イージング時間
		bool isEase = false;				//イージング中か
		bool isEndDirectEase = false;		//演出イージングが終わったか
	};

	/// @brief コンストラクタ
	/// @param  
	MenuController(void);

	/// @brief デストラクタ
	/// @param  
	~MenuController(void);

	/// @brief フォントのロード
	/// @param
	void LoadFont(const std::wstring _fontType,const int _size);

	/// @brief メニューの追加
	/// @param  
	void AddMenu(const int _arrayNum,const std::wstring _menu,const Vector2 _pos);

	/// @brief メニュー操作
	/// @param  
	void Update(void);

	/// @brief サイズイージングのフォントハンドルの取得(サイズを変えるためにはフォントを作り直す必要があるため)
	/// @param _arrayNum 配列番号
	/// @param _startSize1 初期サイズ　
	/// @param _goalSize1 最終的なサイズ
	/// @param _easeTime イージング時間
	/// @param _easeType イージング種類
	/// @return サイズ変更後のフォントハンドル
	const int GetSizeEasingFontHandle(const int _arrayNum,const int _startSize, const int _goalSize, const float _easeTime,Easing::EASING_TYPE _easeType);

	/// @brief メニュー演出の更新
	/// @param _disSpawn 出てくる間隔時間
	
	/// @brief メニュー演出の更新
	/// @param _disSpawn 出てくる間隔時間
	/// @param _easeTime イージング時間
	/// @param _goalPosX 最終的なX座標
	/// @return true:演出終了	false:演出中
	const bool DirectionMenu(const float _disSpawn,const float _easeTime,const int _goalPosX);

	/// @brief メニューの描画
	/// @param  
	void Draw(void);

	/// @brief 選択中のメニュー番号の加算
	/// @param  
	void AddSelectMenuNum(void);
	/// @brief 選択中のメニュー番号の加算
	/// @param  
	void SubSelectMenuNum(void);

	/// @brief 選択中のメニュー番号の取得
	/// @param  
	/// @return 
	const int GetSelectMenuNum(void) { return selectMenuNum_; }

private:

	//デフォルトのフォントサイズ
	int defaultFontSize_;

	//フォントハンドル
	std::wstring defaultFontHandle_;

	//イージング
	std::unique_ptr<Easing>easing_;

	//メニューのフォントハンドル
	int fontHandle_;

	//演出でメニューが出てくるときの間隔カウント
	float disSpawnCnt_;

	//メニュー表
	std::unordered_map<int, BTN_INFO> menuList_;

	//選択中のメニュー
	int selectMenuNum_;
};

