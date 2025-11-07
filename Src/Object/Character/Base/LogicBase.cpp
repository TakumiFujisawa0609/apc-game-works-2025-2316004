#include "LogicBase.h"

LogicBase::LogicBase(void):
	actCntl_(ACT_CNTL::NONE),
	moveDeg_(-1.0f),
	moveDir_({}),
	isActioning_(false)
{
}

LogicBase::~LogicBase(void)
{
}

const VECTOR& LogicBase::GetLookAtTargetDir(void)const
{
	return {};
}

const float& LogicBase::GetLookAtTargetDeg(void)const
{
	return 0.0;
}
