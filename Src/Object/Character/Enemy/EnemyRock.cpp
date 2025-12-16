#include "EnemyRock.h"
#include "../pch.h"
#include "../Utility/Utility3D.h"
#include "../Object/Common/Collider.h"
#include "../Object/Common/Geometry/Sphere.h"
#include "../Manager/Resource/ResourceManager.h"
#include "EnemyRock.h"

EnemyRock::EnemyRock(int& _num, VECTOR& _startPos):
	num_(_num),
	startPos_(_startPos),
	distance_(0.0f)
{

}

EnemyRock::~EnemyRock(void)
{
}

void EnemyRock::Load(void)
{
	trans_.modelId = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::SPHERE_ROCK);
}

void EnemyRock::Init(void)
{
	float angle = ARROUND_PER_RAD * num_;

	goalPos_.x = startPos_.x + sin(angle) * DISTANCE_RADIUS;
	goalPos_.y = 0.0f;
	goalPos_.z = startPos_.z - cos(angle) * DISTANCE_RADIUS;

	tag_ = Collider::TAG::ROCK;
	velocity_.y = sqrtf(2.0f * GRAVITY * JUMP_HEIGHT);
	jumpPow_.y = velocity_.y;

	isDamaged_ = false;

	noneHitTag_.emplace(Collider::TAG::STAGE);
	noneHitTag_.emplace(Collider::TAG::CAMERA);
	noneHitTag_.emplace(Collider::TAG::ENEMY1);
	noneHitTag_.emplace(Collider::TAG::NML_ATK);

	std::unique_ptr<Geometry> geo = std::make_unique<Sphere>(trans_.pos, ROCK_COL_RADIUS);
	MakeCollider(TAG_PRIORITY::ROCK_SPHERE, {tag_ }, std::move(geo),noneHitTag_);

	trans_.pos = startPos_;

	

}

void EnemyRock::Update(void)
{
	//ê∂ë∂ÇµÇƒÇ¢Ç»ÇØÇÍÇŒèàóùÇµÇ»Ç¢
	if (!isAlive_)return;

	velocity_.y -= GRAVITY;
	jumpPow_.y = velocity_.y;
	
	VECTOR vec = VNorm(VSub(goalPos_, startPos_));
	trans_.pos = VAdd(trans_.pos, VScale(vec, 20.0f));
	trans_.pos = VAdd(trans_.pos, jumpPow_);

	trans_.Update();

}

void EnemyRock::Draw(void)
{
	//ê∂ë∂ÇµÇƒÇ¢Ç»ÇØÇÍÇŒèàóùÇµÇ»Ç¢
	if (!isAlive_)return;
	//DrawSphere3D(trans_.pos, 150.0f, 5, 0xff0000, 0xff0000, true);
	MV1DrawModel(trans_.modelId);

	//DrawFormatString(200, 200 + num_ * 20, 0xffffff, L"RockNum:%f,%f,%f", trans_.pos.x, trans_.pos.y, trans_.pos.z);
}

void EnemyRock::OnHit(const std::weak_ptr<Collider> _hitCol)
{
}
