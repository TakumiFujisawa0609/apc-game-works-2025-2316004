
#include "CardBase.h"
#include"CardDeck.h"
#include "CardSystem.h"


const CardSystem::BATTLE_RESULT CardSystem::GetResult(int _cardPlayerNo) const
{
	return BATTLE_RESULT();
}

CardSystem::CardSystem(void):
	putCardPow_{-1,-1},
	canPut_(true)
{
	
}

const CardSystem::BATTLE_RESULT CardSystem::GetResult(int _cardPlayerNo) const
{
	//配列数より大きい数字を指定されたら何も返さない
	if (_cardPlayerNo > ARRAY_NUM)return;

	return playerResult_[_cardPlayerNo];
}


void CardSystem::CompareCards(void)
{
	if (putCardPow_[FIRST_ATK] == -1 || putCardPow_[SECOND_ATK] == -1)
	{
		canPut_ = true;
	}
	else
	{
		canPut_ = false;
	}
	//お互いに手札が0枚なら処理を抜ける
	if (putCardPow_[FIRST_ATK]==-1 && putCardPow_[SECOND_ATK] ==-1)
	{
		return;
	}
	//以下、2つのカードの強さを比較
	//どちらかのカードが出されていなければ先出し勝利
	if (putCardPow_[SECOND_ATK] == -1)
	{
		result_[FIRST_ATK] = BATTLE_RESULT::SUCCESS_USE;
	}

	//先出しと後出しカードが同じ強さの時はドロー
	if (putCardPow_[FIRST_ATK] == putCardPow_[SECOND_ATK])
	{
		//引き分け
		result_[FIRST_ATK] = BATTLE_RESULT::BE_DRAW;
		result_[SECOND_ATK] = BATTLE_RESULT::GIVE_DRAW;
	}
	//先出しの勝ち
	else if (putCardPow_[FIRST_ATK] > putCardPow_[SECOND_ATK])
	{
		result_[FIRST_ATK] = BATTLE_RESULT::SUCCESS_CARD_BREAK;
		result_[SECOND_ATK] = BATTLE_RESULT::FAILURE_USE_BE_REFLECTED;
	}
	//後出しの勝ち
	else if (putCardPow_[FIRST_ATK] < putCardPow_[SECOND_ATK])
	{
		result_[FIRST_ATK] = BATTLE_RESULT::FAILURE_USE_BE_REFLECTED;
		result_[SECOND_ATK] = BATTLE_RESULT::SUCCESS_CARD_BREAK;
	}
}

void CardSystem::PutCard(const int pow)
{
	//playerNoは0か1のどちらかで、それ以外の数値が来た場合は処理を抜ける
	//カードの強さをセットする
	for(int i=0; i < ARRAY_NUM; i++)
	{
		if(putCardPow_[i] == -1)
		{
			playerResult_[]
			putCardPow_[i] = pow;
			break;
		}
	}
}

