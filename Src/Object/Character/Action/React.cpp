#include "../Manager/Generic/SceneManager.h"
#include "../Base/ActionBase.h"
#include "../Base/LogicBase.h"
#include "../Utility/Utility3D.h"
#include "../Player/ActionController.h"
#include "../Base/CharacterBase.h"
#include "../../Common/AnimationController.h"

#include "React.h"

React::React(ActionController& _actCntl):
	ActionBase(_actCntl)
{
	isTurnable_ = false;
}

React::~React(void)
{
}

void React::Load(void)
{

}

void React::Init(void)
{
	bool isLoop = false;
	if (flinchCnt_ > 0.0f)
	{
		isLoop = true;
	}
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::REACT), isLoop, 0.0f, 16.0f);
}

void React::Update(void)
{
	if (flinchCnt_ == 0)
	{
		if (anim_.IsEnd())
		{
			actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		}
	}
	else
	{
		flinchCnt_ -= SceneManager::GetInstance().GetDeltaTime();
		anim_.SetEndLoop(14.0f, 16.0f, 3.0f);
		if (flinchCnt_ < 0.0f)
		{
			flinchCnt_ = 0.0f;
			actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		}
	}

}
