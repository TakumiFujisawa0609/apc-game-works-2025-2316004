#include "../Base/ActionBase.h"
#include"../Base/LogicBase.h"
#include"../Utility/Utility3D.h"
#include"../Player/ActionController.h"
#include"../Base/CharacterBase.h"
#include"../../Common/AnimationController.h"
#include "Run.h"

Run::Run(ActionController& _actCntl):
	ActionBase(_actCntl)
{
	speed_ = MOVE_SPEED;
}

Run::~Run(void)
{
}

void Run::Init(void)
{
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::RUN));
}

void Run::Update(void)
{
	//入力方向が0だった場合、アイドル状態移行
	if (Utility3D::EqualsVZero(actionCntl_.GetInput().GetDir()))
	{
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		return;
	}

	//カードを場に出す処理
	if (actionCntl_.GetInput().GetIsAct().isCardUse)
	{
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::CARD_ACTION);
		return;
	}

	//ジャンプ入力があった場合、ジャンプ状態へ移行
	if (actionCntl_.GetInput().GetIsAct().isJump)
	{
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::JUMP);
		return;
	}
}
