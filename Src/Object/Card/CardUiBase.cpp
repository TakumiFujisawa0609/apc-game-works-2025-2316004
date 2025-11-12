#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include"../Manager/Generic/InputManager.h"
#include"../Manager/Generic/SceneManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "CardUIBase.h"

CardUIBase::CardUIBase(void):
selectState_(CARD_SELECT::RELOAD),
atkCardImg_(-1),
reloadCardImg_(-1)
{
}

CardUIBase::~CardUIBase(void)
{
}

void CardUIBase::ChangeSelectState(const CARD_SELECT _select)
{
	if (selectState_ == _select)return;
	selectState_ = _select;
	changeMoveState_[selectState_]();
}

void CardUIBase::CardDisition(void)
{

}
void CardUIBase::ChangeUsedActionCard(void)
{
	for (auto& act : actions_)
	{
		if (act.state == CARD_STATE::REACT)continue;
		if (act.disitionCnt_ > 0.0f)continue;
		act.state = CARD_STATE::USED;
	}
}

void CardUIBase::ChangeReactActionCard(void)
{
	for (auto& act : actions_)
	{
		act.state = CARD_STATE::REACT;
		act.reactCnt_ = REACT_MOVE_CARD_TIME;
	}
}

void CardUIBase::AddCardUi(const CardBase::CARD_STATUS _status)
{
	CARD_UI_INFO info = {};

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

void CardUIBase::DisitionMoveCardAll(void)
{
	//disitionCnt_ -= SceneManager::GetInstance().GetDeltaTime();
	//選択したカードの情報を取得
	for (auto& card : actions_)
	{
		if (card.state == CARD_STATE::USED||card.state==CARD_STATE::REACT)continue;
		card.state = CARD_STATE::USING;
		DisitionMoveSpecificCard(card);
	}
}

void CardUIBase::DisitionMoveSpecificCard(CARD_UI_INFO& _card)
{
	_card.disitionCnt_ -= DELTA;
	_card.cardPos = UtilityCommon::Lerp(_card.cardPos, DISITON_CARD_POS,
		(DISITION_MOVE_CARD_TIME - _card.disitionCnt_) / DISITION_MOVE_CARD_TIME);
}

void CardUIBase::UpdateUsedCard(void)
{
	if (actions_.empty())return;
	for (auto& act : actions_)
	{
		if (act.state != CARD_STATE::USED)continue;
		act.cardScl = UtilityCommon::Lerp(act.cardScl, 0.0, (SCL_LERP_TIME - act.sclCnt) / SCL_LERP_TIME);
		act.sclCnt -= static_cast<double>(DELTA);
	}

	//消去アニメーションが終わったカードはアクション配列から削除
	std::erase_if(actions_, [](auto& act) {return act.sclCnt < 0.0f; });
}

void CardUIBase::ReactMoveCard(const Vector2F& _goalPos)
{
	//選択したカードの情報を取得
	for (auto& card : actions_)
	{
		if (card.state != CARD_STATE::REACT)continue;
		//まだ決定移動中ならそちらを優先
		if (card.disitionCnt_ > 0.0f)
		{
			DisitionMoveSpecificCard(card);
			continue;
		}
		//弾かれ移動
		ReactMoveSpecificCard(card, _goalPos);
		if(card.reactCnt_<=0.0f)
		{
			card.state = CARD_STATE::USED;
			card.sclCnt = SCL_LERP_TIME;
		}
	}
	
}

void CardUIBase::ReactMoveSpecificCard(CARD_UI_INFO& _card, const Vector2F& _goalPos)
{
	//弾かれ移動
	_card.cardPos = UtilityCommon::Lerp(_card.cardPos, _goalPos,
		(REACT_MOVE_CARD_TIME - _card.reactCnt_) / REACT_MOVE_CARD_TIME);
	_card.reactCnt_ -= DELTA;
}

void CardUIBase::AddHandCurrent(void)
{
	handCurrent_++;
	if (handCurrent_ == handCards_.end())
	{
		handCurrent_ = handCards_.begin();
	}
}

void CardUIBase::SubHandCurrent(void)
{
	if (handCurrent_ == handCards_.begin())
	{
		handCurrent_ = handCards_.end();
	}
	handCurrent_--;
}

void CardUIBase::DrawCard(const CARD_UI_INFO _card)
{
	constexpr double NUM_SCL = 0.18;
	//カードの描画
	DrawRotaGraphF(_card.cardPos.x, _card.cardPos.y, _card.cardScl, 0.0f, _card.typeImg, true);

	int num = _card.status.pow_ - 1;
	if (num == -1) { num = 9; }
	DrawRotaGraphF(_card.numPos.x, _card.numPos.y, _card.cardScl * NUM_SCL, 0.0f, cardNoImgs_[num], true);
	//DrawLine(CENTER_X, CENTER_Y, _card.cardPos.x, _card.cardPos.y, 0xFFFFFF);
}