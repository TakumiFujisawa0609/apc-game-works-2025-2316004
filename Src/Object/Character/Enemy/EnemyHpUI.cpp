#include "../pch.h"
#include "../Application.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Common/Easing.h"
#include "../Renderer/PixelRenderer.h"
#include "../Renderer/PixelMaterial.h"
#include "EnemyHpUI.h"

EnemyHpUI::EnemyHpUI(float& _hpPer, float& _preHp) :
	HpUIBase(_hpPer, _preHp)
{
	initBarPos_ = BAR_POS;
	initCoverPos_ = BAR_COVER_POS;
	barPos_ = initBarPos_;
	barCoverPos_ = initCoverPos_;
	material_ = std::make_unique<PixelMaterial>(L"EnemyHpBarPS.cso",3);
	renderer_ = std::make_unique<PixelRenderer>(*material_);
}

EnemyHpUI::~EnemyHpUI(void)
{
	Vector2F vec = { static_cast<float>(Application::SCREEN_SIZE_X - 500.0f),20.0f };
}

void EnemyHpUI::Load(void)
{
	hpMask_ = resMng_.Load(ResourceManager::SRC::E_HP_BAR_MASK).handleId_;
	barCoverHandle_ = resMng_.Load(ResourceManager::SRC::E_HP_COVER).handleId_;
	barFrame_ = resMng_.Load(ResourceManager::SRC::E_HP_BAR_FRAME).handleId_;

}

void EnemyHpUI::Init(void)
{
	HpUIBase::Init();
	material_->AddTextureBuf(hpMask_);
	material_->AddConstBuf({ 0.2f, 0.8f, 1.0f,0.0f });	//バーの色(明るい青)
	material_->AddConstBuf({ 0.6f, 0.2f, 0.8f,0.0f });	//バーの色(紫)
	material_->AddConstBuf({ hpPer_,0.0f,0.0f,0.0f });
	renderer_->MakeSquareVertex(barCoverPos_, BAR_SIZE);
	//renderer_->SetPos(barCoverPos_);
	//renderer_->SetSize(BAR_COVER_SIZE);
}

void EnemyHpUI::Update(void)
{
	HpUIBase::Update();
	renderer_->SetPos(barPos_);
	material_->SetConstBuf(2, { hpPer_,preHp_,0.0f,0.0f });
}

void EnemyHpUI::Draw(void)
{
	DrawExtendGraphF(barCoverPos_.x, barCoverPos_.y, barCoverPos_.x + BAR_COVER_SIZE.x, barCoverPos_.y + BAR_COVER_SIZE.y, barFrame_, true);
	renderer_->Draw(barPos_.x,barPos_.y);
	DrawExtendGraphF(barCoverPos_.x, barCoverPos_.y, barCoverPos_.x + BAR_COVER_SIZE.x, barCoverPos_.y + BAR_COVER_SIZE.y, barCoverHandle_, true);

	DrawFormatString(0, 128, 0x000000, L"barPos(%f,%f)", barPos_.x, barPos_.y);
}


