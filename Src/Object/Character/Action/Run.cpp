#include "../Base/ActionBase.h"
#include "Run.h"

Run::Run(InputBase& _input):
	ActionBase(_input)
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
	//移動
	//入力方向の移動
	MoveDirFronInput();

	//方向の更新
	moveDir_ = dir_;
	//移動量の更新
	movePow_ = VScale(moveDir_, speed_);
}
