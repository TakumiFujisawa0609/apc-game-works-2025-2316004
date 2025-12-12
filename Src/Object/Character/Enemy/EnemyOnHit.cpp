#include "../pch.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Object/Common/Geometry/Sphere.h"
#include "../../../Object/Common/Geometry/Capsule.h"
#include "../../../Object/Common/Geometry/Line.h"
#include"../../../Object/Common/Geometry/Model.h"
#include "../Object/Character/Base/CharacterBase.h"
#include "../../../Utility/Utility3D.h"
#include "../Object/Character/Player/ActionController.h"
#include "../Base/CharacterOnHitBase.h"
#include "EnemyOnHit.h"
namespace
{
	using TAG_PRIORITY = ObjectBase::TAG_PRIORITY;
	using TAG = Collider::TAG;
}
EnemyOnHit::EnemyOnHit(CharacterBase& _chara, VECTOR& _movedPos, VECTOR& _moveDiff
	, ActionController& _action, std::map<ObjectBase::TAG_PRIORITY, std::shared_ptr<Collider>>& _colParam, Transform& _trans):
	CharacterOnHitBase(_chara, _movedPos, _moveDiff, _action, _colParam, _trans)
{
	//それぞれの当たった処理を格納する
	using TAG = Collider::TAG;
	colUpdates_ = {
		{ TAG::PLAYER1, [this](const std::weak_ptr<Collider> _hitCol) {CollChara(_hitCol); } },
		{ TAG::NML_ATK, [this](const std::weak_ptr<Collider> _hitCol) {CollNormalAttack(_hitCol); } },
		{ TAG::STAGE, [this](const std::weak_ptr<Collider>_hitCol) {CollStage(_hitCol); } }
	};
}

EnemyOnHit::~EnemyOnHit(void)
{
}

void EnemyOnHit::Init(void)
{
}

void EnemyOnHit::Load(void)
{
}

void EnemyOnHit::DrawDebug(void)
{
}


void EnemyOnHit::CollNormalAttack(const std::weak_ptr<Collider> _hitCol)
{
}

void EnemyOnHit::CollDuelSpecialAttack(const std::weak_ptr<Collider> _hitCol)
{
}

void EnemyOnHit::CollChara(const std::weak_ptr<Collider> _hitCol)
{
	////相手のタグをとる
	//CharacterBase& parentChara = _hitCol.lock()->GetParentCharacter();
	//std::set<Collider::TAG> tags = _hitCol.lock()->GetTags();

	////攻撃の場合は無視
	//if (std::ranges::any_of(tags, [](Collider::TAG tag) { return tag == Collider::TAG::NML_ATK || tag == Collider::TAG::ROAR_ATK; })
	//	|| charaObj_.GetCardAction() == ActionBase::CARD_ACT_TYPE::RUSH_ATK)
	//{
	//	return;
	//}
	//Geometry& myCap = colParam_[TAG_PRIORITY::BODY]->GetGeometry();
	//Geometry& hitCap = _hitCol.lock()->GetGeometry();
	////自分の座標
	//VECTOR myPos = charaObj_.GetTransform().pos;
	//VECTOR hitCharaPos = parentChara.GetTransform().pos;

	////お互いの距離をとる
	//float dis = static_cast<float>(Utility3D::Distance(myPos, hitCharaPos));
	//float minDist = myCap.GetRadius() + hitCap.GetRadius();
	////お互いの距離が離れていたら押し出さない
	//if (dis > minDist)return;

	////押し出し量の計算
	//float pushPow = abs(minDist - dis);

	////押し出す方向ベクトルの計算
	//VECTOR vec = Utility3D::GetMoveVec(parentChara.GetTransform().pos, charaObj_.GetTransform().pos);
	////Y成分はいらない
	//vec.y = 0.0f;

	////押し出し
	//movedPos_ = VAdd(moveDiff_, VScale(vec, pushPow));
}
