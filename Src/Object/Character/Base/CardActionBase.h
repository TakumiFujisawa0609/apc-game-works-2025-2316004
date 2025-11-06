#pragma once
#include<queue>
#include<map>
#include "ActionBase.h"

class CardDeck;
class CharacterBase;

class CardActionBase :
    public ActionBase
{

public:
    //カードアクション種類
    enum class CARD_ACT_TYPE
    {
        NONE = -1,
        //プレイヤーのアクション
        ATTACK_ONE, //攻撃アクション1回目
        ATTACK_TWO, //攻撃アクション2回目
        ATTACK_THREE, //攻撃アクション3回目
        HEAL_CARD,       //ヒールカード使用
        MAGIC_CARD,        //マジックカード使用

        //敵のアクション
        SWIP_ATK, //ひっかき攻撃
        ROAR_ATK, //咆哮
        JUMP_ATK, //ジャンプ攻撃
		ROLE_ATK,  //ロール攻撃

        //共通
        RELOAD, //リロード
        NUM,
    };

    //攻撃のステータス
   struct ATK_STATUS
    {
        float colStartCnt;
        float colEndCnt;
        float atkRadius;
        bool isDamage=false;
    };

    CardActionBase(ActionController& _actCntl, CharacterBase& _charaObj, CardDeck& _deck);
    ~CardActionBase(void)override;
    void Init(void) override;
    void Update() override;

protected:
    //カードデッキ
    CardDeck& deck_;
    //オブジェクト
    CharacterBase& charaObj_;
    //状態
    CARD_ACT_TYPE actType_;
    //カードアクション関数ポインタ
    std::function<void(void)> cardActFunc_;
    //カードアクション遷移
    std::map<CARD_ACT_TYPE, std::function<void(void)>>changeAction_;
    //攻撃ステータステーブル
    std::map<CARD_ACT_TYPE, ATK_STATUS>atkStatusTable_;
    //アタックのQueue配列
    std::queue<std::function<void(void)>>cardFuncs_;

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

    //移動量
    VECTOR velocity_;

    //コンボアクション遷移(あれば実装する)
    virtual void ChangeComboAction(void);

	//カード失敗かどうかの監視更新
	void CardFailureUpdate((const Collider::TAG& _tag));
private:

};

