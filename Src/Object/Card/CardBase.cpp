#include<DxLib.h>
#include "../Utility/UtilityCommon.h"
#include "../Manager/Resource/ResourceManager.h"
#include "CardBase.h"

CardBase::CardBase(const CARD_STATUS _status) :
	status_(_status),
	isWin_(false),
	isUsed_(false),
	atkCardImg_(UtilityCommon::INITIAL_HANDLE),
	cardPos_(),
	cardScl_(CARD_SCALE),
	numPos_(),
	cardNoImg_(nullptr)
{
}

CardBase::~CardBase(void)
{
}

void CardBase::Load(void)
{
	ResourceManager& res = ResourceManager::GetInstance();
	cardNoImg_ = res.Load(ResourceManager::SRC::NUMBERS_IMGS).handleIds_;
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
