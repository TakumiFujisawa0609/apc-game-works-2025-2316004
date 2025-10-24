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
        ROAR_ATK,     //咆哮
        JUMP_ATK, //攻撃アクション3回目

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

    CardActionBase(CharacterBase& _charaObj, ActionController& _actCntl, CardDeck& _deck);
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
    void AttackMotion(const ATK_STATUS& _status,const VECTOR& _localPos);
    //カードの勝敗を常に監視する
    bool IsCardFailure(void);

    //コンボアクション遷移(あれば実装する)
    virtual void ChangeComboAction(void);
private:

};

