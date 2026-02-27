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
#include "../../Object/Stage.h"
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
		{MODE::CHANGE_TARGET,[this]() {ChangeTargetLerp(); }},
		{MODE::START_DIRECTION ,[this]() {ChangeStartDirection(); }}
	};
	changeSub_ = {
		{SUB_MODE::NONE,[this]() {ChangeNone(); }},
		{SUB_MODE::SHAKE,[this]() {ChangeShake(); }},
		{SUB_MODE::ONE_SHAKE,[this]() {ChangeShakeOne(); }}
	};
	changeDirectionMode_ = {
		{DIRECTION_MODE::NONE,[this]() {ChangeDirectionNone(); }},
		{DIRECTION_MODE::PLAYER_AND_ENEMY_VIEW,[this]() {ChangeDirectionLegLow(); }},
		{DIRECTION_MODE::ENEMY_ONLY_VIEW,[this]() {ChangeDirectionEnemyOnly(); }},
		{DIRECTION_MODE::ENEMY_ROAR_VIEW,[this]() {ChangeDirectionEnemyRoar(); }},
		{DIRECTION_MODE::PLAYER_ONLY_VIEW,[this]() {ChangeDirectionPlayerOnly(); }},
		{DIRECTION_MODE::END,[this]() {ChangeEndDirection(); }}
	};
	subMode_ = SUB_MODE::SHAKE;
	directionEaseCnt_ = 0.0f;
	startF2CPosZ_ = {};
	goalF2CPosZ_ = {};
	localF2CPos_ = {};
	localF2TPos_ = {};
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
	//float degX = UtilityCommon::Rad2DegF(angles_.x);
	//float degY = UtilityCommon::Rad2DegF(angles_.y);
	//float degZ = UtilityCommon::Rad2DegF(angles_.z);
	//DrawFormatString(0, 0, GetColor(255, 255, 255)
	//	, L"F2CPos:(%.2f,%.2f,%.2f)\nF2TPos:(%.2f,%.2f,%.2f)\nangle:(%.2f,%.2f,%.2f)\nPos(%.2f,%.2f,%.2f)"
	//	, localF2CPos_.x, localF2CPos_.y, localF2CPos_.z,
	//	localF2TPos_.x, localF2TPos_.y, localF2TPos_.z ,
	//	degX, degY, degZ,
	//	pos_.x,pos_.y,pos_.z);
	////DrawFormatString(0, 32, GetColor(255, 255, 255), L"Frame Pos:(%.2f,%.2f,%.2f)", followFramePos_.x, followFramePos_.y, followFramePos_.z);


	//if (collider_.size() > 0)
	//{
	//	//VECTOR local = VSub(followTransform_->pos, pos_);
	//	//DrawFormatString(0, 64, GetColor(255, 255, 255), L"Local Pos:(%.2f,%.2f,%.2f)", local.x, local.y, local.z);

	//	// 同期先の位置
	//	VECTOR followPos = VAdd(followTransform_->pos, followLocalCenterPos_);

	//	VECTOR pos = collider_[TAG_PRIORITY::CAMERA_LINE]->GetGeometry().GetPosPoint2();
	//	DrawSphere3D(
	//		pos, 50.0f, 8, GetColor(255, 0, 0), GetColor(255, 0, 0), TRUE);
	//}
	//
}

void Camera::ChangeSub(const  SUB_MODE _submode)
{
	//同じモード、またはすでに連続シェイクが入っている場合は処理を抜ける
	if (subMode_ == _submode || (subMode_ == SUB_MODE::SHAKE && _submode == SUB_MODE::ONE_SHAKE))return;
	subMode_ = _submode;
	changeSub_[subMode_]();
}

void Camera::SetFollow(const Transform* follow,const VECTOR _localCenterPos)
{
	followTransform_ = follow;
	followLocalCenterPos_ = _localCenterPos;
}

void Camera::SetTarget(const Transform* _target)
{
	targetTransform_ = _target;
}

void Camera::SetTargetPos(const VECTOR _targetPos)
{
	targetPoses_ = _targetPos;
}

