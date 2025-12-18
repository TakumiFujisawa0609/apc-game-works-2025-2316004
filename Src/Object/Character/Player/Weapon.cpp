#include "../pch.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Utility/ModelFrameUtility.h"
#include "Weapon.h"

Weapon::Weapon(void)
{

}

Weapon::~Weapon(void)
{
}

void Weapon::Load(void)
{
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::KEY_BLADE));
}

void Weapon::Init(void)
{
	//武器の相対位置初期化
	localPos_ = { 0.0f,0.0f,0.0f };
	//武器の相対回転初期化
	localRot_ = { 0.0f,0.0f,0.0f };

	//モデル情報初期化
	trans_.scl = { WEAPON_SCL,WEAPON_SCL, WEAPON_SCL };
}

void Weapon::Update(void)
{
	if (InputManager::GetInstance().IsNew(KEY_INPUT_UP))
	{
		localRot_.y+=0.1f;
	}

	// 対象フレームの位置にtargetを配置し、
	// 対象フレームの回転に加え、指定した相対座標・回転を加える
	ModelFrameUtility::SetFrameWorldMatrix(
		*targetTrans_, followFrameNo_, trans_,
		localPos_, localRot_
	);

	trans_.quaRot = Quaternion::GetRotation(trans_.matRot);
	trans_.Update();
}

void Weapon::Draw(void)
{
	MV1DrawModel(trans_.modelId);
}

void Weapon::SetTargetAndFrameNo(Transform* _targetTrans, int _frameNo)
{
	targetTrans_ = _targetTrans;
	followFrameNo_ = _frameNo;
}
