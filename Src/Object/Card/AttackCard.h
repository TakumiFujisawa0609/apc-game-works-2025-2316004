#pragma once
#include "CardBase.h"
class AttackCard :
    public CardBase
{
public:
	//コンストラクタ
	AttackCard(const CardBase::CARD_STATUS _status);
	//デストラクタ
	~AttackCard(void)override;

	void Execute(void)override;
private:

};

