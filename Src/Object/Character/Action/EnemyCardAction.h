#pragma once
#include<functional>
#include<queue>
#include "../Base/ActionBase.h"

class CardDeck;
class CharacterBase;

class EnemyCardAction :
    public ActionBase
{
public:
    EnemyCardAction(CharacterBase& _charaObj, ActionController& _actCntl, CardDeck& _deck);
    ~EnemyCardAction(void)override;
    void Init(void) override;
    void Update() override;
private:
};

