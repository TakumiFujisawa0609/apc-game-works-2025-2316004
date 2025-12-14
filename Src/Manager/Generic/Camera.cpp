#include "../pch.h"

#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include"../Application.h"
#include "../../Utility/Utility3D.h"
#include "../../Utility//UtilityCommon.h"
#include "../Common/Easing.h"
#include "../../Object/Common/Transform.h"
#include "../../Object/Common/Geometry/Sphere.h"
#include "../../Object/Common/Geometry/Line.h"
#include"../../Manager/Generic/InputManager.h"
#include"../../Manager/Generic/InputManagerS.h"
#include"../../Manager/Generic/SceneManager.h"
#include "Camera.h"

Camera::Camera(void)
{
	angles_ = VECTOR();
	cameraUp_ = VECTOR();
	mode_ = MODE::NONE;
	pos_ = Utility3D::VECTOR_ZERO;
	targetPos_ = Utility3D::VECTOR_ZERO;
	followTransform_ = nullptr;
	isChangingCamera_ = false;
	changeTargetLerpCnt_ = CHANGE_TARGET_LERP_TIME;
	shekePerCnt_ = 0.0f;
}

Camera::~Camera(void)
{
	followTransform_ = nullptr;
}

void Camera::Init(void)
{
	easing_ = std::make_unique<Easing>();
	changeMode_ = {
		{MODE::FIXED_POINT,[this]() {ChangeFixedPoint(); }},
		{MODE::FOLLOW,[this]() {ChangeFollow(); }},
		{MODE::SELF_SHOT,[this]() {ChangeSelfShot(); }},
		{MODE::TARGET_POINT,[this]() {ChangeTargetCamera(); }},
		{MODE::CHANGE_TARGET,[this]() {ChangeTargetLerp(); }}
	};
	changeSub_ = {
		{SUB_MODE::NONE,[this]() {ChangeNone(); }},
		{SUB_MODE::SHAKE,[this]() {ChangeShake(); }},
		{SUB_MODE::ONE_SHAKE,[this]() {ChanegShakeOne(); }}
	};
	subMode_ = SUB_MODE::SHAKE;
	ChangeSub(SUB_MODE::NONE);
	ChangeMode(MODE::FIXED_POINT);
}

void Camera::Update(void)
{
	//モード更新
	modeUpdate_();
	//イージングなどの更新
	subUpdate_();
}

void Camera::SetBeforeDraw(void)
{

	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_, 
		targetPos_, 
		cameraUp_
	);

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();

}

void Camera::Draw(void)
{
}

void Camera::ChangeSub(const  SUB_MODE _submode)
{
	//同じモード、またはすでに連続シェイクが入っている場合は処理を抜ける
	if (subMode_ == _submode || (subMode_ == SUB_MODE::SHAKE && _submode == SUB_MODE::ONE_SHAKE))return;
	subMode_ = _submode;
	changeSub_[subMode_]();
}

void Camera::SetFollow(const Transform* follow)
{
	followTransform_ = follow;
}

void Camera::SetTarget(const Transform* _target)
{
	targetTransform_ = _target;
}

void Camera::SetTargetPos(const VECTOR _targetPos)
{
	targetPoses_ = _targetPos;
}


VECTOR Camera::GetPos(void) const
{
	return pos_;
}

VECTOR Camera::GetAngles(void) const
{
	return angles_;
}

VECTOR Camera::GetTargetPos(void) const
{
	return targetPos_;
}

Quaternion Camera::GetQuaRot(void) const
{
	return rot_;
}

Quaternion Camera::GetQuaRotOutX(void) const
{
	return rotOutX_;
}

VECTOR Camera::GetForward(void) const
{
	return VNorm(VSub(targetPos_, pos_));
}

void Camera::ChangeMode(const MODE mode)
{
	// カメラの初期設定
	//SetDefault();
	if (mode_ == mode)return;
	// カメラモードの変更
	mode_ = mode;
	changeMode_[mode_]();
}



