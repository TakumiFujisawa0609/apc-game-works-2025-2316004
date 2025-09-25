#include"../Manager/Generic/SceneManager.h"
#include"../Player/ActionController.h"
#include"../../Common/AnimationController.h"
#include"../../Card/CardDeck.h"
#include"../../Card/CardBase.h"
#include"../../Card/CardSystem.h"
#include"../Base/CharacterBase.h"
#include"../Base/InputBase.h"
#include "CardAction.h"

CardAction::CardAction(ActionController& _actCntl, CardDeck& _deck):
	ActionBase(_actCntl),
	deck_(_deck)
{
	isCardAct_ = false;
	cardActTime_ = 0.0f;
}

CardAction::~CardAction(void)
{
}

void CardAction::Init(void)
{
	//手札に移動
	deck_.MoveHandToCharge();
	//アクション中にする
	isCardAct_ = true;
	//カードの属性を受け取ってアニメーションを再生
	std::vector<CardBase::CARD_TYPE>cardTypes = deck_.GetCardType();
	int handCardTypeSize = deck_.GetCardType().size();
	if (handCardTypeSize == 1&&cardTypes[0]==CardBase::CARD_TYPE::ATTACK)
	{
		anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK),false);
	}
}

void CardAction::Update()
{
	deck_.CardUseUpdate();
	//デバッグ用
	cardActTime_ += scnMng_.GetDeltaTime();

	if (deck_.IsCardFailure()||anim_.IsEnd())
	{
		//アクション終了
		isCardAct_ = false;
		cardActTime_ = 0.0f;
		deck_.DisCard();
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		return;
	}
}
