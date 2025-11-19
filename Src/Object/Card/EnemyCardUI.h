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

#ifdef _DEBUG
	void DrawDebug(void);
#endif // _DEBUG

private:

    //弾かれた後のゴール座標(敵)
    static constexpr Vector2F REACT_GOAL_CARD_POS = { Application::SCREEN_SIZE_X + 200.0f, Application::SCREEN_HALF_Y + 500 };
    //カード状態遷移
    void ChangeNone(void) override;		//通常
    void ChangeDecision(void) override;	//決定
	void ChangeReloadWait(void) override;//リロード待機

	//カード更新関数
	void UpdateNone(void) override;//通常
	void UpdateDecision(void) override;//決定
	void UpdateReloadWait(void) override;//リロード待機

    /// @brief カードUI初期化
	/// @param
	void InitCardUI(void)override;
};

