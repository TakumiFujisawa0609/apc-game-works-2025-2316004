#pragma once
#include "CardUIBase.h"
class EnemyCardUI :
    public CardUIBase
{
    public:
    // コンストラクタ
    EnemyCardUI(void);
    // デストラクタ
    ~EnemyCardUI(void)override;
    /// @brief ロード
    /// @param  
    void Load(void) override;
    /// @brief 初期化
    /// @param  
    void Init(void) override;
    /// @brief 更新
    /// @param  
    void Update(void) override;
    /// @brief 描画
    /// @param
	void Draw(void) override;
private:
    //選択カード初期位置
	static constexpr Vector2F SELECT_CARD_START_POS = { Application::SCREEN_SIZE_X - 200,Application::SCREEN_HALF_Y + 200 };
    //弾かれた後のゴール座標(敵)
    static constexpr Vector2F REACT_GOAL_CARD_POS = { Application::SCREEN_SIZE_X + 200.0f, Application::SCREEN_HALF_Y + 500 };
    //カード状態遷移
    void ChangeNone(void) override;		//通常
    //void ChangeLeft(void) override;		//左に移動
    //void ChangeRight(void) override;		//右に移動
    void ChangeDecision(void) override;	//決定
    void ChangeReloadWait(void) override;
    void UpdateNone(void) override;
    //void UpdateLeft(void) override;
    //void UpdateRight(void) override;
    void UpdateDecision(void) override;
	void UpdateReloadWait(void) override;

    /// @brief カードUI初期化
	/// @param
	void InitCardUI(void)override;
};

