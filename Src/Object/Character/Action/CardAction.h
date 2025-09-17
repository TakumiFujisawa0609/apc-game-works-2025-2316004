#pragma once
#include "../Base/ActionBase.h"
class CardDeck;
class CardAction :
    public ActionBase
{

public:
    static constexpr float CARD_ACT_TIME_MAX = 1.0f; //カードアクション時間(デバッグ用)
    CardAction(ActionController& _actCntl,CardDeck& _deck);
    ~CardAction(void)override;
    void Init(void) override;
    void Update() override;

public:

    //カードデッキ
    CardDeck& deck_;
    //カードアクション中かどうか
    bool isCardAct_;
    //カードアクション時間(デバッグ用)
    float cardActTime_; 
};