void Camera::MakeColliderGeometry(void)
{
	tag_ = Collider::TAG::CAMERA;
	//球体の当たり判定
	std::unique_ptr<Geometry>geo = std::make_unique<Sphere>(pos_, HIT_RADIUS);
	MakeCollider(TAG_PRIORITY::CAMERA_SPHERE, { tag_ }, std::move(geo));
	tagPrioritys_.emplace_back(TAG_PRIORITY::CAMERA_SPHERE);

	//ステージ以外都は当たり判定しない
	noneHitTag_.emplace(Collider::TAG::ENEMY1);
	noneHitTag_.emplace(Collider::TAG::JUMP_ATK);
	noneHitTag_.emplace(Collider::TAG::NML_ATK);
	noneHitTag_.emplace(Collider::TAG::ROAR_ATK);
	noneHitTag_.emplace(Collider::TAG::ROCK);
	noneHitTag_.emplace(Collider::TAG::PLAYER1);
}

void Camera::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	const std::set<Collider::TAG> tags = _hitCol.lock()->GetTags();

	//当たったタグがステージではなかった場合は抜ける
	if (std::find(tags.begin(), tags.end(), Collider::TAG::STAGE) == tags.end())return;

	Geometry& cameraGeo = collider_[TAG_PRIORITY::CAMERA_SPHERE]->GetGeometry();
	auto& modelInfo = cameraGeo.GetHitModelInfo();
	for (int i=0;i<modelInfo.HitNum;i++)
	{
		auto hit = modelInfo.Dim[i];
		//一定回数の押し出し処理をする
		for (int tryCnt = 0; tryCnt < COL_TRY_CNT_MAX; tryCnt++)
		{
			int pHit = HitCheck_Sphere_Triangle(pos_, HIT_RADIUS
				, hit.Position[0], hit.Position[1], hit.Position[2]);

			

			if (pHit)
			{
				VECTOR normal = hit.Normal;

				pos_ = VAdd(pos_, VScale(normal, 3.0f));

				continue;
			}
			break;
		}
	}

}

void Camera::SetDefault(void)
{

	// カメラの初期設定
	pos_ = DEFAULT_CAMERA_POS;

	// 注視点
	targetPos_ = Utility3D::VECTOR_ZERO;

	// カメラの上方向
	cameraUp_ = Utility3D::DIR_U;

	angles_.x = UtilityCommon::Deg2RadF(30.0f);
	angles_.y = 0.0f;
	angles_.z = 0.0f;

	rot_ = Quaternion();

}

void Camera::SyncFollow(void)
{

	//auto& gIns = GravityManager::GetInstance();

	// 同期先の位置
	VECTOR followPos = followTransform_->pos;

	// 重力の方向制御に従う
	Quaternion gRot = Quaternion::Euler(Utility3D::VECTOR_ZERO);

	// 正面から設定されたY軸分、回転させる
	rotOutX_ = gRot.Mult(Quaternion::AngleAxis(angles_.y, Utility3D::AXIS_Y));

	// 正面から設定されたX軸分、回転させる
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, Utility3D::AXIS_X));

	VECTOR localPos;

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(followPos, localPos);

	// カメラ位置
	localPos = rot_.PosAxis(LOCAL_F2C_POS);
	pos_ = VAdd(followPos, localPos);

	// カメラの上方向
	cameraUp_ = gRot.GetUp();

}

void Camera::SyncTargetFollow(void)
{
	// 同期先の位置	
	VECTOR followPos = followTransform_->pos;
	VECTOR targetPos = targetTransform_->pos;
	VECTOR targetVec = Utility3D::GetMoveVec(followPos, targetPos);
	targetVec.y = 0.0f;
	VECTOR targetRot = Utility3D::GetRotAxisToTarget(followPos, targetPos,Utility3D::AXIS_Y);

	// 重力の方向制御に従う
	Quaternion gRot = Quaternion::Euler(Utility3D::VECTOR_ZERO);

	// 正面から設定されたY軸分、回転させる
	//rotOutX_ = gRot.Mult(Quaternion::AngleAxis(angles_.y, Utility3D::AXIS_Y));
	rotOutX_ = gRot.Mult(Quaternion::AngleAxis(static_cast<double>(targetRot.y), Utility3D::AXIS_Y));

	// 正面から設定されたX軸分、回転させる
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(static_cast<double>(targetRot.x), Utility3D::AXIS_X));

	//カメラ角度の同期
	angles_ = targetRot;

	VECTOR localPos;

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
	//targetPos_ = VAdd(followPos, localPos);
	//targetPos_ = VAdd(targetPos, localPos);

	// カメラ位置
	localPos = rot_.PosAxis(TARGET_CAM_LOCAL_F2C_POS);
	pos_ = VAdd(followPos, localPos);


	// カメラの上方向
	cameraUp_ = gRot.GetUp();
}

