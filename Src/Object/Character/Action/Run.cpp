#include "../Base/ActionBase.h"
#include"../Base/InputBase.h"
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
	//ˆÚ“®’†‚É“ü—Í‚ª“ü‚Á‚½‚Ìó‘Ô‘JˆÚ
	ActionBase::Update();
	if (actionCntl_.GetInput().GetMoveDeg() < 0.0f)
	{
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		return;
	}
	

}
