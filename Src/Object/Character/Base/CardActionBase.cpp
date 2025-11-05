#include "../pch.h"
#include "../Utility/Utility3D.h"
#include "ActionBase.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Player/ActionController.h"
#include "../../Common/AnimationController.h"
#include "../../Card/CardDeck.h"
#include "../../Card/CardBase.h"
#include "../../Card/CardUI.h"
#include "../../Card/CardSystem.h"
#include "../Base/CharacterBase.h"
#include "../Base/LogicBase.h"

#include "CardActionBase.h"

CardActionBase::CardActionBase(ActionController& _actCntl, CharacterBase& _charaObj, CardDeck& _deck):
	ActionBase(_actCntl),
	charaObj_(_charaObj),
	deck_(_deck),
	atkPos_({}),
	actType_(CARD_ACT_TYPE::NONE),
	velocity_({})
{
}

CardActionBase::~CardActionBase(void)
{
	atkStatusTable_.clear();
}

void CardActionBase::Init(void)
{
}

void CardActionBase::Update()
{
}

void CardActionBase::ChangeCardAction(const CARD_ACT_TYPE& _type)
{
	//同じ状態が入ってきたら抜ける
	if (actType_ == _type)return;
	actType_ = _type;
	changeAction_[actType_]();

	//当たり判定情報を変える
	atkStatusTable_[actType_];
}

void CardActionBase::AttackMotion(const ATK_STATUS& _status, const Collider::TAG& _attackTag, const VECTOR& _localPos)
{
	//攻撃中にカード負けしたら処理を飛ばす
	if (IsCardFailure())
	{
		FinishAttack(_attackTag);
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::REACT);
		return;
	}

	if (anim_.GetAnimStep() >= _status.colStartCnt&&
		anim_.GetAnimStep() <= _status.colEndCnt)
	{
		//攻撃中
		atkPos_ = Utility3D::AddPosRotate(charaObj_.GetTransform().pos, charaObj_.GetTransform().quaRot, _localPos);
		//攻撃判定有効
		isAliveAtkCol_ = true;
		charaObj_.MakeAttackCol(charaObj_.GetCharaTag(), _attackTag, atkPos_,_status.atkRadius);

	}
	else if (anim_.IsEnd())
	{
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		return;
	}
	else if (anim_.GetAnimStep() > _status.colEndCnt)
	{
		//攻撃判定無効
		charaObj_.DeleteAttackCol(charaObj_.GetCharaTag(),_attackTag);
		charaObj_.GetCardUI().ChangeUsedActionCard();
		//次の攻撃につなげる
		ChangeComboAction();
	}
}

bool CardActionBase::IsCardFailure(void)
{
	//カードの勝敗判定
	deck_.CardUseUpdate();
	//相手のカードに負けたらノックバックする
	if (deck_.IsCardFailure())
	{
		return true;
	}
	return false;
}

void CardActionBase::FinishAttack(const Collider::TAG _attackCol)
{
	//攻撃判定無効
	deck_.EraseHandCard();
	charaObj_.DeleteAttackCol(charaObj_.GetCharaTag(),_attackCol);
	charaObj_.GetCardUI().ChangeUsedActionCard();
	actType_ = CARD_ACT_TYPE::NONE;
	cardFuncs_.pop();
}

void CardActionBase::ChangeComboAction(void)
{
}
