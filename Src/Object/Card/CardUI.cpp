#include <DxLib.h>
#include <cmath>
#include "../Utility/UtilityCommon.h"
#include"../Manager/Generic/InputManager.h"
#include"../Manager/Generic/SceneManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "CardUI.h"

CardUI::CardUI(void):
atkCardImg_(-1),
cardPos_({ 100,100 }),
cardScl_(1.0),
radius_({RADIUS_X,RADIUS_Y}),
cardMoveCnt_(SELECT_MOVE_CARD_TIME)
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
	int visible = 6;
	float startAngle = 0;
	float endAngle = UtilityCommon::Deg2RadF(135.0f);
	//float endAngle = UtilityCommon::Deg2RadF(90.0f);
	float angleOff = UtilityCommon::Deg2RadF(22.5f);

	changeMoveState_.emplace(CARD_SELECT::NONE, [this]() {ChangeNone(); });
	changeMoveState_.emplace(CARD_SELECT::LEFT, [this]() {ChangeLeft(); });
	changeMoveState_.emplace(CARD_SELECT::RIGHT, [this]() {ChangeRight(); });

	CardMoveSelect(CARD_SELECT::NONE);

	for (int i = 0; i < VISIBLE_CARD_MAX+1; i++)
	{
		uiInfos_[i].currentAngle = (END_ANGLE - START_ANGLE) * (visible - i) / visible - angleOff * 2;
		uiInfos_[i].cardPos.x = CENTER_X + std::cos(uiInfos_[i].currentAngle) * radius_.x;
		uiInfos_[i].cardPos.y = CENTER_Y + std::sin(uiInfos_[i].currentAngle) * (-radius_.y);
		//常に強さ番号座標をローカル座標分追従させる
		uiInfos_[i].numPos = uiInfos_[i].cardPos + (NUM_LOCAL_POS * cardScl_);

		int size = static_cast<int>(visibleCards_.size());
		if (size < VISIBLE_CARD_MAX)
		{
			visibleCards_.emplace_back(uiInfos_[i]);
		}
		//リロードカードを入れる
		//else if (i == visible - 1)
		//{
		//	visibleCards_.emplace_front(uiInfos_[i]);
		//}

	}
	visibleEndCardIndex_ = static_cast<int>(visibleCards_.size()) - 1;
}

void CardUI::Update(void)
{
	//カード状態
	cardUpdate_();
}