void Camera::SetShakeStatus(const float t, const float limit
	, const Easing::EASING_TYPE _easeType,const float shakeTime)
{
	easePer_ = t;
	initLimit_ = limit;
	oneShakeTime_ = shakeTime;
	easeType_ = _easeType;
}


//VECTOR Camera::GetPos(void) const
//
//
//VECTOR Camera::GetAngles(void) const
//{
//	return angles_;
//}
//
//VECTOR Camera::GetTargetPos(void) const
//{
//	return targetPos_;
//}
//
//Quaternion Camera::GetQuaRot(void) const
//{
//	return rot_;
//}
//
//Quaternion Camera::GetQuaRotOutX(void) const
//{
//	return rotOutX_;
//}

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
	//ステージ以外都は当たり判定しない
	noneHitTag_.emplace(Collider::TAG::ENEMY1);
	noneHitTag_.emplace(Collider::TAG::JUMP_ATK);
	noneHitTag_.emplace(Collider::TAG::NML_ATK);
	noneHitTag_.emplace(Collider::TAG::ROAR_ATK);
	noneHitTag_.emplace(Collider::TAG::ROCK);
	noneHitTag_.emplace(Collider::TAG::PLAYER1);

	followFramePos_ = { 0.0f,0.0f,0.0f };
	std::unique_ptr<Geometry>geo = std::make_unique<Line>(pos_, rot_, Utility3D::VECTOR_ZERO, Utility3D::VECTOR_ZERO);
	MakeCollider(TAG_PRIORITY::CAMERA_LINE, { tag_ }, std::move(geo),noneHitTag_);
	tagPrioritys_.emplace_back(TAG_PRIORITY::CAMERA_LINE);

}

void Camera::OnHit(const std::weak_ptr<Collider> _hitCol)
{

}

void Camera::UpdateCameraColliderLine(void)
{
	Geometry& cameraGeo = collider_[TAG_PRIORITY::CAMERA_LINE]->GetGeometry();
	followFramePos_ = MV1GetFramePosition(followTransform_->modelId, FOLLOW_FRAME_NUM);

	VECTOR pos = VAdd(followFramePos_, pos_);

	VECTOR localPos = VSub(pos, pos_);
	cameraGeo.SetLocalPosPoint1(Utility3D::VECTOR_ZERO);
	cameraGeo.SetLocalPosPoint2(localPos);
}

void Camera::SetDefault(void)
{

	// カメラの初期設定
	pos_ = DEFAULT_CAMERA_POS;

	// 注視点
	targetPos_ = Utility3D::VECTOR_ZERO;

	// カメラの上方向
	cameraUp_ = Utility3D::DIR_U;

	angles_.x = UtilityCommon::Deg2RadF(DEFAULT_CAMERA_ANGLES_RAD_X);
	angles_.y = 0.0f;
	angles_.z = 0.0f;

	startF2CPosZ_ = {};
	goalF2CPosZ_ = {};
	rot_ = Quaternion();

}

void Camera::SyncFollow(const Transform* _followTransform)
{
	// 同期先の位置
	VECTOR followPos = VAdd(_followTransform->pos, followLocalCenterPos_);

	// 重力の方向制御に従う
	Quaternion gRot = Quaternion::Euler(Utility3D::VECTOR_ZERO);

	// 正面から設定されたY軸分、回転させる
	rotOutX_ = gRot.Mult(Quaternion::AngleAxis(angles_.y, Utility3D::AXIS_Y));

	// 正面から設定されたX軸分、回転させる
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, Utility3D::AXIS_X));

	VECTOR localPos;

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = rotOutX_.PosAxis(localF2TPos_);
	targetPos_ = VAdd(followPos, localPos);

	// カメラ位置
	localPos = rot_.PosAxis(localF2CPos_);
	pos_ = VAdd(followPos, localPos);

	// カメラの上方向
	cameraUp_ = gRot.GetUp();

}

