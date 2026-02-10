#include "./CharacterBase.h"
#include "../Base/CardActionBase.h"
#include"../Utility/Utility3D.h"
#include "LogicBase.h"

LogicBase::LogicBase(Transform& _myTrans):
	myTrans_(_myTrans),
	actCntl_(ACT_CNTL::NONE),
	moveDeg_(-1.0f),
	moveDir_({}),
	//targetChara_(nullptr),
	isActioning_(false),
	prevMoveDir_(Utility3D::DIR_F)
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

void LogicBase::GetLookAtTargetDir(void)
{
	const VECTOR& targetPos = targetChara_.lock()->GetTransform().pos;
	const VECTOR targetVec = Utility3D::GetMoveVec(myTrans_.pos, targetPos);
	moveDir_ = targetVec;
}

void LogicBase::GetLookAtTargetDeg(void)
{
	float deg = static_cast<float>(Utility3D::AngleDeg(targetChara_.lock()->GetTransform().pos, myTrans_.pos));
	moveDeg_ += deg;
}

const bool LogicBase::HitTarget(void) const
{
	return false;
}

const Transform LogicBase::GetTargetTransform(void)
{
	return targetChara_.lock()->GetTransform();
}

const int LogicBase::GetJumpCardNum(void) const
{
	return targetChara_.lock()->GetMainAction().GetJumpCardNum();
}

const float LogicBase::GetTargetDis(void) const
{
	const VECTOR targetPos = targetChara_.lock()->GetTransform().pos;
	return static_cast<float>(Utility3D::Distance(myTrans_.pos, targetPos));
}
