#include "../Utility/Utility3D.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Player/ActionController.h"
#include "../../Common/AnimationController.h"
#include "../../Card/CardDeck.h"
#include "../../Card/CardBase.h"
#include "../../Card/CardUIBase.h"
#include "../../Card/CardSystem.h"
#include "../Base/CharacterBase.h"
#include "../Base/LogicBase.h"
#include "PlayerCardAction.h"

PlayerCardAction::PlayerCardAction(ActionController& _actCntl, CharacterBase& _charaObj, CardDeck& _deck):
	CardActionBase(_actCntl, _charaObj, _deck),
	pushReloadCnt_()
{
	isTurnable_ = false;
	changeAction_={
		{ CARD_ACT_TYPE::ATTACK_ONE, [this]() {ChangeAttackOne(); }},
		{ CARD_ACT_TYPE::ATTACK_TWO, [this]() {ChangeAttackTwo(); }},
		{ CARD_ACT_TYPE::ATTACK_THREE, [this]() {ChangeAttackThree(); }},
		{ CARD_ACT_TYPE::RELOAD, [this]() {ChangeReload(); }},
	};
	ATK_STATUS initStatus={};
	atkStatusTable_ = {
		{CARD_ACT_TYPE::ATTACK_ONE,NORMAL_ATK_ONE},
		{CARD_ACT_TYPE::ATTACK_TWO,NORMAL_ATK_TWO},
		{CARD_ACT_TYPE::ATTACK_THREE,NORMAL_ATK_THREE},
		{CARD_ACT_TYPE::RELOAD,initStatus}		//攻撃判定がないものは初期値を入れる
	};
}

PlayerCardAction::~PlayerCardAction(void)
{
	changeAction_.clear();
	//カード機能配列の解放
	std::queue<std::function<void(void)>> empty;
	std::swap(cardFuncs_, empty);
}

void PlayerCardAction::Init(void)
{
	//カードの属性を受け取ってアニメーションを再生
	std::vector<CardBase::CARD_TYPE>cardTypes = deck_.GetHandCardType();
	attackStageNum_ = 0;
	//手札に移動
	
	if (deck_.GetDrawCardType() == CardBase::CARD_TYPE::ATTACK)
	{
		deck_.MoveHandToCharge();
		charaObj_.GetCardUI().ChangeSelectState(CardUIBase::CARD_SELECT::DISITION);
		ChangeCardAction(CARD_ACT_TYPE::ATTACK_ONE);
	}
	else if (deck_.GetDrawCardType()==CardBase::CARD_TYPE::RELOAD)
	{
		ChangeCardAction(CARD_ACT_TYPE::RELOAD);
	}
}

void PlayerCardAction::Update()
{
	cardFuncs_.front()();
}

void PlayerCardAction::Release(void)
{
	//現在使っているカードを捨てる
	deck_.EraseHandCard();
	//当たり判定削除
	charaObj_.DeleteAttackCol(Collider::TAG::PLAYER1,Collider::TAG::NML_ATK);
	charaObj_.GetCardUI().ChangeUsedActionCard();
	if (!cardFuncs_.empty())
	{
		cardFuncs_.pop();
	}
	actType_ = CARD_ACT_TYPE::NONE;
}

bool PlayerCardAction::IsAttackable(void)
{
	std::vector<CardBase::CARD_TYPE>cardTypes = deck_.GetHandCardType();
	int handCardTypeSize = static_cast<int>(deck_.GetHandCardType().size());
	return handCardTypeSize == 1 && cardTypes[0] == CardBase::CARD_TYPE::ATTACK;
}

bool PlayerCardAction::IsCanComboAttack(void)
{
	return charaObj_.GetCardUI().GetSelectState() != CardUIBase::CARD_SELECT::DISITION
		&& actionCntl_.IsCardDisitionControll();
}

void PlayerCardAction::ChangeActionCardInit(void)
{
	attackStageNum_++;
	charaObj_.DeleteCard();
	cardFuncs_.pop();
}


void PlayerCardAction::UpdateAttack(void)
{
	AttackMotion(atkStatusTable_[actType_],Collider::TAG::NML_ATK ,ATK_ONE_LOCAL);
}

void PlayerCardAction::UpdateReload(void)
{
	if (actionCntl_.GetInput().GetIsAct().isCardPushKeep)
	{
		pushReloadCnt_ += scnMng_.GetDeltaTime();
		float per = pushReloadCnt_ / RELOAD_TIME;

		charaObj_.GetCardUI().SetReloadCount(per);
	}
	else
	{
		cardFuncs_.pop();
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		actType_ = CARD_ACT_TYPE::NONE;
		charaObj_.GetCardUI().ChangeSelectState(CardUIBase::CARD_SELECT::NONE);
	}
	if (pushReloadCnt_ > RELOAD_TIME)
	{
		deck_.Reload();
		cardFuncs_.pop();
		pushReloadCnt_ = 0.0f;
		actType_ = CARD_ACT_TYPE::NONE;
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);

	}
}


void PlayerCardAction::ChangeAttackOne(void)
{
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_1), false);
	cardFuncs_.push([this]() {UpdateAttack(); });
}

void PlayerCardAction::ChangeAttackTwo(void)
{
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_2), false);
	//攻撃段階を増やす
	ChangeActionCardInit();
	cardFuncs_.push([this]() {UpdateAttack(); });
}

void PlayerCardAction::ChangeAttackThree(void)
{
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_3), false);
	ChangeActionCardInit();
	cardFuncs_.push([this]() {UpdateAttack(); });
}

void PlayerCardAction::ChangeReload(void)
{
	if (!cardFuncs_.empty())
	{
		cardFuncs_.pop();
	}
	//現在使っているカードを捨てる
	deck_.EraseHandCard();
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_3), true);

	float per = pushReloadCnt_ / RELOAD_TIME;
	charaObj_.GetCardUI().SetReloadCount(per);
	charaObj_.GetCardUI().ChangeSelectState(CardUIBase::CARD_SELECT::RELOAD_WAIT);
	
	cardFuncs_.push([this]() {UpdateReload(); });
}

void PlayerCardAction::ChangeComboAction(void)
{
	if (deck_.GetDrawCardType() == CardBase::CARD_TYPE::RELOAD && actionCntl_.GetInput().GetIsAct().isCardUse)
	{
		ChangeCardAction(CARD_ACT_TYPE::RELOAD);
	}

	if (actionCntl_.GetInput().GetIsAct().isCardUse)
	{
		if (IsAttackable() && IsCanComboAttack())
		{
			if (attackStageNum_ == static_cast<int>(CARD_ACT_TYPE::ATTACK_ONE))
			{
				charaObj_.GetCardUI().ChangeSelectState(CardUIBase::CARD_SELECT::DISITION);
				ChangeCardAction(CARD_ACT_TYPE::ATTACK_TWO);
			}
			else if (attackStageNum_ == static_cast<int>(CARD_ACT_TYPE::ATTACK_TWO))
			{
				charaObj_.GetCardUI().ChangeSelectState(CardUIBase::CARD_SELECT::DISITION);
				ChangeCardAction(CARD_ACT_TYPE::ATTACK_THREE);
			}
		}

	}
}
