#include "../Base/ActionBase.h"
#include"../Base/InputBase.h"
#include"../Utility/Utility3D.h"
#include"../Player/ActionController.h"
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
	
}

void Run::Update(void)
{
	//if (actionCntl_.GetInput().GetMoveDeg() < 0.0f)
	if (Utility3D::EqualsVZero(actionCntl_.GetInput().GetDir()))
	{
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		return;
	}
	////ˆÚ“®’†‚É“ü—Í‚ª“ü‚Á‚½Žž‚Ìó‘Ô‘JˆÚ
	//ActionBase::Update();
}
