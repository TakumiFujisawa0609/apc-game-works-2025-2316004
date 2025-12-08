#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include"../Manager/Generic/InputManager.h"
#include"../Manager/Generic/SceneManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Common/Easing.h"
#include "../Card/CardUIDraw.h"
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
	//renderer_->MakeSquareVertex(Vector2{},GetGraphSize(ResourceManager::GetInstance().Load(ResourceManager::SRC::))
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
		act->ChangeUsedCard();
	}
}

void CardUIBase::ChangeReactActionCard(void)
{
	for (auto& act : actions_)
	{
		act->ChangeReactCard();
	}
}

void CardUIBase::ChangeUsingActionCard(void)
{
	for (auto& act : actions_)
	{
		act->ChangeUsing();
	}
}

void CardUIBase::Load(void)
{

}

void CardUIBase::MakeObject(void)
{
	//easing_ = std::unique_ptr<Easing>();
}

void CardUIBase::Init(void)
{
	
}

void CardUIBase::Update(void)
{
	//カード状態
	cardUpdate_();

	//見えている部分だけ更新
	UpdateDrawCardUI();

	//使用済みカードの大きさ補完
	UpdateUsedCard();
}

void CardUIBase::Draw(void)
{
	for (auto& card : actions_)
	{
		//DrawCard(card);
		card->Draw();
	}
}




void CardUIBase::AddCardUi(const CardBase::CARD_STATUS _status)
{
	CardBase::CARD_STATUS status = {};
	int typeImg = -1;
	//属性によって画像を変える
	typeImg = GetTypeImg(_status);
	int num = _status.pow_ - 1;
	if (num == -1) { num = 9; }
	int drawNumImg = cardNoImg_[num];
	std::shared_ptr<CardUIController> info = std::make_shared<CardUIController>(drawNumImg);
	info->SetTypeImg_(typeImg);
	info->SetStatus(_status);
	info->Load();
	info->Init();

	//配列に挿入
	initialCards_.emplace_back(info);
}

void CardUIBase::DecisionMoveCardAll(void)
{
	//disitionCnt_ -= SceneManager::GetInstance().GetDeltaTime();
	//選択したカードの情報を取得
	for (auto& card : actions_)
	{
		if (card->GetState() == CardUIController::CARD_STATE::REACT)continue;
		//card->ChangeUsing();

		card->DecisionMove();
		//DecisionMoveSpecificCard(card);
	}

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
	}
	
}

//void CardUIBase::ReactMoveSpecificCard(CARD_UI_INFO& _card, const Vector2F& _goalPos)
//{
//	//弾かれ移動
//	_card.cardPos_ = UtilityCommon::Lerp(_card.cardPos_, _goalPos,
//		(CardUIController::REACT_MOVE_CARD_TIME - _card.reactCnt_) / CardUIController::REACT_MOVE_CARD_TIME);
//	_card.reactCnt_ -= DELTA;
//}

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



const int CardUIBase::GetTypeImg(const CardBase::CARD_STATUS _status)
{
	int typeImg = -1;
	switch (_status.type_)
	{
	case CardBase::CARD_TYPE::ATTACK:
		typeImg = atkCardImg_;
		break;
	case CardBase::CARD_TYPE::RELOAD:
		typeImg = reloadCardImg_;
	}

	return typeImg;
}

void CardUIBase::SetBasePosActionCards(void)
{
	for (auto& card : actions_)
	{
		card->SetBaseCardPos();
	}
}

