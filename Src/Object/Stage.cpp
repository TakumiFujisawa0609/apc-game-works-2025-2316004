#include "../Utility/Utility3D.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Renderer/ModelMaterial.h"
#include "../Renderer/ModelRenderer.h"
#include"../Object/Common/Geometry/Model.h"
#include "Stage.h"

Stage::Stage(void)
{
}

Stage::~Stage(void)
{
}

void Stage::Load(void)
{
}

void Stage::Init(void)
{
	trans_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::STAGE));
	trans_.pos = Utility3D::VECTOR_ZERO;
	trans_.quaRotLocal =
		Quaternion::Euler({ 0.0f,0.0f, 0.0f });

	trans_.scl = { STAGE_SCL ,0.1f ,STAGE_SCL };

	//ƒJƒvƒZƒ‹
	tag_ = Collider::TAG::STAGE;

	std::unique_ptr<Geometry>geo = std::make_unique<Model>(trans_.pos, trans_.quaRot, trans_.modelId);
	MakeCollider(TAG_PRIORITY::BODY,{ tag_ }, std::move(geo), { Collider::TAG::NML_ATK,Collider::TAG::ROAR_ATK });

	material_=std::make_unique<ModelMaterial>(
		L"StageVS.cso", 1,
		L"StagePS.cso", 0
	);
	material_->AddConstBufVS({ 300.0f,0.0f,0.0f,0.0f });
	renderer_ = std::make_unique<ModelRenderer>(trans_.modelId,*material_);

	trans_.Update();


}

void Stage::Update(void)
{
	trans_.Update();
}

void Stage::Draw(void)
{
	//MV1DrawModel(trans_.modelId);
	renderer_->Draw();

}

void Stage::OnHit(const std::weak_ptr<Collider> _hitCol)
{
}
