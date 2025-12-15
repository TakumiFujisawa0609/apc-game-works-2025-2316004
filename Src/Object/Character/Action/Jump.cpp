#include"../pch.h"
#include "../Base/ActionBase.h"
#include"../../../Utility/Utility3D.h"
#include"../../../Utility/UtilityCommon.h"
#include"../../../Manager/Generic/SceneManager.h"
#include"../../Common/AnimationController.h"
#include "../Base/CharacterBase.h"
#include"../Base/LogicBase.h"
#include"../Player/ActionController.h"
#include "Jump.h"

Jump::Jump(ActionController& _actCntl, CharacterBase& _charaObj, VECTOR& _jumpPow):
	ActionBase(_actCntl),
	charaObj_(_charaObj),
	jumpPow_(_jumpPow)
{
}

Jump::~Jump(void)
{
}

void Jump::Load(void)
{
}

void Jump::Init(void)
{
	velocity_.y = sqrtf(2.0f * GRAVITY * JUMP_HEIGHT);
	jumpPow_.y = velocity_.y;

	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::JUMP),true,25.0f,39.0f,false);
}

void Jump::Update(void)
{
	speed_ = 0.0f;
	//ˆÚ“®
	if (!Utility3D::EqualsVZero(actionCntl_.GetInput().GetDir()))
	{
		speed_ = MOVE_SPEED;
	}

	velocity_.y -= GRAVITY;
	jumpPow_.y = velocity_.y;
	anim_.SetEndLoop(36.0f, 39.0f, 3.0f);
	if (charaObj_.GetHitPoint().isDown)
	{
		jumpPow_.y = 0.0f;
		velocity_.y = 0.0f;
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
	}
}