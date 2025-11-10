#include "../Utility/Utility3D.h"
#include"../Player/ActionController.h"
#include "../Base/CharacterBase.h"
#include"../Enemy/EnemyLogic.h"
#include "../../Common/AnimationController.h"
#include "../Manager/Generic/SceneManager.h"
#include "../../Card/CardDeck.h"
#include "../../Card/CardBase.h"
#include "../../Card/CardUIBase.h"
#include "EnemyCardAction.h"

EnemyCardAction::EnemyCardAction(ActionController& _actCntl, CharacterBase& _charaObj, CardDeck& _deck):
CardActionBase(_actCntl, _charaObj, _deck),
jumpAtkCnt_(0.0f),
roleAtkCnt_(0.0f),
preRoleAtkCnt_(0.0f),
preRolePos_(Utility3D::VECTOR_ZERO),
roleDeg_(0.0f)
{
	isTurnable_ = false;

	changeAction_ = {
		{ CARD_ACT_TYPE::SWIP_ATK, [this]() {ChangeSwip(); }},
		{ CARD_ACT_TYPE::ROAR_ATK, [this]() {ChangeRoar(); }},
		{ CARD_ACT_TYPE::JUMP_ATK, [this]() {ChangeJumpAtk(); }},
		{ CARD_ACT_TYPE::ROLE_ATK, [this]() {ChangeRoleAtk(); }},
		{ CARD_ACT_TYPE::RELOAD, [this]() {ChangeReload(); }},
	};

	atkStatusTable_ = {
		{CARD_ACT_TYPE::SWIP_ATK, SWIP_ATK},
		{CARD_ACT_TYPE::ROAR_ATK, ROAR_ATK},
		{CARD_ACT_TYPE::JUMP_ATK, JUMP_ATK},
		{CARD_ACT_TYPE::ROLE_ATK, ROLE_ATK }
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
	speed_ = 0.0f;
	roleAtkCnt_ = 0.0f;
	roleDeg_ = 0.0f;
	if (deck_.GetDrawCardType() == CardBase::CARD_TYPE::ATTACK)
	{
		deck_.MoveHandToCharge();
		actionCntl_.GetInput().GetAttackType();
		charaObj_.GetCardUI().ChangeSelectState(CardUIBase::CARD_SELECT::DISITION);
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
	actionCntl_.GetInput().IsActioningSet();
	//カード機能配列の解放
	if (!cardFuncs_.empty())
	{
		cardFuncs_.pop();
	}

	//現在使っているカードを捨てる
	deck_.EraseHandCard();
	charaObj_.GetCardUI().ChangeUsedActionCard();
	charaObj_.GetCardUI().ChangeSelectState(CardUIBase::CARD_SELECT::NONE);

	//硬直時間セット
	actionCntl_.GetInput().SetFreezeCntByAttackType();
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

void EnemyCardAction::ChangeRoleAtk(void)
{
	preRoleAtkCnt_ = ROLE_PRE_TIME;
	roleAtkCnt_ = ROLE_TIME;
	preRolePos_ = charaObj_.GetTransform().pos;
	
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::NONE), true);
	cardFuncs_.push([this]() {UpdateRoleAtk(); });
}

void EnemyCardAction::ChangeReload(void)
{
	if (!cardFuncs_.empty())
	{
		cardFuncs_.pop();
	}
	//現在使っているカードを捨てる
	deck_.EraseHandCard();
	charaObj_.GetCardUI().ChangeSelectState(CardUIBase::CARD_SELECT::RELOAD);
	cardFuncs_.push([this]() {UpdateReload(); });
}

void EnemyCardAction::UpdateSwip(void)
{
	//攻撃モーション
	AttackMotion(atkStatusTable_[actType_],Collider::TAG::NML_ATK, ATK_ONE_LOCAL);
}

void EnemyCardAction::UpdateRoar(void)
{
	AttackMotion(atkStatusTable_[actType_],Collider::TAG::ROAR_ATK, JUMP_ATK_LOCAL);
}

void EnemyCardAction::UpdateJumpAtk(void)
{
	//負けたら終了
	if (IsCardFailure(Collider::TAG::NML_ATK))return;
	const Transform& charaTrans = charaObj_.GetTransform();
	//ジャンプ攻撃処理
	CardActionBase::ATK_STATUS& status = atkStatusTable_[CARD_ACT_TYPE::JUMP_ATK];
	if (anim_.GetAnimStep()> JUMP_ANIM_END)
	{
		jumpAtkCnt_ += SceneManager::GetInstance().GetDeltaTime();
		//攻撃中
		atkPos_ = Utility3D::AddPosRotate(charaTrans.pos, charaTrans.quaRot, {0.0f,0.0f,0.0f});
		//攻撃判定有効
		isAliveAtkCol_ = true;
		charaObj_.MakeAttackCol(charaObj_.GetCharaTag(), Collider::TAG::NML_ATK, atkPos_, status.atkRadius);

		//攻撃範囲拡大
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

void EnemyCardAction::UpdateRoleAtk(void)
{
	//負けたら終了
	if (IsCardFailure(Collider::TAG::NML_ATK))return;
	const float deltaTIme = scnMng_.GetDeltaTime();
	//前隙カウント
	if (preRoleAtkCnt_ >= 0.0f)
	{
		//前隙中
		preRoleAtkCnt_ -= deltaTIme;
		//前に進む
		//actionCntl_.GetInput().GetLookAtTargetDir();
		roleMoveDeg_ = actionCntl_.GetInput().GetLookAtTargetDeg();
		roleMoveDir_ = actionCntl_.GetInput().GetLookAtTargetDir();
		isTurnable_ = true;
		return;
	}
	

	//現在の座標取得
	const Transform trans = charaObj_.GetTransform();
	const VECTOR charaPos = trans.pos;
	const VECTOR& centerPos = charaObj_.GetCharaCecterPos();
	isTurnable_ = false;
	if (preRoleAtkCnt_ < 0.0f)
	{
		roleAtkCnt_ -= deltaTIme;
		//攻撃中
		atkPos_ = Utility3D::AddPosRotate(trans.pos, trans.quaRot, { 0.0f,centerPos.y,0.0f });
		//転がる間の速度
		speed_ = ROLE_SPEED;
		//前に進む
		charaObj_.LariatMove(roleDeg_);
		roleDeg_ += 30.0f;
		//当たり判定の作成
		charaObj_.MakeAttackCol(Collider::TAG::ENEMY1, Collider::TAG::NML_ATK, atkPos_, ROLE_ATK_RADIUS);
		actionCntl_.GetInput().SetDegAndDir(roleMoveDeg_, roleMoveDir_);
		if (roleAtkCnt_ < 0.0f)
		{
			actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		}
	}
}

void EnemyCardAction::UpdateReload(void)
{
	deck_.Reload();
	actType_ = CARD_ACT_TYPE::NONE;
	actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
}

void EnemyCardAction::DesideCardAction(void)
{
	//ロジックから攻撃タイプを取得
	LogicBase::ENEMY_ATTACK_TYPE attackType = actionCntl_.GetInput().GetAttackType();
	//LogicBase::ENEMY_ATTACK_TYPE attackType = LogicBase::ENEMY_ATTACK_TYPE::ROLE;

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
	case LogicBase::ENEMY_ATTACK_TYPE::ROLE:
		ChangeCardAction(CARD_ACT_TYPE::ROLE_ATK);
		break;
	}
}

