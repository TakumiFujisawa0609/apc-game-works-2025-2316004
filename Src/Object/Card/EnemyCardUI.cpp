#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include"../Manager/Generic/InputManager.h"
#include"../Manager/Generic/SceneManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Card/CardUIController.h"
#include "EnemyCardUI.h"

EnemyCardUI::EnemyCardUI(void)
{
}

EnemyCardUI::~EnemyCardUI(void)
{
}

void EnemyCardUI::Load(void)
{
	ResourceManager& res = ResourceManager::GetInstance();
	cardNoImg_ = res.Load(ResourceManager::SRC::NUMBERS_IMG).handleIds_;
	atkCardImg_ = res.Load(ResourceManager::SRC::ENEMY_ATK_CARD_IMG).handleId_;
	reloadCardImg_ = -1;	//敵はリロードカードを使わないので読み込まない
	SoundManager::GetInstance().LoadResource(SoundManager::SRC::CARD_MOVE);
}

void EnemyCardUI::Init(void)
{
	//カード初期化
	InitCardUI();

	//状態遷移登録
	changeMoveState_ = {
	{CARD_SELECT::NONE, [this]() {ChangeNone(); } },
	{CARD_SELECT::DISITION, [this]() {ChangeDecision(); } },
	{CARD_SELECT::RELOAD_WAIT, [this]() {ChangeReloadWait(); } }
	};
	ChangeSelectState(CARD_SELECT::NONE);
}

void EnemyCardUI::Update(void)
{

	CardUIBase::Update();
	////カード状態
	//cardUpdate_();
	////使用済みカードの大きさ補完
	//UpdateUsedCard();
	//弾かれるカードの大きさ補完
	ReactMoveCard(REACT_GOAL_CARD_POS);
}

void EnemyCardUI::Draw(void)
{
	//カード描画(共通)
	CardUIBase::Draw();
#ifdef _DEBUG
	DrawDebug();
#endif // _DEBUG

}

void EnemyCardUI::DrawDebug(void)
{
	int i = 0;
	for (auto& action : actions_)
	{
		std::wstring stateStr;
		auto state = action->GetState();
		switch (state)
		{
		case CardUIController::CARD_STATE::DRAW_PILE:
			stateStr = L"DRAW_PILE";
			break;
		case CardUIController::CARD_STATE::MOVE_DRAW:
			stateStr = L"MOVE_DRAW";
			break;
		case CardUIController::CARD_STATE::USING:
			stateStr = L"USING";
			break;
		case CardUIController::CARD_STATE::REACT:
			stateStr = L"REACT";
			break;
		case CardUIController::CARD_STATE::USED:
			stateStr = L"USED";
			break;
		default:
			break;
		}
		DrawFormatString(10, 10 + i * 20, 0xffffff, L"react(%f),Dicision(%f),state(%s)", action->GetReactCount(), action->GetDecisionCnt(), stateStr.c_str());
		i++;
	}
}

void EnemyCardUI::ChangeNone(void)
{
	cardUpdate_ = [this]() {UpdateNone(); };
}

void EnemyCardUI::ChangeDecision(void)
{
	actions_.emplace_back(*handCurrent_);
	handCurrent_++;
	for (auto& act : actions_)
	{
		act->ChangeDicisionEnemyCardMove();
		act->ChangeUsing();
	}

	cardUpdate_ = [this]() {UpdateDecision(); };
}

void EnemyCardUI::ChangeReloadWait(void)
{
	cardUpdate_ = [this]() {UpdateReloadWait(); };
}

void EnemyCardUI::UpdateNone(void)
{
	////手札にすべての初期札を入れる
	//for (auto& it : uiInfos_)
	//{
	//	handCards_.emplace_back(it);
	//}
	//ChangeSelectState(CARD_SELECT::NONE);
}

void EnemyCardUI::UpdateDecision(void)
{
	DecisionMoveCardAll();
}

void EnemyCardUI::UpdateReloadWait(void)
{
	InitCardUI();
	ChangeSelectState(CARD_SELECT::NONE);
}

void EnemyCardUI::UpdateDrawCardUI(void)
{
	for (auto& card : actions_)
	{
		card->Update();
	}
}

void EnemyCardUI::InitCardUI(void)
{
	handCards_.clear();
	//手札にすべての初期札を入れる
	for (auto& it : initialCards_)
	{
		it->ResetCount();
		handCards_.emplace_back(it);
	}

	if (!handCards_.empty())
	{
		handCurrent_ = handCards_.begin();
	}
}
