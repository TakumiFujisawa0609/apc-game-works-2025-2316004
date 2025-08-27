
#include "CardBase.h"
#include "CardSystem.h"

CardSystem::CardSystem(void)
{
}

CardSystem::~CardSystem(void)
{
}

bool CardSystem::CompareCards(CardBase& card1, CardBase& card2)
{
	int card1Pow = card1.GetPow();
	int card2Pow = card2.GetPow();
	if (card1Pow > card2Pow)
	{
		card1.SetIsWin(true);
	}
	else if (card1Pow < card2Pow)
	{
		card2.SetIsWin(true);
	}
		
	return false;
}

