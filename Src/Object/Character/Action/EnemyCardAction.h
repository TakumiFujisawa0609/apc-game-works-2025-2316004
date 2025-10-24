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
    EnemyCardAction(CharacterBase& _charaObj, ActionController& _actCntl, CardDeck& _deck);
    ~EnemyCardAction(void)override;
    void Init(void) override;
    void Update() override;

private:

    //攻撃1段目判定
    static constexpr float ATTACK_ONE_COL_START_ANIM_CNT = 35.0f;   //攻撃当たり判定開始アニメーションカウント
    static constexpr float ATTACK_ONE_COL_END_ANIM_CNT = 46.0f;     //攻撃当たり判定終了アニメーションカウント
    ////攻撃2段目判定
    //static constexpr float ATTACK_TWO_COL_START_ANIM_CNT = 23.0f;   //攻撃当たり判定開始アニメーションカウント
    //static constexpr float ATTACK_TWO_COL_END_ANIM_CNT = 35.0f;     //攻撃当たり判定終了アニメーションカウント
    ////攻撃3段目判定
    //static constexpr float ATTACK_THREE_COL_START_ANIM_CNT = 19.0f;   //攻撃当たり判定開始アニメーションカウント
    //static constexpr float ATTACK_THREE_COL_END_ANIM_CNT = 28.0f;     //攻撃当たり判定終了アニメーションカウント

    //攻撃ローカル座標
    static constexpr VECTOR ATK_ONE_LOCAL = { 0.0f,0.0f,50.0f };

    static constexpr float ATK_SPHERE_RADIUS = 120.0f;						//カプセル球の半径
    //1段目攻撃のステータス
    static constexpr CardActionBase::ATK_STATUS SWIP_ATK = { ATTACK_ONE_COL_START_ANIM_CNT,ATTACK_ONE_COL_END_ANIM_CNT,ATK_SPHERE_RADIUS };
    ////2段目攻撃のステータス
    //static constexpr CardActionBase::ATK_STATUS NORMAL_ATK_TWO = { ATTACK_TWO_COL_START_ANIM_CNT,ATTACK_TWO_COL_END_ANIM_CNT,ATK_SPHERE_RADIUS };
    ////3段目攻撃のステータス
    //static constexpr CardActionBase::ATK_STATUS NORMAL_ATK_THREE = { ATTACK_THREE_COL_START_ANIM_CNT,ATTACK_THREE_COL_END_ANIM_CNT,ATK_SPHERE_RADIUS };

    //遷移
    void ChangeSwip(void);
    void ChangeRoar(void);
    void ChangeJumpAtk(void);
    void ChangeReload(void);

    //更新
    void UpdateSwip(void);
    void UpdateRoar(void);
    void JumpAtkUpdate(void);
    void UpdateReload(void);

    //攻撃別の当たり判定情報
    std::map<ACT_TYPE, ATK_STATUS>atkTable_;
};

