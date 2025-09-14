#include "../Character/Base/ActionBase.h"
#include "Run.h"

Run::Run(InputBase& _input):
	ActionBase(_input)
{

}

Run::~Run(void)
{
}

void Run::Init(void)
{
	speed_ = MOVE_SPEED;
}

void Run::Update(void)
{
	//ˆÚ“®
	//“ü—Í•ûŒü‚ÌˆÚ“®
	MoveDirFronInput();
}
