
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
	int hand2Size = deck2_.lock()->GetHand().size();
	
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
	for(int i=0;i< hand1Size; i++)
	{
		hand1CardPow += deck1_.lock()->GetHand()[i].lock()->GetPow();
	}
	for(int i=0;i< hand2Size; i++)
	{
		hand2CardPow += deck2_.lock()->GetHand()[i].lock()->GetPow();
	}
	//強さを比較し、勝った方のカードに勝利フラグを立てる
	if (hand1CardPow > hand2CardPow)
	{
		for(int i=0;i< hand1Size; i++)
		{
			deck1_.lock()->GetHand()[i].lock()->SetIsWin(true);
		}
	}
	else if (hand2CardPow < hand1CardPow)
	{
		for (int i = 0; i < hand2Size; i++)
		{
			deck2_.lock()->GetHand()[i].lock()->SetIsWin(true);
		}
	}
}

