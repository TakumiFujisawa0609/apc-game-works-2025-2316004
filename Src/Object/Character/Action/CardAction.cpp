#include"../Manager/Generic/SceneManager.h"
#include"../Player/ActionController.h"
#include"../../Common/AnimationController.h"
#include"../../Card/CardDeck.h"
#include"../../Card/CardBase.h"
#include"../../Card/CardSystem.h"
#include"../Base/CharacterBase.h"
#include"../Base/LogicBase.h"
#include "CardAction.h"

CardAction::CardAction(ActionController& _actCntl, CardDeck& _deck):
	ActionBase(_actCntl),
	deck_(_deck)
{
	isCardAct_ = false;
	isTurnable_ = false;
	cardActTime_ = 0.0f;
	changeAction_={
		{ ACT_STATE::ATTACK_ONE, [this]() {ChangeAttackOne(); }},
		{ ACT_STATE::ATTACK_TWO, [this]() {ChangeAttackTwo(); }},
		{ ACT_STATE::ATTACK_THREE, [this]() {ChangeAttackThree(); }},
	};
}

CardAction::~CardAction(void)
{
}

void CardAction::Init(void)
{
	//手札に移動
	deck_.MoveHandToCharge();
	//アクション中にする
	isCardAct_ = true;
	//カードの属性を受け取ってアニメーションを再生
	std::vector<CardBase::CARD_TYPE>cardTypes = deck_.GetCardType();
	int handCardTypeSize = deck_.GetCardType().size();
	if (IsAttackable())
	{
		changeAction_[ACT_STATE::ATTACK_ONE]();
	}
}

void CardAction::Update()
{
	//カードの勝敗判定
	deck_.CardUseUpdate();
	//相手のカードに負けたらノックバックする
	if (deck_.IsCardFailure())
	{
		//アクション終了
		isCardAct_ = false;
		cardActTime_ = 0.0f;
		deck_.DisCard();
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::REACT);
		return;
	}
	cardActFunc_();
}

bool CardAction::IsAttackable(void)
{
	std::vector<CardBase::CARD_TYPE>cardTypes = deck_.GetCardType();
	int handCardTypeSize = deck_.GetCardType().size();
	return handCardTypeSize == 1 && cardTypes[0] == CardBase::CARD_TYPE::ATTACK;
}

void CardAction::UpdateAttackOne(void)
{
	if(anim_.GetAnimStep()>=ATTACK_COL_START_ANIM_CNT&&
		anim_.GetAnimStep()<=ATTACK_COL_END_ANIM_CNT)
	{
		////攻撃判定有効
		//CardSystem::GetInstance().SetAttackCol(true);
	}
	else if (anim_.GetAnimStep() > ATTACK_COL_END_ANIM_CNT)
	{
		////攻撃判定無効
		//CardSystem::GetInstance().SetAttackCol(false);
		if (IsAttackable()&&actionCntl_.GetInput().GetIsAct().isCardUse)
		{
			//現在使っているカードを捨てる
			deck_.DisCard();
			changeAction_[ACT_STATE::ATTACK_TWO]();
		}
		else if (anim_.IsEnd())
		{
			//アクション終了
			isCardAct_ = false;
			cardActTime_ = 0.0f;
			deck_.DisCard();
			actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
			return;
		}
	}
}

void CardAction::UpdateAttackTwo(void)
{
	if (anim_.GetAnimStep() >= ATTACK_COL_START_ANIM_CNT &&
		anim_.GetAnimStep() <= ATTACK_COL_END_ANIM_CNT)
	{
		////攻撃判定有効
		//CardSystem::GetInstance().SetAttackCol(true);
	}
	else if (anim_.GetAnimStep() > ATTACK_COL_END_ANIM_CNT)
	{
		////攻撃判定無効
		//CardSystem::GetInstance().SetAttackCol(false);
		if (IsAttackable() && actionCntl_.GetInput().GetIsAct().isCardUse)
		{
			deck_.DisCard();
			changeAction_[ACT_STATE::ATTACK_THREE]();
		}
		else if(anim_.IsEnd())
		{
			//アクション終了
			isCardAct_ = false;
			cardActTime_ = 0.0f;
			deck_.DisCard();
			actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
			return;
		}
	}
}

void CardAction::UpdateAttackThree(void)
{
	if (anim_.GetAnimStep() >= ATTACK_COL_START_ANIM_CNT &&
		anim_.GetAnimStep() <= ATTACK_COL_END_ANIM_CNT)
	{
		////攻撃判定有効
		//CardSystem::GetInstance().SetAttackCol(true);
	}
	else if (anim_.IsEnd())
	{
		//アクション終了
		isCardAct_ = false;
		cardActTime_ = 0.0f;
		deck_.DisCard();
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		return;
	}
}

void CardAction::ChangeAttackOne(void)
{
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_1), false);
	cardActFunc_ = [this]() {UpdateAttackOne(); };
}

void CardAction::ChangeAttackTwo(void)
{
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_2), false);
	//手札に移動
	
	deck_.MoveHandToCharge();
	cardActFunc_ = [this]() {UpdateAttackTwo(); };
}

void CardAction::ChangeAttackThree(void)
{
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_3), false);
	//手札に移動
	deck_.MoveHandToCharge();
	cardActFunc_ = [this]() {UpdateAttackThree(); };
}
