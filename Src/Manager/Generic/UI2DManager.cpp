#include "../pch.h"
#include "../Object/Character/Base/UIBase2D.h"
#include "UI2DManager.h"

void UI2DManager::Load(void)
{
	for (auto& ui : ui2D_)
	{
		ui->Load();
	}
}

void UI2DManager::Init(void)
{
	for (auto& ui : ui2D_)
	{
		ui->Init();
	}
}

void UI2DManager::Update(void)
{
	for (auto& ui : ui2D_)
	{
		ui->Update();
	}
}

void UI2DManager::Draw(void)
{
	for (auto& ui : ui2D_)
	{
		ui->Draw();
	}
}

void UI2DManager::AddUi2D(std::unique_ptr<UIBase2D> _ui2D)
{
	//ui2D_.emplace_back(std::move(_ui2D));
}

UI2DManager::UI2DManager(void)
{
}

UI2DManager::~UI2DManager(void)
{
}
