#include "../pch.h"
#include "../Object/Character/Player/PlayerHpUI.h"
#include "../Object/Character/Enemy/EnemyHpUI.h"
#include "../Object/Card/PlayerCardUI.h"
#include "../Object/Card/EnemyCardUI.h"
#include "UIManager.h"

UIManager::UIManager(void)
{

	CreateHpUI();
	CreateCardUI();

}

void UIManager::CreateHpUI(void)
{
	std::unique_ptr<HpUIBase>hpUi;
	hpUi = std::make_unique<PlayerHpUI>();
	characterHpUI_[CHARACTER_TYPE::PLAYER] = std::move(hpUi);

	hpUi = std::make_unique<EnemyHpUI>();
	characterHpUI_[CHARACTER_TYPE::ENEMY] = std::move(hpUi);
}

void UIManager::CreateCardUI(void)
{
	std::unique_ptr<CardUIBase>cardUI;

	cardUI = std::make_unique<PlayerCardUI>();
	cardUI_[CHARACTER_TYPE::PLAYER] = std::move(cardUI);

	cardUI = std::make_unique<EnemyCardUI>();
	cardUI_[CHARACTER_TYPE::ENEMY] = std::move(cardUI);
}


void UIManager::Load(void)
{
	for (auto& characterHpUI : characterHpUI_)
	{
		characterHpUI.second->Load();
	}

	for (auto& cardUI : cardUI_)
	{
		cardUI.second->Load();
	}
}

void UIManager::Init()
{
	for (auto& characterHpUI : characterHpUI_)
	{
		characterHpUI.second->Init();
	}

	for (auto& cardUI : cardUI_)
	{
		cardUI.second->Init();
	}
}

void UIManager::Update()
{
	for (auto& characterHpUI : characterHpUI_)
	{
		characterHpUI.second->Update();
	}

	for (auto& cardUI : cardUI_)
	{
		cardUI.second->Update();
	}
}

void UIManager::Draw()
{
	for (auto& characterHpUI : characterHpUI_)
	{
		characterHpUI.second->Draw();
	}

	for (auto& cardUI : cardUI_)
	{
		cardUI.second->Draw();
	}
}


void UIManager::RefreshHpUI(const CHARACTER_TYPE _charaType, const HP_DATA _hpData)
{
	characterHpUI_[_charaType]->RefreshHp(_hpData);
}

CardUIBase& UIManager::GetCardUI(const CHARACTER_TYPE _charaType)
{
	return *cardUI_[_charaType];
}

