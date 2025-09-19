#include"../Player/ActionController.h"
#include"../Utility/Utility3D.h"
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
	//移動
	if (!Utility3D::EqualsVZero(actionCntl_.GetInput().GetDir()))
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