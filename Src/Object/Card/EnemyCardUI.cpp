#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include"../Manager/Generic/InputManager.h"
#include"../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/DataBank.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Card/CardSystem.h"
#include "../Card/CardUIController.h"
#include "EnemyCardUI.h"

EnemyCardUI::EnemyCardUI(void)
{
	charaType_ = "Enemy";
}

EnemyCardUI::~EnemyCardUI(void)
{
}

void EnemyCardUI::Load(void)
{
	CardUIBase::Load();
	atkCardImg_ = resMng_.Load(ResourceManager::SRC::ENEMY_ATK_CARD_IMG).handleId_;
	reloadCardImg_ = -1;	//“G‚ÍƒŠƒ[ƒhƒJ[ƒh‚ðŽg‚í‚È‚¢‚Ì‚Å“Ç‚Ýž‚Ü‚È‚¢
	soundMng_.LoadResource(SoundManager::SRC::CARD_BREAK);
	cardWinRes_ = SoundManager::SRC::CARD_BREAK;
}

void EnemyCardUI::Init(void)
{

	//AddCardUIData();
	LoadCardData();
	//ƒJ[ƒh‰Šú‰»
	InitCardUI();
	soundMng_.SetSoundVolumeSRC(SoundManager::SRC::CARD_BREAK, CARD_BREAK_SOUND_VOLUME);
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

	CardUIBase::Update();

	ReactMoveCard(REACT_GOAL_CARD_POS);
}

void EnemyCardUI::Draw(void)
{
	//ƒJ[ƒh•`‰æ(‹¤’Ê)
	CardUIBase::Draw();
#ifdef _DEBUG
	DrawDebug();
#endif // _DEBUG

}
#ifdef _DEBUG
void EnemyCardUI::DrawDebug(void)
{
	int i = 0;
	for (const auto& action : actions_)
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
#endif // _DEBUG



//void EnemyCardUI::AddCardUIData(void)
//{
//	std::vector<CardBase::CARD_STATUS> charaDeck = DataBank::GetInstance().GetCardDatas(CHARACTER_TYPE::ENEMY);
//	int size = static_cast<int>(charaDeck.size());
//	for (int i = 0; i < size; i++)
//	{
//		AddCardUi(charaDeck[i]);
//	}
//
//}

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
	//Œˆ’èˆÚ“®‚ªI‚í‚Á‚½‚çnoneó‘Ô‚É–ß‚·
	auto it = std::find_if(actions_.begin(), actions_.end(), [this](auto& act) {return act->GetDecisionCnt() > 0.0f; });
	if (it == actions_.end())
	{
		SetBasePosActionCards();
		ChangeSelectState(CARD_SELECT::NONE);
	}
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
	//ŽèŽD‚É‚·‚×‚Ä‚Ì‰ŠúŽD‚ð“ü‚ê‚é
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
