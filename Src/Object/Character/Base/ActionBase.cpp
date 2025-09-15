#include"../Utility/Utility3D.h"
#include"../Utility/UtilityCommon.h"
#include"../Manager/Generic/SceneManager.h"
#include"../Base/InputBase.h"
#include"../../../Manager/Generic/Camera.h"
#include "ActionBase.h"

ActionBase::ActionBase(InputBase& _input):
	scnMng_(SceneManager::GetInstance()),
	input_(_input)
{
	speed_ = 0.0f;
	dir_ = {};
	moveDir_ = {};
	movePow_ = {};

	isJump_ = false;
	jumpDeceralation_ = 0.0f;
	jumpPow_ = {};
	stepJump_ = 0.0f;
	stepRotTime_ = 0.0f;

}

ActionBase::~ActionBase()
{
}

void ActionBase::ChangeAction(ActionBase* action)
{
}


void ActionBase::MoveDirFronInput(void)
{
	//ˆÚ“®—Ê‚ð0‚ÉƒŠƒZƒbƒg
	movePow_ = Utility3D::VECTOR_ZERO;

	//ƒvƒŒƒCƒ„[“ü—ÍƒNƒ‰ƒX‚©‚çŠp“x‚ðŽæ“¾
	VECTOR getDir = input_.GetDir();
	float deg = input_.GetMoveDeg();

	//ƒJƒƒ‰‚ÌŠp“x‚Ç“ü—ÍŠp“x‚ÅƒvƒŒƒCƒ„[‚Ì•ûŒü‚ð•Ï‚¦‚é
	Quaternion cameraRot = scnMng_.GetCamera().lock()->GetQuaRotOutX();
	dir_ = cameraRot.PosAxis(getDir);
	dir_ = VNorm(dir_);

	if (!Utility3D::EqualsVZero(dir_))
	{
		//•âŠ®Šp“x‚ÌÝ’è(“ü—ÍŠp“x‚Ü‚Å•ûŒü“]Š·‚·‚é)
		SetGoalRotate(deg);
	}
}

void ActionBase::Rotate(void)
{
	stepRotTime_ -= SceneManager::GetInstance().GetDeltaTime();
	// ‰ñ“]‚Ì‹…–Ê•âŠÔ
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

void ActionBase::SetGoalRotate(const double _deg)
{
	//ƒJƒƒ‰‚ÌŠp“x‚ðŽæ“¾
	VECTOR cameraRot = scnMng_.GetCamera().lock()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis(
		(double)cameraRot.y + UtilityCommon::Deg2RadD(_deg), Utility3D::AXIS_Y);

	//Œ»ÝÝ’è‚³‚ê‚Ä‚¢‚é‰ñ“]‚Æ‚ÌŠp“x·‚ðŽæ‚é
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	constexpr double ANGLE_THRESHOLD = 0.1;
	// ‚µ‚«‚¢’l
	if (angleDiff > ANGLE_THRESHOLD)
	{
		stepRotTime_ = TIME_ROT;
	}
	goalQuaRot_ = axis;
}
