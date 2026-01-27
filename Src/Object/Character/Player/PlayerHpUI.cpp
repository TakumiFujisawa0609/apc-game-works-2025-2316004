#include "../pch.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Renderer/PixelRenderer.h"
#include "../Renderer/PixelMaterial.h"
#include "PlayerHpUI.h"

PlayerHpUI::PlayerHpUI(void)
{
	per_ = 1.0f;

	initBarPos_ = BAR_POS;
	initCoverPos_ =  BAR_COVER_POS;
	barPos_ = initBarPos_;
	barCoverPos_ = initCoverPos_;
	material_ = std::make_unique<PixelMaterial>(L"LineHpBarPS.cso", LINE_CONST_BUF);
	renderer_ = std::make_unique<PixelRenderer>(*material_);
}

PlayerHpUI::~PlayerHpUI(void)
{
}

void PlayerHpUI::Load(void)
{
	hpMask_ = resMng_.Load(ResourceManager::SRC::E_HP_BAR_MASK).handleId_;
	barCoverHandle_ = resMng_.Load(ResourceManager::SRC::E_HP_COVER).handleId_;
	barFrame_ = resMng_.Load(ResourceManager::SRC::E_HP_BAR_FRAME).handleId_;

}

void PlayerHpUI::Init(void)
{
	HpUIBase::Init();
	material_->AddTextureBuf(hpMask_);
	material_->AddConstBuf(BAR_LIGHT_GREEN);
	material_->AddConstBuf(BAR_BLUE);
	material_->AddConstBuf({ hpData_.hpPer,0.0f,0.0f,0.0f });
	renderer_->MakeSquareVertex(barCoverPos_, BAR_SIZE);
}

void PlayerHpUI::Update(void)
{
	HpUIBase::Update();
	renderer_->SetPos(barPos_);
	material_->SetConstBuf(2, { hpData_.hpPer,hpData_.preHpPer,0.0f,0.0f });
}

void PlayerHpUI::Draw(void)
{
	DrawExtendGraphF(barCoverPos_.x, barCoverPos_.y, barCoverPos_.x + BAR_COVER_SIZE.x, barCoverPos_.y + BAR_COVER_SIZE.y, barFrame_, true);
	renderer_->Draw(barPos_.x, barPos_.y);
	DrawExtendGraphF(barCoverPos_.x, barCoverPos_.y, barCoverPos_.x + BAR_COVER_SIZE.x, barCoverPos_.y + BAR_COVER_SIZE.y, barCoverHandle_, true);
}

