#pragma once
#include<vector>
#include<memory>
class CardDeck;
class CardSystem
{
public:
	CardSystem(std::weak_ptr<CardDeck>_deck1, std::weak_ptr<CardDeck>_deck2);
	~CardSystem(void);

	//ストックカードのカードを比較する
	void CompareCards(void);
private:
	//手札
	std::weak_ptr<CardDeck>deck1_;
	std::weak_ptr<CardDeck>deck2_;
};