VECTOR Camera::GetSyncFollowPos(const Transform* _followTransform)
{
	return VECTOR();
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
	rotOutX_ = gRot.Mult(Quaternion::AngleAxis(static_cast<double>(targetRot.y), Utility3D::AXIS_Y));

	// 正面から設定されたX軸分、回転させる
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(static_cast<double>(targetRot.x), Utility3D::AXIS_X));

	//カメラ角度の同期
	angles_ = targetRot;

	VECTOR localPos;

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
	//targetPos_ = VAdd(followPos, localPos);
	targetPos_ = VAdd(targetPos, localPos);

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
		targetPos_ = easing_->EaseFunc(targetPos_, goalTargetPos, static_cast<float>(lerpRate),Easing::EASING_TYPE::LERP);
	}


	// カメラ位置
	localPos = rot_.PosAxis(TARGET_CAM_LOCAL_F2C_POS);
	VECTOR goalPos = VAdd(followPos, localPos);
	if (changeTargetLerpCnt_ > 0.0)
	{
		pos_ = easing_->EaseFunc(pos_, goalPos, static_cast<float>(lerpRate),Easing::EASING_TYPE::LERP);
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

void Camera::Collision(void)
{

	auto hits = MV1CollCheck_LineDim(stageTransform_->modelId
		, -1, pos_, followFramePos_);

	//追従点に最も近いポリゴンを探す
	bool isCol = false;
	MV1_COLL_RESULT_POLY hitPoly;
	float minDist = FLT_MAX;

	for (int i = 0; i < hits.HitNum; i++)
	{
		auto hit = hits.Dim[i];

		isCol = true;
		//距離判定
		float dis = static_cast<float>(Utility3D::Distance(followFramePos_, hit.HitPosition));
		if (dis < minDist)
		{
			minDist = dis;
			hitPoly = hit;
		}
	}
	//検出した地面ポリゴン情報後始末
	//MV1CollResultPolyDimTerminate(hits);

	//カメラから注視点へのベクトル
	MV1CollResultPolyDimTerminate(hits);

	if (hits.HitNum > 0)
	{
		VECTOR vecToFollow = Utility3D::GetMoveVec(pos_, followFramePos_);
		//衝突点の少し手前にカメラを配置
		pos_ = VAdd(hitPoly.HitPosition, VScale(vecToFollow, HIT_NORMAL_OFFSET));


		int sphereCnt = 0;
		while (sphereCnt < CNT_TRY_COLLISION_CAMERA)
		{
			// 球体と三角形の当たり判定
			int sphereHit = HitCheck_Sphere_Triangle(
				pos_,
				COL_SPHERE_SPHERE,
				hitPoly.Position[0],
				hitPoly.Position[1],
				hitPoly.Position[2]);
			// 衝突していたら法線方向に押し戻し
			pos_ = VAdd(pos_, VScale(hitPoly.Normal, COLLISION_BACK_DIS));
			sphereCnt++;
		}
	}
}

void Camera::ChangeDirectionMode(const DIRECTION_MODE _mode)
{
	if (directionMode_ == _mode)return;
	directionCnt_ = 0.0f;
	
	directionMode_ = _mode;
	changeDirectionMode_[directionMode_]();
}

void Camera::SetBeforeDrawFixedPoint(void)
{
	// 何もしない
}

void Camera::SetBeforeDrawFollow(void)
{
	VECTOR prePos = pos_;
	if (followTransform_ == nullptr)
	{
		ChangeMode(MODE::FIXED_POINT);
		return;
	}
	// カメラ操作
	ProcessRot();

	// 追従対象との相対位置を同期
	SyncFollow(followTransform_);

	//線分当たり判定の更新
	UpdateCameraColliderLine();

	Collision();

	pos_ = easing_->EaseFunc(prePos, pos_, 0.1f, Easing::EASING_TYPE::LERP);
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_T))
	{
		ChangeMode(MODE::TARGET_POINT);
	}
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
	if (followTransform_ == nullptr)
	{
		ChangeMode(MODE::FIXED_POINT);
		return;
	}
	ProcessRot();
	SyncTargetFollow();

	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_T))
	{
		ChangeMode(Camera::MODE::FOLLOW);
	}
}

void Camera::SetBeforeDrawStartDirection(void)
{
	directionUpdate_();
}

