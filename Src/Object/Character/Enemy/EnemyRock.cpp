#include "EnemyRock.h"
#include "../pch.h"
#include "../Utility/Utility3D.h"
#include "../Manager/Resource/ResourceManager.h"
#include "EnemyRock.h"

EnemyRock::EnemyRock(int& _num,VECTOR& _startPos):
	startPos_(_startPos)
{
	float angle = ARROUND_PER_RAD * _num;

	goalPos_.x = _startPos.x + sin(angle) * DISTANCE_RADIUS;
	goalPos_.y = 0.0f;
	goalPos_.z = _startPos.z - cos(angle) * DISTANCE_RADIUS;

	tag_ = Collider::TAG::ROCK;
	velocity_.y = sqrtf(2.0f * GRAVITY * JUMP_HEIGHT);
	jumpPow_.y = velocity_.y;

	trans_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::SPHERE_ROCK));
	trans_.pos = _startPos;
}

EnemyRock::~EnemyRock(void)
{
}

void EnemyRock::Load(void)
{
}

void EnemyRock::Init(void)
{


}

void EnemyRock::Update(void)
{
	velocity_.y -= GRAVITY;
	jumpPow_.y = velocity_.y;
	
	VECTOR vec = VNorm(VSub(goalPos_, startPos_));
	trans_.pos = VAdd(trans_.pos, VScale(vec, 20.0f));
	trans_.pos = VAdd(trans_.pos, jumpPow_);

	trans_.Update();

}

void EnemyRock::Draw(void)
{
	//DrawSphere3D(trans_.pos, 150.0f, 5, 0xff0000, 0xff0000, true);
	MV1DrawModel(trans_.modelId);
}

void EnemyRock::OnHit(const std::weak_ptr<Collider> _hitCol)
{
}
