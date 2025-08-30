
#include "CardBase.h"
#include"CardDeck.h"
#include "CardSystem.h"

CardSystem::CardSystem(void):
	putCardPow_{-1,-1},
	canPut_(true)
{
	
}


void CardSystem::CompareCards(void)
{
	if (putCardPow_[PLAYER_NO] == -1 || putCardPow_[CPU_NO] == -1)
	{
		canPut_ = true;
	}
	else
	{
		canPut_ = false;
	}
	//お互いに手札が0枚なら処理を抜ける
	if (putCardPow_[PLAYER_NO]==-1 && putCardPow_[CPU_NO] ==-1)
	{
		return;
	}
	//以下、2つのカードの強さを比較
	if (putCardPow_[PLAYER_NO] == putCardPow_[CPU_NO])
	{
		//引き分け
	}
	else if (putCardPow_[PLAYER_NO] > putCardPow_[CPU_NO])
	{
		//プレイヤーの勝ち
	}
	else if (putCardPow_[PLAYER_NO] < putCardPow_[CPU_NO])
	{
		//CPUの勝ち
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
			putCardPow_[i] = pow;
			break;
		}
	}
}

