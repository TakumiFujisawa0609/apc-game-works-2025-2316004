#include "CardBase.h"

CardBase::CardBase(const int _pow):
	pow_(_pow)
{
	//とりあえず攻撃カードを生成
	type_ = CARD_TYPE::ATTACK;
}

CardBase::~CardBase(void)
{
}

void CardBase::Execute(void)
{
	//デバッグ用
	int i = 0;
}
