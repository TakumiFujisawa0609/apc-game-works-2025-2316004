#include "../../Common/Quaternion.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../../Utility/UtilityCommon.h"
#include "../../Utility/Utility3D.h"
#include "SkyDome.h"

SkyDome::SkyDome(void)
{
	state_ = STATE::NONE;
}

void SkyDome::Load(void)
{
	//リソース読み込み
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::SKY_DOME));
}

void SkyDome::Init(void)
{
	// モデル制御の基本情報
	transform_.scl = SCALES;
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		UtilityCommon::Deg2RadF(180.0f),
		0.0f);
	transform_.quaRotLocal = Quaternion();

	//座標位置をずらす
	transform_.pos = { 
		0.0f, 
		0.0f, 
		0.0f };
	transform_.Update();

	//Zバッファを無効
	MV1SetUseZBuffer(transform_.modelId, false);
	MV1SetWriteZBuffer(transform_.modelId, false);
}

void SkyDome::Update(void)
{
	Quaternion yRot = Quaternion::AngleAxis(UtilityCommon::Deg2RadF(ROT_SPEED), Utility3D::AXIS_Y);
	transform_.quaRot = Quaternion::Mult(transform_.quaRot, yRot);
	transform_.Update();
}

void SkyDome::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}