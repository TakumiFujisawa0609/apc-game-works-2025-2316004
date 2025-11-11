#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include"../Manager/Generic/InputManager.h"
#include"../Manager/Generic/SceneManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
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
	cardNoImgs_ = res.Load(ResourceManager::SRC::NUMBERS_IMG).handleIds_;
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
	{CARD_SELECT::DISITION, [this]() {ChangeDisition(); } },
	{CARD_SELECT::RELOAD, [this]() {ChangeReloadWait(); } }
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
	ReactMoveCard();
	for (auto& card : actions_)
	{
		card.numPos = card.cardPos + (NUM_LOCAL_POS * card.cardScl);
	}
}

void EnemyCardUI::Draw(void)
{
	for (auto& card : actions_)
	{
		DrawCard(card);
	}
}

void EnemyCardUI::ChangeNone(void)
{
	cardUpdate_ = [this]() {UpdateNone(); };
}

void EnemyCardUI::ChangeDisition(void)
{
	
	actions_.emplace_back(*handCurrent_);
	handCurrent_++;
	for (auto& act : actions_)
	{
		act.cardPos = SELECT_CARD_START_POS;
		act.numPos = act.cardPos + (NUM_LOCAL_POS * act.cardScl);
		act.disitionCnt_ = DISITION_MOVE_CARD_TIME;
	}

	cardUpdate_ = [this]() {UpdateDisition(); };
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

void EnemyCardUI::UpdateDisition(void)
{
	DisitionMoveCardAll();
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
		handCurrent_++;
	}
}
