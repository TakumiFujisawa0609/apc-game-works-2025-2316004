#include "./CharacterBase.h"
#include "../Base/CardActionBase.h"
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

const bool LogicBase::GetIsEnemyJumpCharge(void)const
{
	return targetChara_.lock()->GetCardAction() == CardActionBase::CARD_ACT_TYPE::DUEL_FAZE
		&& targetChara_.lock()->GetMainAction().IsJumpAtkCharge();
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

const int LogicBase::GetJumpCardNum(void) const
{
	return targetChara_.lock()->GetMainAction().GetJumpCardNum();
}
