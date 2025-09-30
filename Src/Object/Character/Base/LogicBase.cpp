#include "LogicBase.h"

LogicBase::LogicBase(void):
	actCntl_(ACT_CNTL::NONE),
	moveDeg_(-1.0f),
	moveDir_({})
{
}

LogicBase::~LogicBase(void)
{
}
