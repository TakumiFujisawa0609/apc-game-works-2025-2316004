
#include "CardBase.h"
#include"CardDeck.h"
#include "CardSystem.h"

CardSystem::CardSystem(std::weak_ptr<CardDeck>_deck1, std::weak_ptr<CardDeck>_deck2):
	deck1_(_deck1),
	deck2_(_deck2)
{
}

CardSystem::~CardSystem(void)
{
}

void CardSystem::CompareCards(void)
{
	int hand1Size = deck1_.lock()->GetHand().size();
	int hand1CardPow = -1;
	int hand2CardPow = -1;
	int hand2Size = hand2_.size();
	
	//お互いに手札が0枚なら処理を抜ける
	if (hand1Size == 0 && hand2Size == 0)
	{
		return;
	}
	//手札が0枚以上ならカードの強さを0に初期化する
	if (hand1Size > 0)
	{
		hand1CardPow = 0;
	}
	if (hand2Size > 0)
	{
		hand2CardPow = 0;
	}
	//複数ある手札のカードの強さを合計する
	for(int i=0;i< hand1_.size(); i++)
	{
		hand1CardPow += hand1_[i].GetPow();
	}
	for(int i=0;i< hand2_.size(); i++)
	{
		hand2CardPow += hand2_[i].GetPow();
	}
	//強さを比較し、勝った方のカードに勝利フラグを立てる
	if (hand1CardPow > hand2CardPow)
	{
		for(int i=0;i< hand1_.size(); i++)
		{
			hand1_[i].SetIsWin(true);
		}
	}
	else if (hand2CardPow < hand1CardPow)
	{
		for (int i = 0; i < hand1_.size(); i++)
		{
			hand2_[i].SetIsWin(true);
		}
	}
}

