#include "../Utility/Utility3D.h"
#include"../Player/ActionController.h"
#include "../Base/CharacterBase.h"
#include"../Enemy/EnemyLogic.h"
#include "../../Common/AnimationController.h"
#include "../Manager/Generic/SceneManager.h"
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
		{CARD_ACT_TYPE::ROAR_ATK, ROAR_ATK},
		{CARD_ACT_TYPE::JUMP_ATK, JUMP_ATK}
	};
}

EnemyCardAction::~EnemyCardAction(void)
{
}

void EnemyCardAction::Init(void)
{
	actType_ = CARD_ACT_TYPE::NONE;
	//deck_.MoveHandToCharge();

	atkStatusTable_ = {
		{CARD_ACT_TYPE::SWIP_ATK, SWIP_ATK},
		{CARD_ACT_TYPE::ROAR_ATK, ROAR_ATK},
		{CARD_ACT_TYPE::JUMP_ATK, JUMP_ATK}
	};
	jumpAtkCnt_ = 0.0f;
	
	if (deck_.GetDrawCardType() == CardBase::CARD_TYPE::ATTACK)
	{
		deck_.MoveHandToCharge();
		actionCntl_.GetInput().GetAttackType();
		//charaObj_.GetCardUI().ChangeSelectState(CardUI::CARD_SELECT::DISITION);
		DesideCardAction();
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

void EnemyCardAction::Release(void)
{
	charaObj_.DeleteAttackCol(Collider::TAG::ENEMY1, Collider::TAG::NML_ATK);
	charaObj_.DeleteAttackCol(Collider::TAG::ENEMY1, Collider::TAG::ROAR_ATK);

	//カード機能配列の解放
	if(!cardFuncs_.empty())
	{
		cardFuncs_.pop();
	}

	//現在使っているカードを捨てる
	deck_.EraseHandCard();
}

void EnemyCardAction::ChangeSwip(void)
{
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::SWIP_ATK), false);
	cardFuncs_.push([this]() {UpdateSwip(); });
}

void EnemyCardAction::ChangeRoar(void)
{
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::ROAR_ATK), false, JUMP_ANIM_START);
	cardFuncs_.push([this]() {UpdateRoar(); });
}

void EnemyCardAction::ChangeJumpAtk(void)
{
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::JUMP_ATK), false, JUMP_ANIM_START);
	cardFuncs_.push([this]() {UpdateJumpAtk(); });
}

void EnemyCardAction::ChangeReload(void)
{
}

void EnemyCardAction::UpdateSwip(void)
{
	AttackMotion(atkStatusTable_[actType_],Collider::TAG::NML_ATK, ATK_ONE_LOCAL);
}

void EnemyCardAction::UpdateRoar(void)
{
	AttackMotion(atkStatusTable_[actType_],Collider::TAG::ROAR_ATK, JUMP_ATK_LOCAL);
}

void EnemyCardAction::UpdateJumpAtk(void)
{
	CardActionBase::ATK_STATUS& status = atkStatusTable_[CARD_ACT_TYPE::JUMP_ATK];
	if (anim_.GetAnimStep()> JUMP_ANIM_END)
	{
		jumpAtkCnt_ += SceneManager::GetInstance().GetDeltaTime();
		//攻撃中
		atkPos_ = Utility3D::AddPosRotate(charaObj_.GetTransform().pos, charaObj_.GetTransform().quaRot, {0.0f,0.0f,0.0f});
		//攻撃判定有効
		isAliveAtkCol_ = true;
		charaObj_.MakeAttackCol(charaObj_.GetCharaTag(), Collider::TAG::NML_ATK,atkPos_, status.atkRadius);
		status.atkRadius += JUMP_ATK_COL_SPD;
		charaObj_.UpdateAttackCol(status.atkRadius);



		if (jumpAtkCnt_ > JUMP_ATK_CNT_MAX)
		{
			jumpAtkCnt_ = 0.0f;
			status.atkRadius = JUMP_ATK_RADIUS;
			charaObj_.DeleteAttackCol(Collider::TAG::ENEMY1,Collider::TAG::NML_ATK);
			actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		}

	}
}

void EnemyCardAction::UpdateReload(void)
{
}

void EnemyCardAction::DesideCardAction(void)
{
	//ロジックから攻撃タイプを取得
	LogicBase::ENEMY_ATTACK_TYPE attackType = actionCntl_.GetInput().GetAttackType();

	switch (attackType)
	{
	case LogicBase::ENEMY_ATTACK_TYPE::NORMAL:
		ChangeCardAction(CARD_ACT_TYPE::SWIP_ATK);
		break;
	case LogicBase::ENEMY_ATTACK_TYPE::JUMP:
		ChangeCardAction(CARD_ACT_TYPE::JUMP_ATK);
		break;
	case LogicBase::ENEMY_ATTACK_TYPE::ROAR:
		ChangeCardAction(CARD_ACT_TYPE::ROAR_ATK);
		break;
	}
}

