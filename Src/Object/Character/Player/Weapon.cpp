#include "../pch.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Utility/Utility3D.h"
#include "../Utility/UtilityCommon.h"
#include "../Utility/ModelFrameUtility.h"
#include "../Object/Common/EffectController.h"
#include "../Object/Common/Geometry/Geometry.h"
#include "../Object/Common/Geometry/Capsule.h"
#include "Weapon.h"

Weapon::Weapon(void)
{
	effect_ = std::make_unique<EffectController>();
}

Weapon::~Weapon(void)
{
}

void Weapon::Load(void)
{
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::KEY_BLADE));
	effect_->Add(ResourceManager::GetInstance().Load(ResourceManager::SRC::KEY_BLADE_HIT_EFF).handleId_,
		EffectController::EFF_TYPE::KEY_BLADE_HIT);

}

void Weapon::Init(void)
{
	//武器の相対位置初期化
	localPos_ = { 0.0f,0.0f,0.0f };
	//武器の相対回転初期化
	localRot_ = { 0.0f,UtilityCommon::Deg2RadF(LOCAL_ANGLE_DEG.y),UtilityCommon::Deg2RadF(LOCAL_ANGLE_DEG.z) };

	tag_ = Collider::TAG::PLAYER1;

	//MakeWeaponCollider();

	//モデル情報初期化
	trans_.scl = { WEAPON_SCL,WEAPON_SCL, WEAPON_SCL };
}

void Weapon::Update(void)
{
	if (InputManager::GetInstance().IsNew(KEY_INPUT_UP))
	{
		localRot_.z+=0.1f;
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
	DrawFormatString(0, 0, GetColor(255, 255, 255), L"localRot(%f,%f,%f)",localRot_.x, localRot_.y, localRot_.z);

	if (collider_.size()==0)return;
	collider_[TAG_PRIORITY::ATK_SPHERE]->GetGeometry().Draw();
}

void Weapon::SetTargetAndFrameNo(Transform* _targetTrans, int _frameNo)
{
	targetTrans_ = _targetTrans;
	followFrameNo_ = _frameNo;
}

void Weapon::MakeWeaponCollider(void)
{
	//当たり判定が存在したら削除する
	//if (IsAliveCollider(Collider::TAG::PLAYER1, Collider::TAG::NML_ATK))return;
	//ダメージフラグ初期化
	isDamage_ = false;

	//当たり判定をしないタグ設定
	noneHitTag_.emplace(Collider::TAG::PLAYER1);
	noneHitTag_.emplace(Collider::TAG::CAMERA);
	noneHitTag_.emplace(Collider::TAG::STAGE);
	noneHitTag_.emplace(Collider::TAG::JUMP_ATK);
	noneHitTag_.emplace(Collider::TAG::ROAR_ATK);
	//カプセル形状作成
	const VECTOR CAP_TOP = { 0.0f,CAPSULE_COL_HEIGHT ,0.0f };
	std::unique_ptr<Capsule> geo = std::make_unique<Capsule>(trans_.pos, trans_.quaRot, Utility3D::VECTOR_ZERO, CAP_TOP, CAPSULE_COL_RADIUS);
	MakeCollider(TAG_PRIORITY::ATK_SPHERE, { Collider::TAG::PLAYER1,Collider::TAG::NML_ATK }, std::move(geo), noneHitTag_);
	tagPrioritys_.emplace_back(TAG_PRIORITY::ATK_SPHERE);
}

void Weapon::DeleteWeaponCollider(void)
{
	DeleteCollider(TAG_PRIORITY::ATK_SPHERE);
}

void Weapon::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	if (isDamage_)return;
	//エフェクト再生
	VECTOR bladeFramePos = MV1GetFramePosition(trans_.modelId, EFFECT_PLAY_FRAME_NO);
	effect_->Play(EffectController::EFF_TYPE::KEY_BLADE_HIT, bladeFramePos, {}, { EFFECT_PLAY_SCL,EFFECT_PLAY_SCL,EFFECT_PLAY_SCL });
}
