#include "../Base/CharacterBase.h"
#include "../../Common/AnimationController.h"
#include "../../Card/CardDeck.h"
#include "../../Card/CardBase.h"
#include "EnemyCardAction.h"

EnemyCardAction::EnemyCardAction(ActionController& _actCntl, CharacterBase& _charaObj, CardDeck& _deck):
CardActionBase(_actCntl, _charaObj, _deck)
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
	actType_ = CARD_ACT_TYPE::NONE;
	deck_.MoveHandToCharge();

	if (deck_.GetDrawCardType() == CardBase::CARD_TYPE::ATTACK)
	{
		//deck_.MoveHandToCharge();
		//charaObj_.GetCardUI().ChangeSelectState(CardUI::CARD_SELECT::DISITION);
		ChangeCardAction(CARD_ACT_TYPE::SWIP_ATK);
	}
	else if (deck_.GetDrawCardType() == CardBase::CARD_TYPE::RELOAD)
	{
		ChangeCardAction(CARD_ACT_TYPE::RELOAD);
	}
}

void EnemyCardAction::Update(void)
{
	cardFuncs_.front()();
}

void EnemyCardAction::ChangeSwip(void)
{
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::SWIP_ATK), false);
	cardFuncs_.push([this]() {UpdateSwip(); });
}

void EnemyCardAction::ChangeRoar(void)
{
}

void EnemyCardAction::ChangeJumpAtk(void)
{
	velocity_.y = sqrtf(2.0f * GRAVITY * JUMP_HEIGHT);
	jumpPow_.y = velocity_.y;
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
