#include "../pch.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Game/CharacterManager.h"
#include "../Manager/Generic/ButtonUIManager.h"
#include "../Renderer/PixelMaterial.h"
#include "../Renderer/PixelRenderer.h"
#include "DirectionUI.h"

DirectionUI::DirectionUI(void):
	resMng_(ResourceManager::GetInstance()),
	skipGaugePer_(0.0f)
{
}

DirectionUI::~DirectionUI(void)
{
}

void DirectionUI::Load(void)
{

	//何回かフォントを使うと一定の大きさだけ大きくなる不具合が発生しているので別で作る
	fontHandle_ = CreateFontToHandle(FontManager::FONT_APRIL_GOTHIC.c_str(), FONT_SIZE, 0);
	//集中線画像のロード
	intensiveLineImg_1 = resMng_.Load(ResourceManager::SRC::INTENSIVE_LINE_1).handleId_;
	intensiveLineImg_2 = resMng_.Load(ResourceManager::SRC::INTENSIVE_LINE_2).handleId_;

	imgSkipButtom_ = resMng_.Load(ResourceManager::SRC::SKIP_BUTTOM).handleId_;
	imgSkipButtomMask_ = resMng_.Load(ResourceManager::SRC::SKIP_BUTTOM_MASK).handleId_;

}

void DirectionUI::Init(void)
{
	skipArcGaugeMaterial_ = std::make_unique<PixelMaterial>(L"ArcHpBarPS.cso", SKIP_GAUGE_CONST_BUFF_SIZE);
	skipArcGaugeRenderer_ = std::make_unique<PixelRenderer>(*skipArcGaugeMaterial_);
	skipArcGaugeMaterial_->AddTextureBuf(imgSkipButtomMask_);
	skipArcGaugeMaterial_->AddConstBuf({ 1.0f,0.0f,0.0f,1.0f });
	skipArcGaugeMaterial_->AddConstBuf({ 0.0f,0.0f,0.0f,0.0f });
	skipArcGaugeRenderer_->MakeSquareVertexFromCenter(SKIP_BTN_POS, SKIP_BTN_SIZE);
}

void DirectionUI::Update(void)
{
	//割合、どこから始めるかをセット
	skipArcGaugeMaterial_->SetConstBuf(SKIP_GAUGE_PER_CONSTBUF_NUM, { 0.0f,skipGaugePer_,1.0f,0.0f });
	UpdateIntensiveLineAnim();
}

void DirectionUI::Draw(void)
{

	skipArcGaugeRenderer_->Draw();
	const Vector2F leftTop = SKIP_BTN_POS - (SKIP_BTN_SIZE / 2.0f);
	const Vector2F rightDown = SKIP_BTN_POS + (SKIP_BTN_SIZE / 2.0f);
	ButtonUIManager::GetInstance().Draw(ButtonUIManager::BTN_UI_TYPE::Y_BUTTON_COL_PUSH, SKIP_BTN_POS, SKIP_BTN_SIZE.x/2.0f);
	DrawStringFToHandle(leftTop.x + SKIP_BTN_SIZE.x, leftTop.y + SKIP_BTN_STR_OFFSET_Y, L"ボタン長押しでスキップ", 0x000000, fontHandle_);
	if (CharacterManager::GetInstance().GetIsEnemyRoar())
	{
		//集中線描画
		DrawGraph(0, 0, intensiveLineAnimImg_, true);
	}

}

void DirectionUI::SetSkipGaugePer(const float _skipPer)
{
	skipGaugePer_ = _skipPer;
}

void DirectionUI::UpdateIntensiveLineAnim(void)
{
	if (!CharacterManager::GetInstance().GetIsEnemyRoar())return;
	intensiveLineAnimFrame_++;
	if (intensiveLineAnimFrame_ >= INTENSIVE_LINE_ANIM_SPEED)
	{
		intensiveLineAnimFrame_ = 0;
	}
	intensiveLineAnimImg_ = (intensiveLineAnimFrame_ % INTENSIVE_LINE_ANIM_SPEED / 2 == 0) ? intensiveLineImg_1 : intensiveLineImg_2;
}
