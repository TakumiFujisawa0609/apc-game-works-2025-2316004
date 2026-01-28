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

void ButtonUIManager::Draw(const BTN_UI_TYPE _btnType, const Vector2F _centerPos, const float _size)
{
	int btnImg = buttonUI_[static_cast<int>(_btnType)];
	Vector2F leftTop = _centerPos - _size;
	Vector2F rightDown = _centerPos + _size;

	DrawExtendGraphF(leftTop.x, leftTop.y, rightDown.x, rightDown.y, btnImg, true);
}

ButtonUIManager::ButtonUIManager(void)
{
	int i = -1;
	buttonUI_ = &i;
}
