#include "../pch.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Renderer/PixelRenderer.h"
#include "../Renderer/PixelMaterial.h"
#include "PlayerHpUI.h"

PlayerHpUI::PlayerHpUI(float& _hpPer, float& _preHp):
HpUIBase(_hpPer,_preHp)
{
	initBarPos_ = {};
	initCoverPos_ = {};
	barPos_ = initBarPos_;
	barCoverPos_ = initCoverPos_;
	per_ = 1.0f;
	material_ = std::make_unique<PixelMaterial>(L"PlayerHpBar.cso", 2);
	renderer_ = std::make_unique<PixelRenderer>(*material_);
}

PlayerHpUI::~PlayerHpUI(void)
{
}

void PlayerHpUI::Load(void)
{
	barFrame_ = resMng_.Load(ResourceManager::SRC::P_HP_ARCBAR_FRAME).handleId_;
	radiusHpBarMaskImg_ = resMng_.Load(ResourceManager::SRC::P_HP_ARCBAR_MASK).handleId_;
	lineHpBarMaskImg_ = resMng_.Load(ResourceManager::SRC::P_HP_LINEBAR_MASK).handleId_;
}

void PlayerHpUI::Init(void)
{
	HpUIBase::Init();
	material_->AddTextureBuf(radiusHpBarMaskImg_);
	material_->AddConstBuf({ 0.0f,1.0f,0.0f,1.0f });
	material_->AddConstBuf({ hpPer_,preHp_,0.75f,0.0f });
	renderer_->MakeSquareVertex(barPos_, GAUGE_SIZE);
}

void PlayerHpUI::Update(void)
{
	HpUIBase::Update();
	if (InputManager::GetInstance().IsNew(KEY_INPUT_UP))
	{
		per_ -= 0.01f;
	}

	material_->SetConstBuf(1, { hpPer_,preHp_,0.0f,0.0f });
}

void PlayerHpUI::Draw(void)
{
	DrawExtendGraphF(barCoverPos_.x, barCoverPos_.y, barCoverPos_.x + GAUGE_SIZE.x, barCoverPos_.y + GAUGE_SIZE.y, barFrame_, true);
	renderer_->Draw();
}

