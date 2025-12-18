#pragma once
#include<queue>
#include<map>
#include "ActionBase.h"

class CardDeck;
class CharacterBase;
class EffectController;
class CardActionBase :
    public ActionBase
{

public:


   // //攻撃のステータス
   //struct ATK_STATUS
   // {
   //     float colStartCnt;
   //     float colEndCnt;
   //     float atkRadius;
   //     bool isDamage=false;
   // };

    CardActionBase(ActionController& _actCntl, CharacterBase& _charaObj, CardDeck& _deck);
    ~CardActionBase(void)override;
    void Init(void) override;
    void Update() override;

    /// @brief ジャンプチャージ攻撃中
    /// @param  
    /// @return 
    virtual const bool IsJumpAtkCharge(void)const override { return false; }

    /// @brief 敵のジャンプチャージ中のカード勝負数
    /// @param  
    /// @return 
    virtual const int GetJumpCardNum(void)const override { return -1; }

protected:
    //ジャンプ溜め中のカード出した回数
    static constexpr int JAMP_CHARGE_CARD_NUM_MAX = 3;

    //カードデッキ
    CardDeck& deck_;
    //オブジェクト
    CharacterBase& charaObj_;
    //カードアクション関数ポインタ
    std::function<void(void)> cardActFunc_;
    //カードアクション遷移
    std::map<CARD_ACT_TYPE, std::function<void(void)>>changeAction_;
    //攻撃ステータステーブル
    std::map<CARD_ACT_TYPE, ATK_STATUS>atkStatusTable_;
    //アタックのQueue配列
    std::queue<std::function<void(void)>>cardFuncs_;

    //エフェクトコントローラ
    std::unique_ptr<EffectController>effect_;

    //デュエルモードでの待機中フラグ　true:待機中
    bool isDuelWait_;

    //攻撃座標
    VECTOR atkPos_;
    //状態遷移
    void ChangeCardAction(const CARD_ACT_TYPE& _type);
    //攻撃モーション
    void AttackMotion(const ATK_STATUS& _status, const Collider::TAG& _attackTag,const VECTOR& _localPos);
    /// @brief カードの勝敗を常に監視する。負けたらtrueを返し、のけぞり状態にする
    /// @param _attackTag 使っている当たり判定タグ
    /// @return 
    bool IsCardFailure(const Collider::TAG& _attackTag);
    //攻撃終了
    void FinishAttack(const Collider::TAG _attackCol);

	//負けて攻撃が終了
	void FinishFailureAttack(const Collider::TAG _attackCol);

    //カードを場に出す
    void PutCard(void);

    //デュエルモード
    virtual void ChangeDuel(void) = 0;              //デュエルモード遷移
    virtual void UpdateDuel(void) = 0;              //デュエルモード更新

    //移動量
    VECTOR velocity_;

    //コンボアクション遷移(あれば実装する)
    virtual void ChangeComboAction(void);
private:

};

