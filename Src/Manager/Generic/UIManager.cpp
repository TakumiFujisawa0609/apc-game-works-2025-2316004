#include "../pch.h"
#include "../Object/Character/Player/PlayerHpUI.h"
#include "../Object/Character/Enemy/EnemyHpUI.h"
#include "UIManager.h"

void UIManager::Init()
{
	std::unique_ptr<HpUIBase>hpUi;

	hpUi = std::make_unique<PlayerHpUI>();
	characterHpUI_[CHARA_TYPE::PLAYER] = std::move(hpUi);

	hpUi = std::make_unique<EnemyHpUI>();
	characterHpUI_[CHARA_TYPE::ENEMY] = std::move(hpUi);
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
}

void UIManager::Release()
{
}
