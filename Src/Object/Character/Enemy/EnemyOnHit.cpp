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

EnemyOnHit::EnemyOnHit(CharacterBase& _chara, VECTOR& _movedPos, VECTOR& _moveDiff
	, ActionController& _action, std::map<ObjectBase::TAG_PRIORITY, std::shared_ptr<Collider>>& _colParam, Transform& _trans):
	CharacterOnHitBase(_chara, _movedPos, _moveDiff, _action, _colParam, _trans)
{
}

EnemyOnHit::~EnemyOnHit(void)
{
}

void EnemyOnHit::Init(void)
{
	//ÇªÇÍÇºÇÍÇÃìñÇΩÇ¡ÇΩèàóùÇäiî[Ç∑ÇÈ
	using TAG = Collider::TAG;
	colUpdates_ = {
		{ TAG::PLAYER1, [this](const std::weak_ptr<Collider> _hitCol) {CollChara(_hitCol); } },
		{ TAG::NML_ATK, [this](const std::weak_ptr<Collider> _hitCol) {CollNormalAttack(_hitCol); } },
		{ TAG::STAGE, [this](const std::weak_ptr<Collider>_hitCol) {CollStage(_hitCol); } }
	};

	hitPoint_ = {};

	moveDiff_ = Utility3D::VECTOR_ZERO;
	movedPos_ = Utility3D::VECTOR_ZERO;
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

}
