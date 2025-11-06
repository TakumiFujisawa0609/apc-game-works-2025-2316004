#include "../pch.h"
#include"../Player/ActionController.h"
#include"../../Common/AnimationController.h"
#include"../Base/CharacterBase.h"
#include"../Utility/Utility3D.h"
#include"../Base/LogicBase.h"
#include "Dodge.h"

Dodge::Dodge(ActionController& _actCntl,Transform& _trans ,float& _spd):
	ActionBase(_actCntl),
	trans_(_trans),
	dodgeSpd_(_spd)
{
	speed_ = dodgeSpd_+ ADD_DODGE_SPEED;
}

Dodge::~Dodge(void)
{
}

void Dodge::Init(void)
{
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::DODGE),false);
}

void Dodge::Update()
{
	if (anim_.GetAnimStep()> END_DODGE_ANIM_STEP)
	{
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		return;
	}
	//回避中はInputクラスへプレイヤーの前情報をセットする
	actionCntl_.GetInput().SetMoveDirTransformFront(trans_);
}
