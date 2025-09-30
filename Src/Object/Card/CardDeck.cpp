#include<DxLib.h>
#include"../../Application.h"
#include "CardBase.h"
#include"./CardSystem.h"
#include"../Utility/UtilityTemplates.h"

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
	drawPile_.clear();
	hand_.clear();
	chargeCard_.clear();
	disCard_.clear();
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

void CardDeck::CardUseUpdate(void)
{
	//////場にカードをだせない状態なら処理を抜ける
	//bool isCanput = CardSystem::GetInstance().GetCanPut();
	//if (!isCanput)return;

	CardSystem& cardSystem = CardSystem::GetInstance();
	//カード同士を比べる
	cardSystem.CompareCards();
}

void CardDeck::CardCharge(void)
{
	int i = 0;
}

void CardDeck::DisCard(void)
{
	for (auto& hand : hand_)
	{
		disCard_.emplace_back(std::move(hand));
	}

	//使ったカードの配列を消す
	UtilityTemplates::EraseVectorArray(hand_);

	//カードを捨てるときに勝敗判定のカードの強さを初期化する
	CardSystem::GetInstance().InitPutCardPow(playerNum_);
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

	//手札の表示
	int handSize = hand_.size();
	if (handSize > 0)
	{
		for (int i = 0; i < handSize; i++)
		{
			int handCardPow = hand_[i]->GetPow();
			DrawFormatString(centerPos_.x + (DISTANCE_X * i), centerPos_.y + 100, 0xffffff, L"(%d)", handCardPow);
		}
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

void CardDeck::MoveHandToCharge(void)
{
	//山札にあるカードを手札に加える
	hand_.emplace_back(std::move(drawPile_[currentNum_]));

	//山札からカードを削除する
	UtilityTemplates::EraseVectorArray(drawPile_);
	if (currentNum_ == 0)
	{
		prevNum_ = static_cast<int>(drawPile_.size()) - 1;
	}
	else if (currentNum_ == static_cast<int>(drawPile_.size()) - 1)
	{
		currentNum_ = 0;
	}
	//drawPile_.resize(drawPile_.size() - 1);

	//現在選んでいるカードを次のカードにする
	//currentNum_++;
	//nextNum_ = currentNum_ + 1;
	//システム側の処理
	DrawCardFromDeck();
}

void CardDeck::DrawCardFromDeck(void)
{
	//使うカードを手札に加える
	int cardPow = 0;

	//手札の合計値を出す
	for (auto& it : hand_)
	{
		cardPow += it->GetPow();
	}

	//カードを場に出してシステム側で処理をする
	CardSystem& cardSystem = CardSystem::GetInstance();

	//先出し後だしの判定
	cardSystem.JudgeIsFirstAtk(playerNum_);

	cardSystem.PutCard(cardPow, playerNum_);
}


//std::vector<std::weak_ptr<CardBase>> CardDeck::GetHand(void)
//{
//	std::vector<std::weak_ptr<CardBase>> hand;
//	for (int i = 0; i < hand_.size(); i++)
//	{
//		hand.emplace_back(hand_[i]);
//	}
//	return hand;
//}

std::vector<CardBase::CARD_TYPE> CardDeck::GetCardType(void)
{
	std::vector<CardBase::CARD_TYPE>handCardTypes;
	for (auto& h : hand_)
	{
		CardBase::CARD_TYPE type = h->GetCardType();
		handCardTypes.emplace_back(type);
	}
	return handCardTypes;
}

void CardDeck::CardMoveLimit(void)
{
	int cardMax = static_cast<int>(drawPile_.size());
	if (currentNum_ < 0)
	{
		currentNum_ = cardMax - 1;
	}
	else if (currentNum_ >= cardMax)
	{
		currentNum_ = 0;
	}
	if (nextNum_ > cardMax - 1)
	{
		nextNum_ = 0;
	}
	if (prevNum_ < 0)
	{
		prevNum_ = cardMax - 1;
	}
}

bool CardDeck::IsCardFailure(void)
{
	CardSystem::BATTLE_RESULT result = CardSystem::GetInstance().GetResult(playerNum_);
	using RESULT = CardSystem::BATTLE_RESULT;
	return result == RESULT::FAILURE_USE_BE_REFLECTED || result == RESULT::BE_DRAW
		|| result == RESULT::GIVE_DRAW;
}
