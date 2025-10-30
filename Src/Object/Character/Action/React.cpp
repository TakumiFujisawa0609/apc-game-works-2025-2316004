#include "../Base/ActionBase.h"
#include"../Base/LogicBase.h"
#include"../Utility/Utility3D.h"
#include"../Player/ActionController.h"
#include"../Base/CharacterBase.h"
#include"../../Common/AnimationController.h"

#include "React.h"

React::React(ActionController& _actCntl):
	ActionBase(_actCntl)
{
	isTurnable_ = false;
}

React::~React(void)
{
}

void React::Init(void)
{
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::REACT), false);
}

void React::Update(void)
{
	if (anim_.IsEnd())
	{
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
	}
}
