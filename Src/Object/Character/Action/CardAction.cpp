#include "../Utility/Utility3D.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Player/ActionController.h"
#include "../../Common/AnimationController.h"
#include "../../Card/CardDeck.h"
#include "../../Card/CardBase.h"
#include "../../Card/CardUI.h"
#include "../../Card/CardSystem.h"
#include "../Base/CharacterBase.h"
#include "../Base/LogicBase.h"
#include "CardAction.h"

CardAction::CardAction(CharacterBase& _charaObj,ActionController& _actCntl, CardDeck& _deck):
	ActionBase(_actCntl),
	charaObj_(_charaObj),
	deck_(_deck),
	pushReloadCnt_(),
	atkPos_()
{
	isCardAct_ = false;
	isTurnable_ = false;
	cardActTime_ = 0.0f;
	changeAction_={
		{ ACT_STATE::ATTACK_ONE, [this]() {ChangeAttackOne(); }},
		{ ACT_STATE::ATTACK_TWO, [this]() {ChangeAttackTwo(); }},
		{ ACT_STATE::ATTACK_THREE, [this]() {ChangeAttackThree(); }},
		{ ACT_STATE::RELOAD, [this]() {ChangeReload(); }},
	};
}

CardAction::~CardAction(void)
{
	changeAction_.clear();
	//カード機能配列の解放
	std::queue<std::function<void(void)>> empty;
	std::swap(cardFuncs_, empty);
}

void CardAction::Init(void)
{
	//アクション中にする
	isCardAct_ = true;
	//カードの属性を受け取ってアニメーションを再生
	std::vector<CardBase::CARD_TYPE>cardTypes = deck_.GetHandCardType();
	attackStageNum_ = 0;
	//手札に移動
	
	if (deck_.IsReloadCard() == CardBase::CARD_TYPE::ATTACK)
	{
		deck_.MoveHandToCharge();
		//charaObj_.GetCardUI().ChangeSelectState(CardUI::CARD_SELECT::DISITION);
		changeAction_[ACT_STATE::ATTACK_ONE]();
	}
	else if (deck_.IsReloadCard()==CardBase::CARD_TYPE::RELOAD)
	{
		changeAction_[ACT_STATE::RELOAD]();
	}
}

void CardAction::Update()
{
	cardFuncs_.front()();
}

bool CardAction::IsAttackable(void)
{
	std::vector<CardBase::CARD_TYPE>cardTypes = deck_.GetHandCardType();
	int handCardTypeSize = static_cast<int>(deck_.GetHandCardType().size());
	return handCardTypeSize == 1 && cardTypes[0] == CardBase::CARD_TYPE::ATTACK;
}

bool CardAction::IsCanComboAttack(void)
{
	return charaObj_.GetCardUI().GetSelectState() != CardUI::CARD_SELECT::DISITION
		&& actionCntl_.IsCardDisitionControll();
}

bool CardAction::IsCardFailure(void)
{
	//カードの勝敗判定
	deck_.CardUseUpdate();
	//相手のカードに負けたらノックバックする
	if (deck_.IsCardFailure())
	{
		//アクション終了
		isCardAct_ = false;
		cardActTime_ = 0.0f;
		deck_.EraseHandCard();
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::REACT);
		return true;
	}
	return false;
}

void CardAction::AttackMotion(const float _atkColStart, const float _atlColEnd)
{
	//攻撃中にカード負けしたら処理を飛ばす
	if (IsCardFailure())return;

	if (anim_.GetAnimStep() >= _atkColStart &&
		anim_.GetAnimStep() <= _atlColEnd)
	{
		//
		atkPos_ = Utility3D::AddPosRotate(charaObj_.GetTransform().pos, charaObj_.GetTransform().quaRot, { 0.0f,50.0f,100.0f });
		//攻撃判定有効
		isAliveAtkCol_ = true;
		charaObj_.MakeAttackCol(charaObj_.GetCharaTag(), atkPos_);

	}
	else if (anim_.GetAnimStep() > ATTACK_COL_END_ANIM_CNT)
	{
		//攻撃判定無効
		charaObj_.DeleteAttackCol(charaObj_.GetCharaTag());
		charaObj_.GetCardUI().ChangeUsedActionCard();
		if (deck_.IsReloadCard() == CardBase::CARD_TYPE::RELOAD && actionCntl_.GetInput().GetIsAct().isCardUse)
		{
			changeAction_[ACT_STATE::RELOAD]();
		}

		if (IsAttackable() && IsCanComboAttack() && actionCntl_.GetInput().GetIsAct().isCardUse)
		{
			if (attackStageNum_ == static_cast<int>(ACT_STATE::ATTACK_ONE))
			{
				changeAction_[ACT_STATE::ATTACK_TWO]();
			}
			else if (attackStageNum_ == static_cast<int>(ACT_STATE::ATTACK_TWO))
			{
				changeAction_[ACT_STATE::ATTACK_THREE]();
			}
		}
		else if (anim_.IsEnd())
		{
			//アクション終了
			isCardAct_ = false;
			cardActTime_ = 0.0f;
			deck_.EraseHandCard();
			cardFuncs_.pop();
			charaObj_.GetCardUI().ChangeUsedActionCard();
			actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
			return;
		}
	}
}

void CardAction::ChangeActionCardInit(void)
{
	attackStageNum_++;
	//現在使っているカードを捨てる
	deck_.EraseHandCard();
	//手札に移動
	deck_.MoveHandToCharge();
	charaObj_.GetCardUI().ChangeUsedActionCard();
	//charaObj_.GetCardUI().ChangeSelectState(CardUI::CARD_SELECT::DISITION);
	cardFuncs_.pop();
}

void CardAction::UpdateAttack(void)
{
	AttackMotion(ATTACK_COL_START_ANIM_CNT, ATTACK_COL_END_ANIM_CNT);
}

void CardAction::UpdateReload(void)
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
		charaObj_.GetCardUI().ChangeSelectState(CardUI::CARD_SELECT::NONE);
	}
	if (pushReloadCnt_ > RELOAD_TIME)
	{
		deck_.Reload();
		cardFuncs_.pop();
		pushReloadCnt_ = 0.0f;
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
	}
}


void CardAction::ChangeAttackOne(void)
{
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_1), false);
	//cardActFunc_ = [this]() {UpdateAttack(); };
	cardFuncs_.push([this]() {UpdateAttack(); });
}

void CardAction::ChangeAttackTwo(void)
{
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_2), false);
	//攻撃段階を増やす
	ChangeActionCardInit();
	cardFuncs_.push([this]() {UpdateAttack(); });
}

void CardAction::ChangeAttackThree(void)
{
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_3), false);
	ChangeActionCardInit();
	cardFuncs_.push([this]() {UpdateAttack(); });
}

void CardAction::ChangeReload(void)
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
	charaObj_.GetCardUI().ChangeSelectState(CardUI::CARD_SELECT::RELOAD);
	
	cardFuncs_.push([this]() {UpdateReload(); });
}
