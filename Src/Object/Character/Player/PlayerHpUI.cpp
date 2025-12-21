#include "../pch.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Renderer/PixelRenderer.h"
#include "../Renderer/PixelMaterial.h"
#include "PlayerHpUI.h"

PlayerHpUI::PlayerHpUI(float& _hpPer, float& _preHp):
HpUIBase(_hpPer,_preHp)
{
	per_ = 1.0f;

	initBarPos_ = BAR_POS;
	initCoverPos_ =  BAR_COVER_POS;
	barPos_ = initBarPos_;
	barCoverPos_ = initCoverPos_;
	material_ = std::make_unique<PixelMaterial>(L"LineHpBarPS.cso", LINE_CONST_BUF);
	renderer_ = std::make_unique<PixelRenderer>(*material_);

	//lineMaterial_ = std::make_unique<PixelMaterial>(L"LineHpBarPS.cso", LINE_CONST_BUF);
	//lineRenderer_ = std::make_unique<PixelRenderer>(*lineMaterial_);
}

PlayerHpUI::~PlayerHpUI(void)
{
}

void PlayerHpUI::Load(void)
{
	//barFrame_ = resMng_.Load(ResourceManager::SRC::P_HP_ARCBAR_FRAME).handleId_;
	//radiusHpBarMaskImg_ = resMng_.Load(ResourceManager::SRC::P_HP_ARCBAR_MASK).handleId_;
	//lineHpBarMaskImg_ = resMng_.Load(ResourceManager::SRC::P_HP_LINEBAR_MASK).handleId_;

	//arcOutLineImg_ = resMng_.Load(ResourceManager::SRC::P_HP_ARC_OUTLINE).handleId_;
	//lineHpFrameImg_ = resMng_.Load(ResourceManager::SRC::P_HP_LINE_OUT_LINE).handleId_;

	hpMask_ = resMng_.Load(ResourceManager::SRC::E_HP_BAR_MASK).handleId_;
	barCoverHandle_ = resMng_.Load(ResourceManager::SRC::E_HP_COVER).handleId_;
	barFrame_ = resMng_.Load(ResourceManager::SRC::E_HP_BAR_FRAME).handleId_;

}

void PlayerHpUI::Init(void)
{
	HpUIBase::Init();
	material_->AddTextureBuf(hpMask_);
	material_->AddConstBuf({ 0.2f, 0.6f, 1.0f,0.0f });	//バーの色(明るい青)
	material_->AddConstBuf({ 0.2f, 1.0f, 0.4f,0.0f });	//バーの色(紫)
	material_->AddConstBuf({ hpPer_,0.0f,0.0f,0.0f });
	renderer_->MakeSquareVertex(barCoverPos_, BAR_SIZE);

	//lineMaterial_->AddTextureBuf(lineHpBarMaskImg_);
	//lineMaterial_->AddConstBuf({ 0.0f, 1.0f, 0.0f,0.0f });	//バーの色(緑)
	//lineMaterial_->AddConstBuf({ 0.0f, 1.0f, 0.0f,0.0f });	//バーの色(紫)
	//lineMaterial_->AddConstBuf({ 1.0f,1.0f,0.0f,0.0f });
	//lineRenderer_->MakeSquareVertex({ barPos_.x + (GAUGE_SIZE.x / 2.0f),barPos_.y }, { GAUGE_SIZE.x,LINE_SIZE_Y });
}

void PlayerHpUI::Update(void)
{
	HpUIBase::Update();
	renderer_->SetPos(barPos_);
	material_->SetConstBuf(2, { hpPer_,preHp_,0.0f,0.0f });




	//renderer_->SetPos(barPos_);
	//lineRenderer_->SetPos({ barPos_.x + (GAUGE_SIZE / 2.0f,barPos_.y) });

	//if (InputManager::GetInstance().IsNew(KEY_INPUT_UP))
	//{
	//	per_ -= 0.01f;
	//}

	//if (hpPer_ > ARC_HP_PER)
	//{
	//	// LINEは常にMAX表示
	//	float linePer = (hpPer_ - ARC_HP_PER)/ LINE_HP_PER;
	//	float linePreHp= (preHp_ - ARC_HP_PER) / LINE_HP_PER;
	//	lineMaterial_->SetConstBuf(2, { linePer, linePreHp, 0.0f, 0.0f });

	//}
	//else
	//{
	//	// LINE部分のみ減っていく
	//	float arcPer = hpPer_ / ARC_HP_PER;


	//	// ARCは0
	//	material_->SetConstBuf(1, { arcPer, preHp_, 0.75f, 0.0f });
	//}
	//lineMaterial_->SetConstBuf(2, { hpPer_,preHp_ ,0.0f,0.0f });

}

void PlayerHpUI::Draw(void)
{
	DrawExtendGraphF(barCoverPos_.x, barCoverPos_.y, barCoverPos_.x + BAR_COVER_SIZE.x, barCoverPos_.y + BAR_COVER_SIZE.y, barFrame_, true);
	renderer_->Draw(barPos_.x, barPos_.y);
	DrawExtendGraphF(barCoverPos_.x, barCoverPos_.y, barCoverPos_.x + BAR_COVER_SIZE.x, barCoverPos_.y + BAR_COVER_SIZE.y, barCoverHandle_, true);


	//DrawExtendGraphF(barCoverPos_.x, barCoverPos_.y, barCoverPos_.x + GAUGE_SIZE.x, barCoverPos_.y + GAUGE_SIZE.y, barFrame_, true);
	//lineRenderer_->Draw();
	//renderer_->Draw();
	//DrawExtendGraphF(barCoverPos_.x, barCoverPos_.y, barCoverPos_.x + GAUGE_SIZE.x , barCoverPos_.y + GAUGE_SIZE.y, arcOutLineImg_, true);
	//DrawExtendGraphF(barPos_.x + (GAUGE_SIZE.x / 2.0f), barCoverPos_.y, barPos_.x + (GAUGE_SIZE.x / 2.0f) + GAUGE_SIZE.x , barCoverPos_.y + LINE_SIZE_Y, lineHpFrameImg_, true);
}

