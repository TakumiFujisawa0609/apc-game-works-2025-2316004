#include "InputBase.h"

InputBase::InputBase(void):
	actCntl_(ACT_CNTL::NONE),
	moveDeg_(-1.0f),
	moveDir_({})
{
}

InputBase::~InputBase(void)
{
}
