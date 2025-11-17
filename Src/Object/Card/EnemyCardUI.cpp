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
	//カード状態
	cardUpdate_();
	//使用済みカードの大きさ補完
	UpdateUsedCard();
	//弾かれるカードの大きさ補完
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
		//act.cardPos_ = ENEMY_SELECT_CARD_START_POS;
		//act.numPos_ = act.cardPos_ + (NUM_LOCAL_POS * act.cardScl_);
		//act.disitionCnt_ = DISITION_MOVE_CARD_TIME;
		//act->ChangeUsing();
	}

	cardUpdate_ = [this]() {UpdateDecision(); };
}

void EnemyCardUI::ChangeReloadWait(void)
{
	cardUpdate_ = [this]() {UpdateReloadWait(); };
}

void EnemyCardUI::UpdateNone(void)
{
	//手札にすべての初期札を入れる
	for (auto& it : uiInfos_)
	{
		handCards_.emplace_back(it);
	}
	ChangeSelectState(CARD_SELECT::NONE);
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
	//手札にすべての初期札を入れる
	for (auto& it : uiInfos_)
	{
		handCards_.emplace_back(it);
	}

	if (!handCards_.empty())
	{
		handCurrent_ = handCards_.begin();
	}
}
