#include "EnemyCardAction.h"

EnemyCardAction::EnemyCardAction(CharacterBase& _charaObj, ActionController& _actCntl, CardDeck& _deck):
CardActionBase(_charaObj,_actCntl,_deck)
{
	isTurnable_ = false;
	changeAction_ = {
		{ CARD_ACT_TYPE::SWIP_ATK, [this]() {ChangeSwip(); }},
		{ CARD_ACT_TYPE::ROAR_ATK, [this]() {ChangeRoar(); }},
		{ CARD_ACT_TYPE::JUMP_ATK, [this]() {ChangeJumpAtk(); }},
		{ CARD_ACT_TYPE::RELOAD, [this]() {ChangeReload(); }},
	};

	atkStatusTable_ = {
		{CARD_ACT_TYPE::SWIP_ATK, SWIP_ATK},
		{CARD_ACT_TYPE::ROAR_ATK, SWIP_ATK},
		{CARD_ACT_TYPE::JUMP_ATK, SWIP_ATK}
	};
}

EnemyCardAction::~EnemyCardAction(void)
{
}

void EnemyCardAction::Init(void)
{
	
}

void EnemyCardAction::Update(void)
{
	cardFuncs_.front();
}

void EnemyCardAction::ChangeSwip(void)
{
	cardFuncs_.push([this]() {UpdateSwip(); });
}

void EnemyCardAction::ChangeRoar(void)
{
}

void EnemyCardAction::ChangeJumpAtk(void)
{
}

void EnemyCardAction::ChangeReload(void)
{
}

void EnemyCardAction::UpdateSwip(void)
{
	AttackMotion(atkStatusTable_[actType_], ATK_ONE_LOCAL);
}

void EnemyCardAction::UpdateRoar(void)
{
}

void EnemyCardAction::JumpAtkUpdate(void)
{
}

void EnemyCardAction::UpdateReload(void)
{
}
