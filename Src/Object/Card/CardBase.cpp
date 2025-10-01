#include "CardBase.h"

CardBase::CardBase(const CARD_STATUS _status):
	status_(_status),
	isWin_(false)
{
	////とりあえず攻撃カードを生成
	//type_ = CARD_TYPE::ATTACK;
}

CardBase::~CardBase(void)
{
}

void CardBase::Execute(void)
{
	//デバッグ用
	int i = 0;
}
