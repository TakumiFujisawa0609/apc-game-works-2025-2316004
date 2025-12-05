#include "./CharacterBase.h"
#include "LogicBase.h"

LogicBase::LogicBase(void):
	actCntl_(ACT_CNTL::NONE),
	moveDeg_(-1.0f),
	moveDir_({}),
	//targetChara_(nullptr),
	isActioning_(false)
{
}

LogicBase::~LogicBase(void)
{
}

void LogicBase::SetTargetCharacter(std::shared_ptr<CharacterBase> _target)
{
	targetChara_ = _target;
}

const VECTOR LogicBase::GetLookAtTargetDir(void)const
{
	return {};
}

const float LogicBase::GetLookAtTargetDeg(void)const
{
	return 0.0;
}

const Transform LogicBase::GetTargetTransform(void)
{
	return targetChara_.lock()->GetTransform();
}
