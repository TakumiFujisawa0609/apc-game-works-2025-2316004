#include "../pch.h"
#include "../Resource/ResourceManager.h"
#include "ButtonUIManager.h"

void ButtonUIManager::Load(void)
{
	buttonUI_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CONTROLLER_UI_IMGS).handleIds_;
}

void ButtonUIManager::Init(void)
{

}

void ButtonUIManager::DrawFromCenter(const BTN_UI_TYPE _btnType, const Vector2F _centerPos, const float _size)
{
	int btnImg = buttonUI_[static_cast<int>(_btnType)];
	Vector2F leftTop = _centerPos - _size / 2;
	Vector2F rightDown = _centerPos + _size / 2;
	DrawExtendGraphF(leftTop.x, leftTop.y, rightDown.x, rightDown.y, btnImg, true);
}

void ButtonUIManager::DrawFromLeftTop(const BTN_UI_TYPE _btnType, const Vector2F _leftTopPos, const float _size)
{
	int btnImg = buttonUI_[static_cast<int>(_btnType)];
	Vector2F rightDown = _leftTopPos + _size;
	DrawExtendGraphF(_leftTopPos.x, _leftTopPos.y, rightDown.x, rightDown.y, btnImg, true);
}

ButtonUIManager::ButtonUIManager(void)
{
	int i = -1;
	buttonUI_ = &i;
}
