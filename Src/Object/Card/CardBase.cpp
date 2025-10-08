#include<DxLib.h>
#include "../Manager/Resource/ResourceManager.h"
#include "CardBase.h"

CardBase::CardBase(const CARD_STATUS _status):
	status_(_status),
	isWin_(false),
	isUsed_(false),
	atkCardImg_(-1)
{
	//複数画像はコンストラクタで初期化必須
	int i = -1;
	cardNoImgs_ = &i;
	////とりあえず攻撃カードを生成
	//type_ = CARD_TYPE::ATTACK;
}

CardBase::~CardBase(void)
{
}

void CardBase::Load(void)
{
	ResourceManager& res = ResourceManager::GetInstance();
	cardNoImgs_ = res.Load(ResourceManager::SRC::NUMBERS_IMG).handleIds_;
	atkCardImg_ = res.Load(ResourceManager::SRC::ATK_CARD_IMG).handleId_;
}

void CardBase::Init(void)
{
}

void CardBase::Update(void)
{
}

void CardBase::Draw(void)
{
	DrawRotaGraph(200, 200, 1.0, 0.0f, atkCardImg_, true);
	DrawRotaGraph(200, 200, 1.0, 0.0f, cardNoImgs_[status_.pow_], true);
}
