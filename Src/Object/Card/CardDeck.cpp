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
	initDeck_.clear();
}

void CardDeck::Load(void)
{
	drawPile_[0]->Load();
}

void CardDeck::Init(void)
{
	//カードの最後の配列にリロードカードを入れる(リロード用のカードで、勝敗はない)
	//CardBase::CARD_STATUS reloadStatus = { -1, CardBase::CARD_TYPE::RELOAD };
	//std::unique_ptr<CardBase>roloadCard = std::make_unique<CardBase>(reloadStatus);
	//std::unique_ptr<CardBase>initReloadCard = std::make_unique<CardBase>(reloadStatus);
	//drawPile_.emplace_back(std::move(roloadCard));
	//initDeck_.emplace_back(std::move(initReloadCard));

	//カードUIの選択番号が1番なので1に初期化する
	currentNum_ = 0;
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

void CardDeck::EraseHandCard(const bool _isLose)
{
	hand_.clear();

	//使ったカードの配列を消す
	UtilityTemplates::EraseVectorArray(hand_);

	if (_isLose == true)
	{
		//カードに負けたときの強さを初期化する
		CardSystem::GetInstance().LoseInitPutCardPow(playerNum_);
	}
	else
	{
		//カードを捨てるときに勝敗判定のカードの強さを初期化する
		CardSystem::GetInstance().InitPutCardPow(playerNum_);
	}

}

void CardDeck::CardMoveRight(void)
{
	//指令はプレイヤーやエネミーから出す
	currentNum_--;
	nextNum_ = currentNum_ + 1;
	prevNum_ = currentNum_ - 1;
	//カード制限
	CardMoveLimit();
}

void CardDeck::CardMoveLeft(void)
{
	//指令はプレイヤーやエネミーから出す
	currentNum_++;
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
	CardBase::CARD_STATUS currentCardPow = drawPile_[currentNum_]->GetCardStatus();
	//CardBase::CARD_STATUS nextCardPow = drawPile_[nextNum_]->GetCardStatus();
	//CardBase::CARD_STATUS prevCardPow = drawPile_[prevNum_]->GetCardStatus();

	const float DISTANCE_X = 40;

	//DrawFormatString(centerPos_.x-DISTANCE_X, centerPos_.y, 0xffffff,L"(%d)", prevCardPow.pow_);
	DrawFormatString(centerPos_.x, centerPos_.y, 0x000000,L"(%d)", currentCardPow);
	//DrawFormatString(centerPos_.x + DISTANCE_X, centerPos_.y, 0xffffff,L"(%d)", nextCardPow);

	//drawPile_[0]->DrawPlayerUI();


	//手札の表示
	int handSize = static_cast<int>(hand_.size());
	if (handSize > 0)
	{
		for (int i = 0; i < handSize; i++)
		{
			int handCardPow = hand_[i]->GetCardStatus().pow_;
			DrawFormatString(centerPos_.x + (DISTANCE_X * i), centerPos_.y + 100, 0x000000, L"(%d)", handCardPow);
		}
	}
}

void CardDeck::Release(void)
{

}

void CardDeck::AddDrawPile(const CardBase::CARD_STATUS& _status)
{
	std::unique_ptr<CardBase>card = std::make_unique<CardBase>(_status);
	std::unique_ptr<CardBase>initCard = std::make_unique<CardBase>(_status);
	drawPile_.emplace_back(std::move(card)); 
	initDeck_.emplace_back(std::move(initCard));
	
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
		nextNum_ = currentNum_ + 1;
	}
	else if (currentNum_ > static_cast<int>(drawPile_.size()) - 1)
	{
		currentNum_ = 0;
		prevNum_= currentNum_-1;
		nextNum_ = 0;
	}
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
		cardPow += it->GetCardStatus().pow_;
	}

	//カードを場に出してシステム側で処理をする
	CardSystem& cardSystem = CardSystem::GetInstance();

	//先出し後だしの判定
	cardSystem.JudgeIsFirstAtk(playerNum_);

	//カードを場に出す
	cardSystem.PutCard(cardPow, playerNum_);
}

std::vector<CardBase::CARD_TYPE> CardDeck::GetHandCardType(void)
{
	std::vector<CardBase::CARD_TYPE>handCardTypes;
	for (auto& h : hand_)
	{
		CardBase::CARD_TYPE type = h->GetCardStatus().type_;
		handCardTypes.emplace_back(type);
	}
	return handCardTypes;
}

const CardBase::CARD_TYPE CardDeck::GetDrawCardType(void)
{
	return drawPile_[currentNum_]->GetCardStatus().type_;
}

void CardDeck::Reload(void)
{
	drawPile_.clear();
	for (auto& deck : initDeck_)
	{
		CardBase* newCard = new CardBase(*deck);
		std::unique_ptr<CardBase>deckPtr(newCard);
		drawPile_.emplace_back(std::move(deckPtr));
	}
	//それぞれの番号の初期化
	currentNum_ = 0;
}

void CardDeck::CardMoveLimit(void)
{
	int cardMax = static_cast<int>(drawPile_.size());
	if (currentNum_ < 0)
	{
		currentNum_ = cardMax - 1;
	}
	else if (currentNum_ > cardMax - 1)
	{
		currentNum_ = 0;
	}
}

bool CardDeck::IsCardFailure(void)
{
	CardSystem::BATTLE_RESULT result = CardSystem::GetInstance().GetResult(playerNum_);
	using RESULT = CardSystem::BATTLE_RESULT;
	return result == RESULT::FAILURE_USE_BE_REFLECTED || result == RESULT::BE_DRAW
		|| result == RESULT::GIVE_DRAW||result== RESULT::NONE;
}
