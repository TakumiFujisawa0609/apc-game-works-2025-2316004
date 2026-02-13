#include "../pch.h"
#include "CardPresenter.h"
#include "../Card/CardUiBase.h"
#include "../Card/CardDeck.h"
#include "../../Manager/Generic/UIManager.h"

CardPresenter::CardPresenter(CHARACTER_TYPE _type, CardDeck& _deck):
	deck_(_deck),
	type_(_type),
	cardUI_(UIManager::GetInstance().GetCardUI(type_))
{
}

CardPresenter::~CardPresenter(void)
{
}

void CardPresenter::FinishCard(void)
{
	//使用済みへ移行
	cardUI_.ChangeUsedActionCard();
	deck_.EraseHandCard();
	cardUI_.ChangeSelectState(CardUIBase::CARD_SELECT::NONE);
}

void CardPresenter::FailureCard(void)
{
	//弾き状態へ移行
	cardUI_.ChangeReactActionCard();
	deck_.EraseHandCard();
}

void CardPresenter::PutCard(void)
{
	deck_.MoveUsingCardToDrawPile();
	cardUI_.ChangeSelectState(CardUIBase::CARD_SELECT::DISITION);
}


void CardPresenter::RoleRevolver(const CardUIBase::CARD_SELECT _moveLR)
{
	//UIのカードリボルバー回転
	cardUI_.ChangeSelectState(_moveLR);

	//内部のカードを動かす
	_moveLR == CardUIBase::CARD_SELECT::LEFT ? deck_.CardMoveLeft() : deck_.CardMoveRight();
}

const CardBase::CARD_TYPE CardPresenter::GetCardType(void) const
{
	return deck_.GetDrawCardType();
}

const std::vector<CardBase::CARD_TYPE> CardPresenter::GetHandCardType(void) const
{
	return deck_.GetHandCardType();
}

void CardPresenter::DeckReload(void)
{
	deck_.Reload();
}

void CardPresenter::ChangeCard(void)
{

}

void CardPresenter::EnemyCardReload(void)
{
	deck_.Reload();
	cardUI_.ChangeSelectState(CardUIBase::CARD_SELECT::RELOAD_WAIT);
}

void CardPresenter::ChangeAction(void)
{
	//使用済みへ移行
	cardUI_.ChangeUsedActionCard();
	deck_.EraseHandCard();
	cardUI_.ChangeSelectState(CardUIBase::CARD_SELECT::NONE);
}

const bool CardPresenter::IsCardFailure(void)const
{
	return deck_.IsCardFailure();
}
