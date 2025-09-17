#include"../Player/ActionController.h"
#include"../Base/InputBase.h"
#include "Idle.h"

Idle::Idle(ActionController& _actCntl):
	ActionBase(_actCntl)
{
}

Idle::~Idle(void)
{
}

void Idle::Init(void)
{
}

void Idle::Update(void)
{
	//共通処理の更新
	ActionBase::Update();
	//移動
	if (actionCntl_.GetInput().GetMoveDeg() >= 0.0f)
	{
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::MOVE);
		return;
	}
	if (actionCntl_.GetInput().GetIsAct().isCardUse)
	{
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::CARD_ACTION);
		return;
	}


	//何もしないが、待機モーションとかずっと止まってるときにアニメーションを変えたりしたい
}