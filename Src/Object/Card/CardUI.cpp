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
	int uiInfoSize = static_cast<int>(uiInfos_.size());
	int visible = 6;
	float startAngle = 0;
	float endAngle = UtilityCommon::Deg2RadF(135.0f);
	//float endAngle = UtilityCommon::Deg2RadF(90.0f);
	float angleOff = UtilityCommon::Deg2RadF(22.5f);

	changeMoveState_.emplace(CARD_SELECT::NONE, [this]() {ChangeNone(); });
	changeMoveState_.emplace(CARD_SELECT::LEFT, [this]() {ChangeLeft(); });
	changeMoveState_.emplace(CARD_SELECT::RIGHT, [this]() {ChangeRight(); });

	CardMoveSelect(CARD_SELECT::NONE);

	for (int i = 0; i < visible; i++)
	{
		uiInfos_[i].currentAngle = (endAngle - startAngle) * (visible - i) / visible - angleOff * 2;
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
}

void CardUI::Update(void)
{
	int uiInfoSize = static_cast<int>(uiInfos_.size());
	//int visible = 4;
	int visible = 6;
	float startAngle = 0;
	float endAngle = UtilityCommon::Deg2RadF(135.0f);
	//float endAngle = UtilityCommon::Deg2RadF(90.0f);

	float angleOff = UtilityCommon::Deg2RadF(22.5f);

	int size = static_cast<int>(visibleCards_.size());
	int i = 0;
	//for (auto& card : visibleCards_)
	//{
	//	//card.currentAngle = (endAngle - startAngle) * (size - i) / size + startAngle;
	//	card.currentAngle = (card.endAngle - startAngle) * angleOff;
	//	card.cardPos.x = CENTER_X + std::cos(uiInfos_[i].currentAngle) * radius_.x;
	//	card.cardPos.y = CENTER_Y + std::sin(uiInfos_[i].currentAngle) * (-radius_.y);
	//	//常に強さ番号座標をローカル座標分追従させる
	//	card.numPos = card.cardPos + (NUM_LOCAL_POS * cardScl_);

	//	//更新回数を回す
	//	i++;
	//} 

	//カード状態
	cardUpdate_();


	if (InputManager::GetInstance().IsNew(KEY_INPUT_UP))
	{
		radius_.y++;
	}
	if (InputManager::GetInstance().IsNew(KEY_INPUT_DOWN))
	{
		radius_.y--;
	}
	if (InputManager::GetInstance().IsNew(KEY_INPUT_LEFT))
	{
		radius_.x++;
	}
	if (InputManager::GetInstance().IsNew(KEY_INPUT_RIGHT))
	{
		radius_.x--;
	}
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
	select_ = _select;
	changeMoveState_[select_]();
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
	for (auto& card : visibleCards_)
	{
		card.goalAngle += card.currentAngle+UtilityCommon::Deg2RadF(VISIBLE_ANGLE_OFFSET);
	}
	cardUpdate_ = [this]() {UpdateLeft(); };
}

void CardUI::ChangeRight(void)
{
	cardMoveCnt_ = SELECT_MOVE_CARD_TIME;
	for (auto& card : visibleCards_)
	{
		card.goalAngle -= UtilityCommon::Deg2RadF(VISIBLE_ANGLE_OFFSET);
	}
	cardUpdate_ = [this]() {UpdateRight(); };
}

void CardUI::UpdateNone(void)
{
	//何もしない
}

void CardUI::UpdateLeft(void)
{
	cardMoveCnt_ -= 1.0f / 60.0f;
	if (cardMoveCnt_ < 0.0f)
	{
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
		//card.currentAngle = rad;
	}
}

void CardUI::UpdateRight(void)
{
	for (auto& card : visibleCards_)
	{
		double startDeg = static_cast<double>(UtilityCommon::Rad2DegF(card.currentAngle));
		double goalDeg = static_cast<double>(UtilityCommon::Rad2DegF(card.goalAngle));
		card.currentAngle = UtilityCommon::Deg2RadF(UtilityCommon::LerpDeg(startDeg, goalDeg
			, SELECT_MOVE_CARD_TIME - cardMoveCnt_ / SELECT_MOVE_CARD_TIME));
	}
	if (cardMoveCnt_ < 0.0f)
	{
		changeMoveState_[CARD_SELECT::NONE]();
		return;
	}
}