void CardUI::Draw(void)
{
	constexpr double NUM_SCL = 0.18;

	auto visibleCards = visibleCards_;

	//描画の時のみ逆に回す
	visibleCards.reverse();
	for (auto& card : visibleCards)
	{

		DrawRotaGraphF(card.cardPos.x, card.cardPos.y, cardScl_, 0.0f, card.typeImg, true);

		int num = card.status.pow_ - 1;
		if (num < 0)num = 9;

		DrawRotaGraphF(card.numPos.x, card.numPos.y, cardScl_ * NUM_SCL, 0.0f, cardNoImgs_[num], true);
		DrawLine(CENTER_X, CENTER_Y, card.cardPos.x, card.cardPos.y, 0xFFFFFF);

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

void CardUI::CardMoveSelect(const CARD_SELECT _select)
{
	if (cardMoveCnt_ < SELECT_MOVE_CARD_TIME)return;
	selectState_ = _select;
	changeMoveState_[selectState_]();
}

void CardUI::ChangeNone(void)
{
	cardMoveCnt_ = SELECT_MOVE_CARD_TIME;
	//目標角度を現在の角度にする
	for (auto& card : visibleCards_)
	{
		card.goalAngle = 0.0f;
	}
	cardUpdate_ = [this]() {UpdateNone(); };
}

void CardUI::ChangeLeft(void)
{
	cardMoveCnt_ = SELECT_MOVE_CARD_TIME;
	visibleEndCardIndex_++;
	if(visibleEndCardIndex_>=static_cast<int>(uiInfos_.size())-1)
	{
		visibleEndCardIndex_ = 0;
	}
	visibleCards_.emplace_back(uiInfos_[visibleEndCardIndex_]);
	for (auto& card : visibleCards_)
	{
		card.goalAngle = card.currentAngle+UtilityCommon::Deg2RadF(VISIBLE_ANGLE_OFFSET);
	}
	cardUpdate_ = [this]() {UpdateLeft(); };
}

void CardUI::ChangeRight(void)
{
	cardMoveCnt_ = SELECT_MOVE_CARD_TIME;
	visibleStartCardIndex_--;
	if (visibleStartCardIndex_ < 0)
	{
		visibleStartCardIndex_ = static_cast<int>(uiInfos_.size()) - 1;
	}
	visibleCards_.emplace_front(uiInfos_[visibleStartCardIndex_]);
	for (auto& card : visibleCards_)
	{
		card.goalAngle = card.currentAngle - UtilityCommon::Deg2RadF(VISIBLE_ANGLE_OFFSET);
	}
	cardUpdate_ = [this]() {UpdateRight(); };
}

void CardUI::UpdateNone(void)
{
	int i = 0;
	int visible = 7;

	//何もしない
	for(auto& card:visibleCards_)
	{
		card.currentAngle = (END_ANGLE - START_ANGLE) * (visible - i) / visible - VISIBLE_ANGLE_OFFSET*2;
		card.cardPos.x = CENTER_X + std::cos(card.currentAngle) * radius_.x;
		card.cardPos.y = CENTER_Y + std::sin(card.currentAngle) * (-radius_.y);
		//常に強さ番号座標をローカル座標分追従させる
		card.numPos = card.cardPos + (NUM_LOCAL_POS * cardScl_);
		i++;
	}
}

void CardUI::UpdateLeft(void)
{
	cardMoveCnt_ -= 1.0f / 60.0f;
	if (cardMoveCnt_ < 0.0f)
	{
		visibleCards_.pop_front();
		changeMoveState_[CARD_SELECT::NONE]();
		return;
	}
	for (auto& card : visibleCards_)
	{
		float time = (SELECT_MOVE_CARD_TIME - cardMoveCnt_) / SELECT_MOVE_CARD_TIME;
		float startDeg = UtilityCommon::Rad2DegF(card.currentAngle);
		float goalDeg = UtilityCommon::Rad2DegF(card.goalAngle);
		card.currentAngle = UtilityCommon::Deg2RadF(UtilityCommon::LerpDeg(startDeg
			, goalDeg, time));

		card.cardPos.x = CENTER_X + std::cos(card.currentAngle) * radius_.x;
		card.cardPos.y = CENTER_Y + std::sin(card.currentAngle) * (-radius_.y);

		//常に強さ番号座標をローカル座標分追従させる
		card.numPos = card.cardPos + (NUM_LOCAL_POS * cardScl_);
	}
}

void CardUI::UpdateRight(void)
{
	cardMoveCnt_ -= 1.0f / 60.0f;
	if (cardMoveCnt_ < 0.0f)
	{
		visibleCards_.pop_back();
		changeMoveState_[CARD_SELECT::NONE]();
		return;
	}
	for (auto& card : visibleCards_)
	{
		float time = (SELECT_MOVE_CARD_TIME - cardMoveCnt_) / SELECT_MOVE_CARD_TIME;
		float startDeg = UtilityCommon::Rad2DegF(card.currentAngle);
		float goalDeg = UtilityCommon::Rad2DegF(card.goalAngle);
		card.currentAngle = UtilityCommon::Deg2RadF(UtilityCommon::LerpDeg(startDeg
			, goalDeg, time));

		card.cardPos.x = CENTER_X + std::cos(card.currentAngle) * radius_.x;
		card.cardPos.y = CENTER_Y + std::sin(card.currentAngle) * (-radius_.y);

		//常に強さ番号座標をローカル座標分追従させる
		card.numPos = card.cardPos + (NUM_LOCAL_POS * cardScl_);
	}
}
