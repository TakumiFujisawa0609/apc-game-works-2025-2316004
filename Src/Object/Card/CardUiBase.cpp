#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include"../Manager/Generic/InputManager.h"
#include"../Manager/Generic/SceneManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Card/CardUI.h"
#include "../Card/CardUIController.h"
#include "../Renderer/PixelMaterial.h"
#include "../Renderer/PixelRenderer.h"
#include "CardUIBase.h"

CardUIBase::CardUIBase(void):
selectState_(CARD_SELECT::RELOAD_WAIT),
atkCardImg_(-1),
reloadCardImg_(-1)
{
	//material_ = std::make_unique<PixelMaterial>(L"CardUI",0);
	//renderer_ = std::make_unique<PixelRenderer>(*material_);
	//renderer_->MakeSquereVertex(Vector2{},GetGraphSize(ResourceManager::GetInstance().Load(ResourceManager::SRC::))
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

void CardUIBase::CardDecision(void)
{

}
void CardUIBase::ChangeUsedActionCard(void)
{
	for (auto& act : actions_)
	{

	}
}

void CardUIBase::ChangeReactActionCard(void)
{
	for (auto& act : actions_)
	{
		//act.state_ = CARD_STATE::REACT;
		//act.reactCnt_ = REACT_MOVE_CARD_TIME;
	}
}

void CardUIBase::AddCardUi(const CardBase::CARD_STATUS _status)
{
	CardBase::CARD_STATUS status = {};
	int typeImg = -1;
	//属性によって画像を変える
	switch (_status.type_)
	{
	case CardBase::CARD_TYPE::ATTACK:
		typeImg = atkCardImg_;
		break;
	case CardBase::CARD_TYPE::RELOAD:
		typeImg = reloadCardImg_;
	}

	std::shared_ptr<CardUIController> info = std::make_shared<CardUIController>(cardNoImgs_);
	info->SetTypeImg_(typeImg);
	info->SetStatus(_status);

	//配列に挿入
	uiInfos_.emplace_back(info);
}

void CardUIBase::DecisionMoveCardAll(void)
{
	//disitionCnt_ -= SceneManager::GetInstance().GetDeltaTime();
	//選択したカードの情報を取得
	for (auto& card : actions_)
	{
		card->ChangeUsing();
		card->DecisionMove();
		//DecisionMoveSpecificCard(card);
	}
}

void CardUIBase::DecisionMoveSpecificCard(CARD_UI_INFO& _card)
{
	_card.disitionCnt_ -= DELTA;
	_card.cardPos_ = UtilityCommon::Lerp(_card.cardPos_, DISITON_CARD_POS,
		(DISITION_MOVE_CARD_TIME - _card.disitionCnt_) / DISITION_MOVE_CARD_TIME);
}

void CardUIBase::UpdateUsedCard(void)
{
	if (actions_.empty())return;
	for (auto& act : actions_)
	{
		act->EraseUsedCard();
	}

	//消去アニメーションが終わったカードはアクション配列から削除
	std::erase_if(actions_, [](auto& act) {return act->GetSclCnt() < 0.0f; });
}

void CardUIBase::ReactMoveCard(const Vector2F& _goalPos)
{
	//選択したカードの情報を取得
	for (auto& card : actions_)
	{
		card->ReactUpdate(_goalPos);
		//if (card.state_ != CARD_STATE::REACT)continue;
		////まだ決定移動中ならそちらを優先
		//if (card.disitionCnt_ > 0.0f)
		//{
		//	DecisionMoveSpecificCard(card);
		//	continue;
		//}
		////弾かれ移動
		//ReactMoveSpecificCard(card, _goalPos);
		//if(reactCnt_<=0.0f)
		//{
		//	state_ = CARD_STATE::USED;
		//	card.sclCnt = SCL_LERP_TIME;
		//}
	}
	
}

void CardUIBase::ReactMoveSpecificCard(CARD_UI_INFO& _card, const Vector2F& _goalPos)
{
	//弾かれ移動
	_card.cardPos_ = UtilityCommon::Lerp(_card.cardPos_, _goalPos,
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
	DrawRotaGraphF(_card.cardPos_.x, _card.cardPos_.y, _card.cardScl_, 0.0f, _card.typeImg, true);

	int num = _card.status.pow_ - 1;
	if (num == -1) { num = 9; }
	DrawRotaGraphF(_card.numPos_.x, _card.numPos_.y, _card.cardScl_ * NUM_SCL, 0.0f, cardNoImgs_[num], true);
	//DrawLine(CENTER_X, CENTER_Y, _card.cardPos.x, _card.cardPos.y, 0xFFFFFF);
}