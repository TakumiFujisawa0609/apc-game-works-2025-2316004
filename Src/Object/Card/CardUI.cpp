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
	reloadCardImg_ = res.Load(ResourceManager::SRC::RELOAD_CARD_IMG).handleId_;

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

	ChangeSelectState(CARD_SELECT::NONE);

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
		////リロードカードを入れる
		//else if (i == VISIBLE_CARD_MAX - 1)
		//{
		//	visibleCards_.emplace_front(uiInfos_[i]);
		//}

	}
	CARD_UI_INFO info = {};
	info.status.pow_ = -1;
	info.status.type_ = CardBase::CARD_TYPE::RELOAD;
	info.typeImg = reloadCardImg_;
	uiInfos_.emplace_back(info);
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
	case CardBase::CARD_TYPE::RELOAD:
		info.typeImg = reloadCardImg_;
	}

	//呼び出し元の配列からの情報を代入する
	info.status = _status;

	//配列に挿入
	uiInfos_.emplace_back(info);
}

void CardUI::ChangeSelectState(const CARD_SELECT _select)
{
	if (cardMoveCnt_ < SELECT_MOVE_CARD_TIME)return;
	selectState_ = _select;
	changeMoveState_[selectState_]();
}

void CardUI::CardDisition(void)
{

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
	visibleStartCardIndex_++;
	if(visibleEndCardIndex_>=static_cast<int>(uiInfos_.size()))
	{
		visibleEndCardIndex_ = visibleEndCardIndex_ - uiInfos_.size();
	}
	if (visibleStartCardIndex_ >= static_cast<int>(uiInfos_.size()))
	{
		visibleStartCardIndex_ = visibleStartCardIndex_ - uiInfos_.size();
	}
	
	visibleCards_.emplace_back(uiInfos_[visibleEndCardIndex_]);
	for (auto& card : visibleCards_)
	{
		card.goalAngle = card.currentAngle + UtilityCommon::Deg2RadF(VISIBLE_ANGLE_OFFSET);
	}
	cardUpdate_ = [this]() {UpdateLeft(); };
}

void CardUI::ChangeRight(void)
{
	cardMoveCnt_ = SELECT_MOVE_CARD_TIME;
	//描画するカードのインデックスをずらす
	visibleStartCardIndex_--;
	visibleEndCardIndex_--;
	//負の数になったら配列の最後に戻す
	if (visibleStartCardIndex_ < 0)
	{
		visibleStartCardIndex_ = static_cast<int>(uiInfos_.size()) + visibleStartCardIndex_;
	}
	if (visibleEndCardIndex_ < 0)
	{
		visibleEndCardIndex_ = static_cast<int>(uiInfos_.size()) + visibleEndCardIndex_;
	}
	//先頭に追加
	visibleCards_.emplace_front(uiInfos_[visibleStartCardIndex_]);
	//目標角度をずらす
	for (auto& card : visibleCards_)
	{
		card.goalAngle = card.currentAngle - UtilityCommon::Deg2RadF(VISIBLE_ANGLE_OFFSET);
	}
	cardUpdate_ = [this]() {UpdateRight(); };
}

void CardUI::ChangeDisition(void)
{
}

void CardUI::UpdateNone(void)
{
	//配列ごとに座標を追従させる
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
		//visibleStartCardIndex_++;
		changeMoveState_[CARD_SELECT::NONE]();
		return;
	}
	MoveCard();
}

void CardUI::UpdateRight(void)
{
	cardMoveCnt_ -= 1.0f / 60.0f;
	if (cardMoveCnt_ < 0.0f)
	{
		visibleCards_.pop_back();
		//visibleEndCardIndex_--;
		changeMoveState_[CARD_SELECT::NONE]();
		return;
	}
	MoveCard();
}

void CardUI::UpdateDisition(void)
{
	int i = 0;
	disitionCnt_ += 1.0f / 60.0f;
	int cardSize = static_cast<int>(visibleCards_.size());
	for (auto& card : visibleCards_)
	{
		if (i == SELECT_CARD_NO)
		{
			//選択したカードの情報を取得
			card.cardPos = UtilityCommon::Lerp(card.cardPos, Vector2F{ Application::SCREEN_HALF_X,Application::SCREEN_HALF_Y }, 0.1f);
			i++;
			continue;
		}
		i++;
	}
}

void CardUI::MoveCard(void)
{
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
