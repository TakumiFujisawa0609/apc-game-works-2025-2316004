#include"../Utility/Utility3D.h"
#include"../Utility/UtilityCommon.h"
#include"../Manager/Generic/SceneManager.h"
#include"../../../Manager/Generic/Camera.h"
#include"../Player/ActionController.h"
#include"../Base/InputBase.h"
#include "ActionBase.h"

ActionBase::ActionBase(ActionController& _actCntl):
	scnMng_(SceneManager::GetInstance()),
	actionCntl_(_actCntl)
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

void ActionBase::Init(void)
{
}

void ActionBase::Update(void)
{
	MoveDirFronInput();
	Rotate();
	DirAndMovePowUpdate();
}


void ActionBase::MoveDirFronInput(void)
{
	//入力情報を取得
	InputBase& input = actionCntl_.GetInput();

	//プレイヤー入力クラスから角度を取得
	VECTOR getDir = input.GetDir();
	float deg = input.GetMoveDeg();

	//カメラの角度ど入力角度でプレイヤーの方向を変える
	Quaternion cameraRot = scnMng_.GetCamera().lock()->GetQuaRotOutX();
	dir_ = cameraRot.PosAxis(getDir);
	dir_ = VNorm(dir_);

	if (!Utility3D::EqualsVZero(dir_))
	{
		//補完角度の設定(入力角度まで方向転換する)
		SetGoalRotate(deg);
	}
}

void ActionBase::Rotate(void)
{
	stepRotTime_ -= SceneManager::GetInstance().GetDeltaTime();
	// 回転の球面補間
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

void ActionBase::DirAndMovePowUpdate(void)
{
	//方向の更新
	moveDir_ = dir_;
	//移動量の更新
	movePow_ = VScale(moveDir_, speed_);
}

void ActionBase::SetGoalRotate(const double _deg)
{
	//カメラの角度を取得
	VECTOR cameraRot = scnMng_.GetCamera().lock()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis(
		(double)cameraRot.y + UtilityCommon::Deg2RadD(_deg), Utility3D::AXIS_Y);

	//現在設定されている回転との角度差を取る
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	constexpr double ANGLE_THRESHOLD = 0.1;
	// しきい値
	if (angleDiff > ANGLE_THRESHOLD)
	{
		stepRotTime_ = TIME_ROT;
	}
	goalQuaRot_ = axis;
}
