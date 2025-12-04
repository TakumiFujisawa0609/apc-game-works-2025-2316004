#include "../pch.h"
#include "../Manager/Generic/Camera.h"
#include "../Utility/Utility3D.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Resource/ResourceManager.h"

#include "DrawUI3D.h"

DrawUI3D::DrawUI3D(std::weak_ptr<Camera> _camera):
	camera_(_camera)
{
}

DrawUI3D::~DrawUI3D(void)
{
}

void DrawUI3D::Load(void)
{
	testTrans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::CARD_MDL));
	testTrans_.pos = { 0.0f,0.0,0.0f };
	testTrans_.pos = Utility3D::AddPosRotate(camera_.lock()->GetPos(), camera_.lock()->GetQuaRot(), {0.0f,0.0f,100.0f});
	testTrans_.quaRot = camera_.lock()->GetQuaRot();
}

void DrawUI3D::Init(void)
{
	testTrans_.quaRot = camera_.lock()->GetQuaRot();
	testTrans_.Update();
}

void DrawUI3D::Update(void)
{
	VECTOR pos = ConvScreenPosToWorldPos({ 0.0f,0.0f,0.99f });
	testTrans_.pos = pos;
	testTrans_.quaRot = camera_.lock()->GetQuaRot();
	testTrans_.Update();
}

void DrawUI3D::Draw(void)
{
	MV1DrawModel(testTrans_.modelId);
}

