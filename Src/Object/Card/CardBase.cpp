#include<DxLib.h>
#include "../Manager/Resource/ResourceManager.h"
#include "CardBase.h"

CardBase::CardBase(const CARD_STATUS _status) :
	status_(_status),
	isWin_(false),
	isUsed_(false),
	atkCardImg_(-1),
	cardPos_({ 100,100 }),
	cardScl_(1.4),
	numPos_({0.0f,0.0f})
{
	//複数画像はコンストラクタで初期化必須
	int i = -1;
	cardNoImg_ = &i;
	////とりあえず攻撃カードを生成
	//type_ = CARD_TYPE::ATTACK;
}

CardBase::~CardBase(void)
{
	int i = 0;
}

void CardBase::Load(void)
{
	ResourceManager& res = ResourceManager::GetInstance();
	cardNoImg_ = res.Load(ResourceManager::SRC::NUMBERS_IMG).handleIds_;
	atkCardImg_ = res.Load(ResourceManager::SRC::PLAYER_ATK_CARD_IMG).handleId_;
}

void CardBase::Init(void)
{
}

void CardBase::Update(void)
{

}

void CardBase::Draw(void)
{
	numPos_ = cardPos_ + (NUM_LOCAL_POS*cardScl_);
	constexpr double NUM_SCL = 0.18;
	DrawRotaGraphF(cardPos_.x, cardPos_.y, cardScl_, 0.0f, atkCardImg_, true);
	DrawRotaGraphF(numPos_.x, numPos_.y, cardScl_* NUM_SCL, 0.0f, cardNoImg_[status_.pow_], true);
}
