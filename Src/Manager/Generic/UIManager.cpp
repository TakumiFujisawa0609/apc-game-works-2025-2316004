#include "../pch.h"
#include "../Object/Character/Player/PlayerHpUI.h"
#include "../Object/Character/Enemy/EnemyHpUI.h"
#include "UIManager.h"

UIManager::UIManager(void)
{
	std::unique_ptr<HpUIBase>hpUi;

	hpUi = std::make_unique<PlayerHpUI>();
	characterHpUI_[CHARACTER_TYPE::PLAYER] = std::move(hpUi);

	hpUi = std::make_unique<EnemyHpUI>();
	characterHpUI_[CHARACTER_TYPE::ENEMY] = std::move(hpUi);
}


void UIManager::Load(void)
{
	for (auto& characterHpUI : characterHpUI_)
	{
		characterHpUI.second->Load();
	}
}

void UIManager::Init()
{
	for (auto& characterHpUI : characterHpUI_)
	{
		characterHpUI.second->Init();
	}
}

void UIManager::Update()
{
	for (auto& characterHpUI : characterHpUI_)
	{
		characterHpUI.second->Update();
	}
}

void UIManager::Draw()
{
	for (auto& characterHpUI : characterHpUI_)
	{
		characterHpUI.second->Draw();
	}
}


void UIManager::RefreshHpUI(const CHARACTER_TYPE _charaType, const HP_DATA _hpData)
{
	characterHpUI_[_charaType]->RefreshHp(_hpData);
}

