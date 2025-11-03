#include "../Manager/Resource/ResourceManager.h"
#include "../Utility/Utility3D.h"
#include"../Object/Common/Geometry/Model.h"
#include "Stage.h"

void Stage::Load(void)
{
}

void Stage::Init(void)
{
	trans_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::STAGE));
	trans_.pos = Utility3D::VECTOR_ZERO;
	trans_.quaRotLocal =
		Quaternion::Euler({ 0.0f,0.0f, 0.0f });

	trans_.scl = { STAGE_SCL ,1.0f ,STAGE_SCL };

	//ƒJƒvƒZƒ‹
	std::unique_ptr<Geometry>geo = std::make_unique<Model>(trans_.pos, trans_.quaRot, trans_.modelId);
	MakeCollider({ tag_ }, std::move(geo),{Collider::TAG::NML_ATK});

	trans_.Update();
}

void Stage::Update(void)
{
	trans_.Update();
}

void Stage::Draw(void)
{
	MV1DrawModel(trans_.modelId);
}

void Stage::OnHit(const std::weak_ptr<Collider> _hitCol)
{
}
