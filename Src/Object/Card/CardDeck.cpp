#include<DxLib.h>
#include"../../Application.h"
#include "CardBase.h"

#include"./CardSystem.h"

#include "CardDeck.h"

CardDeck::CardDeck(Vector2& _centerPos, int _playerNum):
	drawPile_(),
	currentNum_(0),
	nextNum_(0),
	prevNum_(0),
	centerPos_(_centerPos),
	playerNum_(_playerNum)
{
	currentNum_ = 0;
	nextNum_ = 0;
	prevNum_ = 0;
}

CardDeck::~CardDeck(void)
{
}

void CardDeck::Init(void)
{
	//for (int i = 0; i < CARD_NUM_MAX; i++)
	//{
	//	std::unique_ptr<CardBase> card = std::make_unique<CardBase>(CARD_POWS[i]);
	//	drawPile_.emplace_back(std::move(card));
	//}
	currentNum_ = 0;
	nextNum_ = currentNum_ + 1;
	prevNum_ = CARD_NUM_MAX - 1;

	Vector2 pos = centerPos_;
}

void CardDeck::CardUse(void)
{
	//場にカードを出星南い状態なら処理を抜ける
	if (CardSystem::GetInstance().GetCanPut() == false)return;

	//使うカードを手札に加える
	int cardPow = 0;
	hand_.emplace_back(std::move(drawPile_[currentNum_]));

	drawPile_.resize(drawPile_.size() - 1);
	//現在選んでいるカードを次のカードにする
	currentNum_++;
	nextNum_ = currentNum_ + 1;

	//手札の合計値を出す
	for(auto& it:hand_)
	{
		cardPow += it->GetPow();
	}

	//カードを場に出してシステム側で処理をする
	CardSystem::GetInstance().PutCard(cardPow);
	
}

void CardDeck::CardCharge(void)
{
	int i = 0;
}

void CardDeck::DisCard(void)
{
}

void CardDeck::CardMoveRight(void)
{
	//指令はプレイヤーやエネミーから出す
	currentNum_++;
	nextNum_ = currentNum_ + 1;
	prevNum_ = currentNum_ - 1;
	//カード制限
	CardMoveLimit();
}

void CardDeck::CardMoveLeft(void)
{
	//指令はプレイヤーやエネミーから出す
	currentNum_--;
	nextNum_ = currentNum_ + 1;
	prevNum_ = currentNum_ - 1;
	//カード制限
	CardMoveLimit();
}

void CardDeck::Update(void)
{
}

void CardDeck::Draw(void)
{
	int currentCardPow = drawPile_[currentNum_]->GetPow();
	int nextCardPow = drawPile_[nextNum_]->GetPow();
	int prevCardPow = drawPile_[prevNum_]->GetPow();

	const float DISTANCE_X = 40;

	DrawFormatString(centerPos_.x-DISTANCE_X, centerPos_.y, 0xffffff,L"(%d)", prevCardPow);
	DrawFormatString(centerPos_.x, centerPos_.y, 0xffffff,L"(%d)", currentCardPow);
	DrawFormatString(centerPos_.x + DISTANCE_X, centerPos_.y, 0xffffff,L"(%d)", nextCardPow);
	for(int i=0;i<hand_.size();i++)
	{
		int handCardPow = hand_[i]->GetPow();
		DrawFormatString(centerPos_.x + (DISTANCE_X*i), centerPos_.y+100, 0xffffff,L"(%d)", handCardPow);
	}
}

void CardDeck::Release(void)
{
}

void CardDeck::AddDrawPile(const int _pow)
{
	std::unique_ptr<CardBase>card = std::make_unique<CardBase>(_pow);
	drawPile_.emplace_back(std::move(card));
}

std::vector<std::weak_ptr<CardBase>> CardDeck::GetHand(void)
{
	std::vector<std::weak_ptr<CardBase>> hand;
	for (int i = 0; i < hand_.size(); i++)
	{
		hand.emplace_back(hand_[i]);
	}
	return hand;
}

void CardDeck::CardMoveLimit(void)
{
	if (currentNum_ < 0)
	{
		currentNum_ = CARD_NUM_MAX - 1;
	}
	else if (currentNum_ >= CARD_NUM_MAX)
	{
		currentNum_ = 0;
	}
	if (nextNum_ > CARD_NUM_MAX - 1)
	{
		nextNum_ = 0;
	}
	if (prevNum_ < 0)
	{
		prevNum_ = CARD_NUM_MAX - 1;
	}
}