void Camera::ProcessRot(void)
{
	//int x_t, y_t;
	Vector2 mPos;
	mPos = InputManager::GetInstance().GetMousePos();
	//GetMousePoint(&x_t, &y_t);
	//マウスの移動量をクランプして、カメラの角度に反映する
	angles_.y += float(std::clamp(mPos.x - Application::SCREEN_SIZE_X / 2, -120, 120)) * FOV_PER / GetFPS();
	angles_.x += float(std::clamp(mPos.y - Application::SCREEN_SIZE_Y / 2, -120, 120)) * FOV_PER / GetFPS();

	// マウスの位置を画面中央に戻す
	SetMousePoint(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2);

	// マウスを表示状態にする
	SetMouseDispFlag(FALSE);

	auto rStick = InputManagerS::GetInstance().GetKnockRStickSize(InputManager::JOYPAD_NO::PAD1);
	float rotPow = SPEED_PAD;
	angles_.x += UtilityCommon::Deg2RadF(rStick.y * rotPow);
	angles_.y += UtilityCommon::Deg2RadF(rStick.x * rotPow);

	if (angles_.x >= LIMIT_X_UP_RAD)
	{
		angles_.x = LIMIT_X_UP_RAD;
	}
	else if (angles_.x <= LIMIT_X_DW_RAD)
	{
		angles_.x = LIMIT_X_DW_RAD;
	}
}

void Camera::SmoothChangeCamera(void)
{
	// 同期先の位置	
	VECTOR followPos = followTransform_->pos;
	VECTOR targetPos = targetTransform_->pos;
	VECTOR targetVec = Utility3D::GetMoveVec(followPos, targetPos);
	targetVec.y = 0.0f;

	VECTOR targetRot = Utility3D::GetRotAxisToTarget(followPos, targetPos, Utility3D::AXIS_Y);

	//補完割合
	double lerpRate = (CHANGE_TARGET_LERP_TIME - changeTargetLerpCnt_) / CHANGE_TARGET_LERP_TIME;

	// 重力の方向制御に従う
	Quaternion gRot = Quaternion::Euler(Utility3D::VECTOR_ZERO);

	// 正面から設定されたY軸分、回転させる
	Quaternion goalrotOutX = gRot.Mult(Quaternion::AngleAxis(static_cast<double>(targetRot.y), Utility3D::AXIS_Y));

	// 正面から設定されたX軸分、回転させる
	Quaternion goalrot = rotOutX_.Mult(Quaternion::AngleAxis(static_cast<double>(targetRot.x), Utility3D::AXIS_X));
	if (changeTargetLerpCnt_ > 0.0)
	{
		rotOutX_.Slerp(rotOutX_, goalrotOutX, lerpRate);
		rot_.Slerp(rot_, goalrot, lerpRate);
	}
	
	//カメラ角度の同期
	angles_ = targetRot;

	VECTOR localPos;

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
	//targetPos_ = VAdd(followPos, localPos);
	VECTOR goalTargetPos = VAdd(targetPos, localPos);
	if (changeTargetLerpCnt_ > 0.0)
	{
		targetPos_ = easing_->EaseFunc(targetPos_, goalTargetPos, lerpRate,Easing::EASING_TYPE::LERP);
	}


	// カメラ位置
	localPos = rot_.PosAxis(TARGET_CAM_LOCAL_F2C_POS);
	VECTOR goalPos = VAdd(followPos, localPos);
	if (changeTargetLerpCnt_ > 0.0)
	{
		pos_ = easing_->EaseFunc(pos_, goalPos, lerpRate,Easing::EASING_TYPE::LERP);
	}

	// カメラの上方向
	cameraUp_ = gRot.GetUp();

	changeTargetLerpCnt_ -= SceneManager::GetInstance().GetDeltaTime();

	if (changeTargetLerpCnt_ < 0.0)
	{
		changeTargetLerpCnt_ = CHANGE_TARGET_LERP_TIME;
		ChangeMode(Camera::MODE::TARGET_POINT);
	}
}

