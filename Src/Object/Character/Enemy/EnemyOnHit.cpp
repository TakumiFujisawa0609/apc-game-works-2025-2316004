#include "../pch.h"
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
}

void EnemyOnHit::Load(void)
{
}

void EnemyOnHit::DrawDebug(void)
{
}


void EnemyOnHit::CollDuelSpecialAttack(const std::weak_ptr<Collider> _hitCol)
{
}
