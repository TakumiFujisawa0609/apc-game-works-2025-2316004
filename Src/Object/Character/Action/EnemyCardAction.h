#pragma once
#include<functional>
#include<queue>
#include "../Base/ActionBase.h"
#include "../Base/CardActionBase.h"

class CardDeck;
class CharacterBase;

class EnemyCardAction :
    public CardActionBase
{

public:

    enum class ACT_TYPE
    {
        NONE = -1,
        SWIP_ATK, //ひっかき攻撃
        ROAR,     //咆哮
        JUMP_ATK, //攻撃アクション3回目
        RELOAD, //リロード
    };
    EnemyCardAction(ActionController& _actCntl, CharacterBase& _charaObj, CardDeck& _deck);
    ~EnemyCardAction(void)override;

    /// @brief 初期化
    /// @param  
    void Init(void) override;

    /// @brief 更新
    /// @param  
    void Update(void) override;

    /// @brief 解放
    /// @param  
    void Release(void)override;
private:

    //攻撃1段目判定
    static constexpr float ATTACK_ONE_COL_START_ANIM_CNT = 35.0f;   //攻撃当たり判定開始アニメーションカウント
    static constexpr float ATTACK_ONE_COL_END_ANIM_CNT = 46.0f;     //攻撃当たり判定終了アニメーションカウント

    //重力
    static constexpr float GRAVITY = 0.1f;
    //ジャンプ高さ
    static constexpr float JUMP_HEIGHT = 100.0f;
    //ジャンプ攻撃アニメーション
    static constexpr float JUMP_ANIM_START = 16.4f;
    static constexpr float JUMP_ANIM_END = 64.1f;
    ////攻撃2段目判定
    static constexpr float ATTACK_TWO_COL_START_ANIM_CNT = 63.0f;   //攻撃当たり判定開始アニメーションカウント
    static constexpr float ATTACK_TWO_COL_END_ANIM_CNT = 65.0f;     //攻撃当たり判定終了アニメーションカウント
    //攻撃半径広がるスピード
    static constexpr float JUMP_ATK_COL_SPD = 5.0f;
    //広がる時間
    static constexpr float JUMP_ATK_CNT_MAX = 1.0f;
    //攻撃3段目判定
    static constexpr float ATTACK_THREE_COL_START_ANIM_CNT = 19.0f;   //攻撃当たり判定開始アニメーションカウント
    static constexpr float ATTACK_THREE_COL_END_ANIM_CNT = 28.0f;     //攻撃当たり判定終了アニメーションカウント

    //攻撃ローカル座標
    static constexpr VECTOR ATK_ONE_LOCAL = { 0.0f,0.0f,50.0f };

    static constexpr float ATK_SPHERE_RADIUS = 50.0f;						//カプセル球の半径
    static constexpr float JUMP_ATK_RADIUS = 30.0f;						//カプセル球の半径
    //1段目攻撃のステータス
    static constexpr CardActionBase::ATK_STATUS SWIP_ATK = { ATTACK_ONE_COL_START_ANIM_CNT,ATTACK_ONE_COL_END_ANIM_CNT,ATK_SPHERE_RADIUS };
    ////2段目攻撃のステータス
    static constexpr CardActionBase::ATK_STATUS JUMP_ATK = { ATTACK_TWO_COL_START_ANIM_CNT,ATTACK_TWO_COL_END_ANIM_CNT,JUMP_ATK_RADIUS };
    //3段目攻撃のステータス
    static constexpr CardActionBase::ATK_STATUS ROAR_ATK = { ATTACK_THREE_COL_START_ANIM_CNT,ATTACK_THREE_COL_END_ANIM_CNT,ATK_SPHERE_RADIUS };

    //ジャンプ攻撃カウント
    float jumpAtkCnt_;

    //遷移
    void ChangeSwip(void);
    void ChangeRoar(void);
    void ChangeJumpAtk(void);
    void ChangeReload(void);

    //更新
    void UpdateSwip(void);
    void UpdateRoar(void);
    void UpdateJumpAtk(void);
    void UpdateReload(void);

    //攻撃別の当たり判定情報
    std::map<ACT_TYPE, ATK_STATUS>atkTable_;
};

