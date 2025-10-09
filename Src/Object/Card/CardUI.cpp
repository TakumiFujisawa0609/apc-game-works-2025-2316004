#include <DxLib.h>
#include <cmath>
#include "../Utility/UtilityCommon.h"
#include "../Manager/Resource/ResourceManager.h"
#include "CardUI.h"

CardUI::CardUI(void):
atkCardImg_(-1),
cardPos_({ 100,100 }),
cardScl_(1.4)
{
	int i = -1;
	//複数画像はコンストラクタで初期化必須
	cardNoImgs_ = &i;
}

CardUI::~CardUI(void)
{
}

void CardUI::Load(void)
{
	ResourceManager& res = ResourceManager::GetInstance();
	cardNoImgs_ = res.Load(ResourceManager::SRC::NUMBERS_IMG).handleIds_;
	atkCardImg_ = res.Load(ResourceManager::SRC::ATK_CARD_IMG).handleId_;
}

void CardUI::Init(void)
{
	int uiInfoSize = static_cast<int>(uiInfos_.size());
	for (int i = 0; i < uiInfoSize; i++)
	{
		uiInfos_[i].angle = DX_PI_F / (uiInfoSize - 2)*i;
		uiInfos_[i].cardPos.x = 500 + std::cos(uiInfos_[i].angle) * RADIUS;
		uiInfos_[i].cardPos.y = 200 + std::sin(uiInfos_[i].angle) * RADIUS;
		//常に強さ番号座標をローカル座標分追従させる
		uiInfos_[i].numPos = uiInfos_[i].cardPos + (NUM_LOCAL_POS * cardScl_);
	}
}

void CardUI::Update(void)
{
	//int uiInfoSize = static_cast<int>(uiInfos_.size());
	//for (int i = 0; i < uiInfoSize; i++)
	//{
	//	//常に強さ番号座標をローカル座標分追従させる
	//	uiInfos_[i].numPos = uiInfos_[i].cardPos + (NUM_LOCAL_POS * cardScl_);
	//	uiInfos_[i].angle = 20 * i;
	//	uiInfos_[i].cardPos.x = std::cos(uiInfos_[i].angle) * RADIUS;
	//	uiInfos_[i].cardPos.y = std::sin(uiInfos_[i].angle) * RADIUS;
	//}
}

void CardUI::Draw(void)
{
	constexpr double NUM_SCL = 0.18;
	for (auto& ui : uiInfos_)
	{
		DrawRotaGraphF(ui.cardPos.x, ui.cardPos.y, cardScl_, 0.0f, ui.typeImg, true);
		DrawRotaGraphF(ui.numPos.x, ui.numPos.y, cardScl_ * NUM_SCL, 0.0f, cardNoImgs_[ui.status.pow_], true);
	}
}

void CardUI::AddCardUi(const CardBase::CARD_STATUS _status)
{
	CARD_UI_INFO info={};

	//属性によって画像を変える
	switch (_status.type_)
	{
	case CardBase::CARD_TYPE::ATTACK:
		info.typeImg = atkCardImg_;
		break;
	}

	//呼び出し元の配列からの情報を代入する
	info.status = _status;

	//配列に挿入
	uiInfos_.emplace_back(info);
}
