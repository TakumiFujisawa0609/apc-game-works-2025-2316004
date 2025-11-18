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
	atkCardImg_ = res.Load(ResourceManager::SRC::ATK_CARD_IMG).handleId_;
	reloadCardImg_ = res.Load(ResourceManager::SRC::RELOAD_CARD_IMG).handleId_;
	SoundManager::GetInstance().LoadResource(SoundManager::SRC::CARD_MOVE);
}

void EnemyCardUI::Init(void)
{
	//ƒJ[ƒh‰Šú‰»
	InitCardUI();

	//ó‘Ô‘JˆÚ“o˜^
	changeMoveState_ = {
	{CARD_SELECT::NONE, [this]() {ChangeNone(); } },
	{CARD_SELECT::DISITION, [this]() {ChangeDecision(); } },
	{CARD_SELECT::RELOAD_WAIT, [this]() {ChangeReloadWait(); } }
	};
	
	ChangeSelectState(CARD_SELECT::NONE);
}

void EnemyCardUI::Update(void)
{
	//ƒJ[ƒhó‘Ô
	cardUpdate_();
	//Žg—pÏ‚ÝƒJ[ƒh‚Ì‘å‚«‚³•âŠ®
	UpdateUsedCard();
	//’e‚©‚ê‚éƒJ[ƒh‚Ì‘å‚«‚³•âŠ®
	ReactMoveCard(REACT_GOAL_CARD_POS);
	//for (auto& card : actions_)
	//{
	//	card.numPos_ = card.cardPos_ + (NUM_LOCAL_POS * card.cardScl_);
	//}
}

void EnemyCardUI::Draw(void)
{
	for (auto& card : actions_)
	{
		card->Draw();
	}
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
	////ŽèŽD‚É‚·‚×‚Ä‚Ì‰ŠúŽD‚ð“ü‚ê‚é
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

void EnemyCardUI::InitCardUI(void)
{
	handCards_.clear();
	//ŽèŽD‚É‚·‚×‚Ä‚Ì‰ŠúŽD‚ð“ü‚ê‚é
	for (auto& it : uiInfos_)
	{
		it->ResetCount();
		handCards_.emplace_back(it);
	}

	if (!handCards_.empty())
	{
		handCurrent_ = handCards_.begin();
	}
}