void Camera::ChangeFixedPoint(void)
{
	SetDefault();
	modeUpdate_ = [this]() {SetBeforeDrawFixedPoint(); };
}

void Camera::ChangeFollow(void)
{
	//SetDefault();
	localF2CPos_ = LOCAL_F2C_POS;
	localF2TPos_ = LOCAL_F2T_POS;
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
void Camera::ChangeStartDirection(void)
{
	directionMode_ = DIRECTION_MODE::NONE;
	ChangeDirectionMode(DIRECTION_MODE::PLAYER_AND_ENEMY_VIEW);
	modeUpdate_ = [this]() {SetBeforeDrawStartDirection(); };
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
	if (easePer_ > 1.0f|| shekePerCnt_> oneShakeTime_)
	{
		ChangeSub(SUB_MODE::NONE);
		return;
	}
	//ローカル座標を計算
	float localPosY = 0.0f;
	if (easePer_ != -1.0f)
	{
		localPosY = easing_->EaseFunc(0.0f, limit_, easePer_, easeType_);
	}
	else
	{
		localPosY = easing_->EaseFunc(0.0f, limit_, shekePerCnt_ /oneShakeTime_, easeType_);
	}

	//ローカル座標を足す
	pos_.y += localPosY;

	//1シェイクにかかる時間をカウント
	shekePerCnt_ += SceneManager::GetInstance().GetDeltaTime();
}
void Camera::ChangeNone(void)
{
	easePer_ = 0.0f;
	shekePerCnt_ = 0.0f;
	subUpdate_ = [this]() {UpdateNone(); };

}
void Camera::ChangeShake(void)
{
	initPosY_ = pos_.y;
	limit_ = initLimit_;
	subUpdate_ = [this]() {UpdateShake(); };
}

void Camera::ChangeShakeOne(void)
{
	initPosY_ = pos_.y;
	limit_ = initLimit_;
	subUpdate_ = [this]() {UpdateShakeOne(); };
}

void Camera::DirectionNone(void)
{
}



void Camera::DirectionPlayerAndTarget(void)
{
	if(directionCnt_>PLAYER_AND_ENEMY_VIEW_TIME)
	{
		ChangeDirectionMode(DIRECTION_MODE::ENEMY_ONLY_VIEW);
		return;
	}
	directionCnt_ += SceneManager::GetInstance().GetDeltaTime();
	SyncFollow(targetTransform_);
	angles_.y += UtilityCommon::Deg2RadF(0.1f);
}

void Camera::DirectionEnemyOnly(void)
{
	if (directionCnt_ > PLAYER_AND_ENEMY_VIEW_TIME)
	{
		ChangeDirectionMode(DIRECTION_MODE::ENEMY_ROAR_VIEW);
		return;
	}
	
	////ローカル座標をイージングで補完
	localF2CPos_ = easing_->EaseFunc(easingStartF2CPos_, easingGoalF2CPos_, directionCnt_ / 1.0f
		, Easing::EASING_TYPE::OUT_BACK);
	localF2TPos_ = easing_->EaseFunc(easingStartF2TPos_, easingGoalF2TPos_, directionCnt_ / 1.0f
		, Easing::EASING_TYPE::OUT_BACK);


	SyncFollow(targetTransform_);
	directionCnt_ += SceneManager::GetInstance().GetDeltaTime();
	
}

void Camera::DirectionEnemyRoar(void)
{
	if (directionCnt_ > ENEMY_ROAR_VIEW_TIME)
	{
		ChangeSub(SUB_MODE::NONE);
		ChangeDirectionMode(DIRECTION_MODE::PLAYER_ONLY_VIEW);
		return;
	}
	//SetShakeStatus(directionCnt_ / PLAYER_AND_ENEMY_VIEW_TIME, ENEMY_ROAR_SHAKE_LIMIT);
	//ChangeSub(SUB_MODE::SHAKE);
	SyncFollow(targetTransform_);
	directionCnt_ += SceneManager::GetInstance().GetDeltaTime();

}

void Camera::DirectionPlayerOnly(void)
{
	//localF2CPos_.z = easing_->EaseFunc(startF2CPosZ_, goalF2CPosZ_, directionCnt_ / 1.0f, Easing::EASING_TYPE::BOUNCE);

	if (directionCnt_ > PLAYER_AND_ENEMY_VIEW_TIME)
	{
		ChangeDirectionMode(DIRECTION_MODE::END);
	}

	SyncFollow(followTransform_);
	if (directionCnt_ <= 1.0f)
	{
		localF2CPos_ = easing_->EaseFunc(easingStartF2CPos_, easingGoalF2CPos_, directionCnt_ / 1.0f
			, Easing::EASING_TYPE::QUAD_OUT);
	}

	directionCnt_ += SceneManager::GetInstance().GetDeltaTime();
}

void Camera::EndDirection(void)
{
	if (directionCnt_ > PLAYER_AND_ENEMY_VIEW_TIME)
	{
		ChangeMode(MODE::FOLLOW);
		return;
	}

	angles_ = easing_->EaseFunc(startAngles_, goalAngles_, directionCnt_ / 2.0f, Easing::EASING_TYPE::QUAD_OUT);
	localF2CPos_ = easing_->EaseFunc(easingStartF2CPos_, LOCAL_F2C_POS, directionCnt_ / 2.0f, Easing::EASING_TYPE::QUAD_OUT);
	followLocalCenterPos_ = easing_->EaseFunc(startFollowLocalCenterPos_, goalFollowLocalCenterPos_, directionCnt_ / 2.0f, Easing::EASING_TYPE::QUAD_OUT);
	SyncFollow(followTransform_);
	directionCnt_ += SceneManager::GetInstance().GetDeltaTime();

}

void Camera::ChangeDirectionNone(void)
{
	directionUpdate_ = [this]() {DirectionNone(); };
}

void Camera::ChangeDirectionLegLow(void)
{
	localF2CPos_ = PLAYER_AND_ENEMY_LOCAL_F2C_POS;
	localF2TPos_ = LOCAL_F2T_POS;
	followLocalCenterPos_ = PLAYER_WAIST;
	directionCnt_ = 0.0f;
	directionUpdate_ = [this]() {DirectionPlayerAndTarget(); };
}

void Camera::ChangeDirectionEnemyOnly(void)
{
	directionCnt_ = 0.0f;
	easingStartF2CPos_ = localF2CPos_;
	easingGoalF2CPos_ = ENEMY_ONLY_LOCAL_F2C_POS;
	easingStartF2TPos_ = localF2TPos_;
	easingGoalF2TPos_= ENEMY_ONLY_LOCAL_F2T_POS;

	directionUpdate_ = [this]() {DirectionEnemyOnly(); };
}

void Camera::ChangeDirectionEnemyRoar(void)
{
	directionUpdate_= [this]() {DirectionEnemyRoar(); };
}

void Camera::ChangeDirectionPlayerOnly(void)
{
	localF2TPos_ = LOCAL_F2T_POS;

	easingStartF2CPos_ = PLAYER_ONLY_LOCAL_F2C_START_POS;
	easingGoalF2CPos_ = PLAYER_ONLY_LOCAL_F2C_GOAL_POS;

	followLocalCenterPos_ = PLAYER_HEAD_POS;
	directionCnt_ = 0.0f;
	angles_.y = UtilityCommon::Deg2RadF(PLAYER_ONLY_CAMERA_ANGLE_Y);
	directionUpdate_ = [this]() {DirectionPlayerOnly(); };
}

void Camera::ChangeEndDirection(void)
{
	easingStartF2CPos_ = localF2CPos_;
	startAngles_ = angles_;
	goalAngles_ = { UtilityCommon::Deg2RadF(-10.0f), 0.0f, 0.0f };
	followLocalCenterPos_ = PLAYER_HEAD_POS;
	startFollowLocalCenterPos_ = followLocalCenterPos_;
	goalFollowLocalCenterPos_ = Utility3D::VECTOR_ZERO;
	directionUpdate_ = [this]() {EndDirection(); };
}
