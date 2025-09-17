#include"../Manager/Generic/SceneManager.h"
#include"../Player/ActionController.h"
#include"../../Card/CardDeck.h"
#include"../../Card/CardSystem.h"
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
}

void CardAction::Update()
{
	deck_.CardUseUpdate();
	cardActTime_ += scnMng_.GetDeltaTime();
	if (deck_.IsCardFailure()||cardActTime_>= CARD_ACT_TIME_MAX)
	{
		//アクション終了
		isCardAct_ = false;
		cardActTime_ = 0.0f;
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		return;
	}

	
}
