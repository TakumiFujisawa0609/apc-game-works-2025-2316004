#include "../pch.h"
#include "../Application.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Renderer/PixelRenderer.h"
#include "../Renderer/PixelMaterial.h"
#include "EnemyHpUI.h"

EnemyHpUI::EnemyHpUI(float& _hpPer):
	enemyHpPer_(_hpPer),
	resMng_(ResourceManager::GetInstance()),
	barCoverPos_({ static_cast<float>(Application::SCREEN_SIZE_X - 500.0f),20.0f }),
	barPos_({ 550.0f,23.0f })
{
	material_ = std::make_unique<PixelMaterial>(L"EnemyHpBarPS.cso",3);
	renderer_ = std::make_unique<PixelRenderer>(*material_);
}

EnemyHpUI::~EnemyHpUI(void)
{
}

void EnemyHpUI::Load(void)
{
	hpMask_ = resMng_.Load(ResourceManager::SRC::E_HP_BAR_MASK).handleId_;
	barCoverHandle_ = resMng_.Load(ResourceManager::SRC::E_HP_COVER).handleId_;
	barFrame_ = resMng_.Load(ResourceManager::SRC::E_HP_BAR_FRAME).handleId_;
}

void EnemyHpUI::Init(void)
{

	material_->AddTextureBuf(hpMask_);
	material_->AddConstBuf({ 0.0f,0.0f,0.0f,0.0f });	//ƒo[‚ÌF
	material_->AddConstBuf({ enemyHpPer_,0.0f,0.0f,0.0f });
	renderer_->MakeSquareVertex(barPos_, BAR_SIZE);
	//renderer_->SetPos(barCoverPos_);
	//renderer_->SetSize(BAR_COVER_SIZE);
}

void EnemyHpUI::Update(void)
{
	renderer_->SetPos(barPos_);
	material_->SetConstBuf(1, { enemyHpPer_,0.0f,0.0f,0.0f });
}

void EnemyHpUI::Draw(void)
{
	DrawExtendGraphF(barCoverPos_.x, barCoverPos_.y, barCoverPos_.x + BAR_COVER_SIZE.x, barCoverPos_.y + BAR_COVER_SIZE.y, barFrame_, true);
	renderer_->Draw(barPos_.x,barPos_.y);
	DrawExtendGraphF(barCoverPos_.x, barCoverPos_.y, barCoverPos_.x + BAR_COVER_SIZE.x, barCoverPos_.y + BAR_COVER_SIZE.y, barCoverHandle_, true);

	DrawFormatString(0, 128, 0x000000, L"barPos(%f,%f)", barPos_.x, barPos_.y);
}