void Camera::SetBeforeDrawFixedPoint(void)
{
	// 何もしない
}

void Camera::SetBeforeDrawFollow(void)
{
	// カメラ操作
	ProcessRot();

	// 追従対象との相対位置を同期
	SyncFollow();

	//if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_T))
	//{
	//	ChangeMode(MODE::TARGET_POINT);
	//}
}

void Camera::SetBeforeDrawSelfShot(void)
{
}

void Camera::SetBeforeDrawLerpCamera(void)
{
	SmoothChangeCamera();
}

void Camera::SetBeforeDrawTargetPoint(void)
{
	ProcessRot();
	SyncTargetFollow();

	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_T))
	{
		ChangeMode(Camera::MODE::FOLLOW);
	}
}

void Camera::ChangeFixedPoint(void)
{
	SetDefault();
	modeUpdate_ = [this]() {SetBeforeDrawFixedPoint(); };
}

void Camera::ChangeFollow(void)
{
	SetDefault();
	modeUpdate_ = [this]() {SetBeforeDrawFollow(); };
}

void Camera::ChangeSelfShot(void)
{
	SetDefault();
	modeUpdate_ = [this]() {SetBeforeDrawSelfShot(); };
}

void Camera::ChangeTargetLerp(void)
{
	modeUpdate_ = [this]() {SetBeforeDrawLerpCamera(); };
}

void Camera::ChangeTargetCamera(void)
{
	SetDefault();
	modeUpdate_ = [this]() {SetBeforeDrawTargetPoint(); };
}
void Camera::UpdateNone(void)
{
	//何もしない
}
void Camera::UpdateShake(void)
{
	//シェイク時間が終わったらNone状態へ
	if (easePer_ > 1.0f)
	{
		ChangeSub(SUB_MODE::NONE);
		return;
	}
	if (shekePerCnt_ > SHAKE_PER)
	{
		shekePerCnt_ = 0.0f;
	}
	//ローカル座標を計算
	float localPosY = easing_->EaseFunc(0.0f, limit_, shekePerCnt_ / SHAKE_PER, easeType_);

	//ローカル座標を足す
	pos_.y += localPosY;

	//1シェイクにかかる時間をカウント
	shekePerCnt_ += SceneManager::GetInstance().GetDeltaTime();

}
void Camera::UpdateShakeOne(void)
{
	//シェイク時間が終わったらNone状態へ
	if (easePer_ > 1.0f)
	{
		ChangeSub(SUB_MODE::NONE);
		return;
	}
	//ローカル座標を計算
	float localPosY = easing_->EaseFunc(0.0f, limit_, easePer_, easeType_);

	//ローカル座標を足す
	pos_.y += localPosY;

	//1シェイクにかかる時間をカウント
	shekePerCnt_ += SceneManager::GetInstance().GetDeltaTime();
}
void Camera::ChangeNone(void)
{
	easePer_ = 0.0f;
	subUpdate_ = [this]() {UpdateNone(); };

}
void Camera::ChangeShake(void)
{
	initPosY_ = pos_.y;
	limit_ = initLimit_;
	subUpdate_ = [this]() {UpdateShake(); };
}

void Camera::ChanegShakeOne(void)
{
	initPosY_ = pos_.y;
	limit_ = initLimit_;
	subUpdate_ = [this]() {UpdateShakeOne(); };
}
