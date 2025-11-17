#include "../pch.h"
#include "../Manager/Resource/ResourceManager.h"
#include "CardUI.h"

CardUI::CardUI(Vector2F& _centerPos, float& _scl):
	centerPos_(_centerPos),
	scl_(_scl)
{
}

CardUI::~CardUI(void)
{
}

void CardUI::Draw(void)
{
	//constexpr double NUM_SCL = 0.18;
	////カードの描画
	//DrawRotaGraphF(_card.cardPos_.x, _card.cardPos_.y, _card.cardScl_, 0.0f, _card.typeImg, true);

	//int num = _card.status.pow_ - 1;
	//if (num == -1) { num = 9; }
	//DrawRotaGraphF(_card.numPos_.x, _card.numPos_.y, _card.cardScl_ * NUM_SCL, 0.0f, cardNoImg_[num], true);



	size_.x= GetGraphSizeF(typeImg_, &size_.x, &size_.y);
	size_.y= GetGraphSizeF(typeImg_, &size_.x, &size_.y);

	//左上の座標
	Vector2F rightPos = centerPos_ - size_ * scl_;
	//右下の座標
	Vector2F leftPos = centerPos_ + size_ * scl_;

	DrawExtendGraphF(
		0, 0,
		static_cast<int>(size_.x), static_cast<int>(size_.y),
		typeImg_,
		true
	);
}