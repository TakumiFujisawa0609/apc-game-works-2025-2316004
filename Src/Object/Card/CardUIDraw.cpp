#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Renderer/PixelMaterial.h"
#include "../Renderer/PixelRenderer.h"
#include "CardUIDraw.h"

CardUIDraw::CardUIDraw(int& _typeImg,Vector2F& _centerPos, float& _scl):
	typeImg_(_typeImg),
	centerPos_(_centerPos),
	scl_(_scl)
{
}

CardUIDraw::~CardUIDraw(void)
{
}
void CardUIDraw::Load(void)
{
	trans_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::CARD));
}

void CardUIDraw::Init(void)
{
	//画像サイズ取得
	GetGraphSizeF(typeImg_, &size_.x, &size_.y);
	halfSize_ = size_ / 2.0f;
	//左上の座標
	rightTopPos_ = centerPos_ - halfSize_ * scl_;
	//右下の座標
	leftDownPos_ = centerPos_ + halfSize_ * scl_;

	Vector2 rightTopPos = {static_cast<int>(rightTopPos_.x),static_cast<int>(rightTopPos_.y)};
	Vector2 size = {};
	size = { static_cast<int>(size_.x),static_cast<int>(size_.y) };


	trans_.pos = CARD_INIT_POS;
	trans_.quaRot = Quaternion();
	trans_.scl = { CARD_SCL,CARD_SCL,CARD_SCL };
	trans_.quaRotLocal =
		Quaternion::Euler({ 0.0f,0.0f,0.0f });

	pixelMaterial_ = std::make_unique<PixelMaterial>(L"CardPS.cso", 1);
	pixelMaterial_->AddTextureBuf(typeImg_);
	pixelMaterial_->AddConstBuf({ 1.0f,1.0f, 1.0f,1.0f });
	pixelRenderer_ = std::make_unique<PixelRenderer>(*pixelMaterial_);
	pixelRenderer_->MakeSquareVertex(rightTopPos_, size_);
	

	trans_.Update();
}
void CardUIDraw::Update(void)
{
	trans_.Update();
}
void CardUIDraw::Draw(void)
{
	//画像サイズ取得
	GetGraphSizeF(typeImg_, &size_.x, &size_.y);

	halfSize_ = size_ / 2.0f;
	//左上の座標
	Vector2F rightTopPos = centerPos_ - halfSize_ * scl_;
	//右下の座標
	Vector2F leftDownPos = centerPos_ + halfSize_ * scl_;

	//DrawExtendGraphF(
	//	rightTopPos.x, rightTopPos.y,
	//	leftDownPos.x, leftDownPos.y,
	//	typeImg_,
	//	true
	//);

	//ピクセルレンダラー座標更新
	pixelRenderer_->SetSize(size_ * scl_);
	pixelRenderer_->Draw(rightTopPos.x, rightTopPos.y);
}

void CardUIDraw::DrawModel(void)
{
	MV1DrawModel(trans_.modelId);
}
