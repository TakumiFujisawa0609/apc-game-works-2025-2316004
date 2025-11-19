#include "../pch.h"
#include "../Manager/Resource/ResourceManager.h"
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
}

void CardUIDraw::Init(void)
{
}
void CardUIDraw::Update(void)
{
}
void CardUIDraw::Draw(void)
{
	//画像サイズ取得
	GetGraphSizeF(typeImg_, &size_.x, &size_.y);

	size_ /= 2.0f;
	//左上の座標
	Vector2F rightTopPos = centerPos_ - size_ * scl_;
	//右下の座標
	Vector2F leftDownPos = centerPos_ + size_ * scl_;

	DrawExtendGraphF(
		rightTopPos.x, rightTopPos.y,
		leftDownPos.x, leftDownPos.y,
		typeImg_,
		true
	);
}