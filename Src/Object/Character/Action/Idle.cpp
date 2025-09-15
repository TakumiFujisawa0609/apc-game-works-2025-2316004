#include "Idle.h"

Idle::Idle(InputBase& _input):
	ActionBase(_input)
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
	//何もしないが、待機モーションとかずっと止まってるときにアニメーションを変えたりしたい
}