#include "../Utility/Utility3D.h"
#include "../Common/Easing.h"
#include"../Player/ActionController.h"
#include "../Base/CharacterBase.h"
#include"../Enemy/EnemyLogic.h"
#include"../Enemy/EnemyRock.h"
#include "../../Common/AnimationController.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../../Card/CardDeck.h"
#include "../../Card/CardBase.h"
#include "../../Card/CardUIBase.h"
#include "../../Card/CardSystem.h"
#include "EnemyCardAction.h"

EnemyCardAction::EnemyCardAction(ActionController& _actCntl, CharacterBase& _charaObj, CardDeck& _deck):
CardActionBase(_actCntl, _charaObj, _deck),
cameraShakeCnt_(0.0f),
roleAtkCnt_(0.0f),
preRoleAtkCnt_(0.0f),
preRolePos_(Utility3D::VECTOR_ZERO),
roleDeg_(0.0f),
soundMng_(SoundManager::GetInstance())
{
	isTurnable_ = false;
	changeAction_ = {
		{ CARD_ACT_TYPE::STOMP_ATK, [this]() {ChangeStomp(); }},
		{ CARD_ACT_TYPE::ROAR_ATK, [this]() {ChangeRoar(); }},
		{ CARD_ACT_TYPE::JUMP_ATK, [this]() {ChangeJumpAtk(); }},
		{ CARD_ACT_TYPE::RUSH_ATK, [this]() {ChangeRoleAtk(); }},
		{ CARD_ACT_TYPE::DUEL_FAZE, [this]() {ChangeDuel(); }},
		{ CARD_ACT_TYPE::RELOAD, [this]() {ChangeReload(); }},
	};

	atkStatusTable_ = {
		{CARD_ACT_TYPE::STOMP_ATK, SWIP_ATK},
		{CARD_ACT_TYPE::ROAR_ATK, ROAR_ATK},
		{CARD_ACT_TYPE::JUMP_ATK, JUMP_ATK},
		{CARD_ACT_TYPE::RUSH_ATK, RUSH_ATK }
	};

	charaObj_.AddEnemyRock(STOMP_ATK_ROCK_NUM,atk_.pos);


}

EnemyCardAction::~EnemyCardAction(void)
{
}

void EnemyCardAction::Load(void)
{
	soundMng_.LoadResource(SoundManager::SRC::ENEMY_JUMP_LAND_SE);
	soundMng_.LoadResource(SoundManager::SRC::ENEMY_CHARGE_SE);
	soundMng_.LoadResource(SoundManager::SRC::ENEMY_STOMP_SE);

	charaObj_.LoadEnemyRock();

}

void EnemyCardAction::Init(void)
{
	actType_ = CARD_ACT_TYPE::NONE;
	//deck_.MoveUsingCardToDrawPile();
	easing_ = std::make_unique<Easing>();
	atkStatusTable_ = {
		{CARD_ACT_TYPE::STOMP_ATK, SWIP_ATK},
		{CARD_ACT_TYPE::ROAR_ATK, ROAR_ATK},
		{CARD_ACT_TYPE::JUMP_ATK, JUMP_ATK}
	};
	cameraShakeCnt_ = 0.0f;
	speed_ = 0.0f;
	roleAtkCnt_ = 0.0f;
	roleDeg_ = 0.0f;
	jampCardNum_ = 0;
	atk_ = {};


	if (deck_.GetDrawCardType() == CardBase::CARD_TYPE::ATTACK)
	{
		//if (actionCntl_.GetInput().GetAttackType() == LogicBase::ENEMY_ATTACK_TYPE::JUMP)
		//{
		//	ChangeCardAction(CARD_ACT_TYPE::DUEL_FAZE);		//ジャンプチャージ中はデュエルフェーズへ
		//	return;
		//}
		PutCard();
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
	//charaObj_.GetCardUI().ChangeUsedActionCard();
	charaObj_.GetCardUI().ChangeSelectState(CardUIBase::CARD_SELECT::NONE);

	//硬直時間セット
	actionCntl_.GetInput().SetFreezeCntByAttackType();
}

const bool EnemyCardAction::IsJumpAtkCharge(void) const
{
	return jumpChargeCnt_ < JUMP_CHARGE_TIME; 
}

void EnemyCardAction::ChangeStomp(void)
{
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::SWIP_ATK), false);
	isGenerateRock_ = false;
	//ジャンプ攻撃処理
	atk_ = atkStatusTable_[CARD_ACT_TYPE::STOMP_ATK];
	cardFuncs_.push([this]() {UpdateStomp(); });
}

void EnemyCardAction::ChangeRoar(void)
{
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::ROAR_ATK), false, JUMP_ANIM_START);
	//ジャンプ攻撃処理
	atk_ = atkStatusTable_[CARD_ACT_TYPE::ROAR_ATK];
	cardFuncs_.push([this]() {UpdateRoar(); });
}

void EnemyCardAction::ChangeJumpAtk(void)
{
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::JUMP_ATK), false);
	jumpChargeCnt_ = 0.0f;
	isDuelWait_ = true;
	soundMng_.Play(SoundManager::SRC::ENEMY_CHARGE_SE, SoundManager::PLAYTYPE::LOOP);
	//ジャンプ攻撃処理
	atk_ = atkStatusTable_[CARD_ACT_TYPE::JUMP_ATK];
	cardFuncs_.push([this]() {UpdateJumpAtk(); });
}

void EnemyCardAction::ChangeRoleAtk(void)
{
	preRoleAtkCnt_ = ROLE_PRE_TIME;
	roleAtkCnt_ = ROLE_TIME;
	preRolePos_ = charaObj_.GetTransform().pos;
	atk_= atkStatusTable_[CARD_ACT_TYPE::RUSH_ATK];
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::RUSH_ATK), true);
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
	charaObj_.GetCardUI().ChangeSelectState(CardUIBase::CARD_SELECT::RELOAD_WAIT);
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::RUSH_ATK), true);
	cardFuncs_.push([this]() {UpdateReload(); });
}

void EnemyCardAction::ChangeDuel(void)
{
	isDuelWait_ = true;

	//ランダムにデュエルデッキを決める
	deck_.DicideDuelDeck();

	//デュエルdeckから1枚ドロー
	PutCardToDuelDeck();
	cardFuncs_.push([this]() {UpdateDuel(); });
}

void EnemyCardAction::UpdateStomp(void)
{
	////負けたら終了
	if (IsCardFailure(Collider::TAG::NML_ATK))
	{
		scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::NONE);
		charaObj_.ClearEnemyRock();
		return;
	}


	if (anim_.GetAnimStep() > ATTACK_ONE_COL_START_ANIM_CNT)
	{
		const Transform& charaTrans = charaObj_.GetTransform();

		//攻撃中
		atk_.pos = Utility3D::AddPosRotate(charaTrans.pos, charaTrans.quaRot, { 0.0f,0.0f,0.0f });
		//攻撃判定有効
		isAliveAtkCol_ = true;
		charaObj_.MakeAttackCol(charaObj_.GetCharaTag(), Collider::TAG::NML_ATK, atk_.pos, atk_.atkRadius);

		//溜めのカメラシェイク
		cameraShakeCnt_ += SceneManager::GetInstance().GetDeltaTime();
		scnMng_.GetCamera().lock()->SetShakeStatus(cameraShakeCnt_ / STOMP_ATK_SHAKE_CNT, 30.0f);
		scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::SHAKE);

		//地響き音再生
		const bool isPlayStompSE_ = soundMng_.IsPlay(SoundManager::SRC::ENEMY_STOMP_SE);
		if (!isPlayStompSE_)
		{
			soundMng_.Play(SoundManager::SRC::ENEMY_STOMP_SE, SoundManager::PLAYTYPE::BACK);
		}

		//地面から岩の玉を生成してあらゆる方向に飛ばす
		if (!isGenerateRock_)
		{
			charaObj_.SetIsAliveEnemyRock(true);
			isGenerateRock_ = true;
		}
		else
		{
			charaObj_.EnemyRockUpdate();
		}

		//ため時間が終わったら
		if (cameraShakeCnt_ > STOMP_ATK_SHAKE_CNT)
		{
			cameraShakeCnt_ = 0.0f;
			atk_.atkRadius = JUMP_ATK_RADIUS;
			//アニメーションループ終了
			anim_.SetEndMidLoop(CharacterBase::ANIM_SPEED);
			charaObj_.SetIsAliveEnemyRock(false);
			charaObj_.DeleteAttackCol(Collider::TAG::ENEMY1, Collider::TAG::NML_ATK);
			//charaObj_.ClearEnemyRock();
			charaObj_.GetCardUI().ChangeUsedActionCard();
			deck_.EraseHandCard();
			actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		}

	}
}

void EnemyCardAction::UpdateRoar(void)
{
	AttackMotion(atkStatusTable_[actType_],Collider::TAG::ROAR_ATK, JUMP_ATK_LOCAL);
}

void EnemyCardAction::UpdateJumpAtk(void)
{
	////負けたら終了
	if (IsCardFailure(Collider::TAG::NML_ATK))
	{
		scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::NONE);
		soundMng_.Stop(SoundManager::SRC::ENEMY_CHARGE_SE);
		return;
	}

	//ジャンプチャージ
	if (jumpChargeCnt_ < JUMP_CHARGE_TIME)
	{
		jumpChargeCnt_ += SceneManager::GetInstance().GetDeltaTime();
		//アニメーションループ
		anim_.SetMidLoop(JUMP_ATK_ANIM_LOOP_START, JUMP_ATK_ANIM_LOOP_END, JUMP_ATK_ANIM_LOOP_SPEED);

		//溜めのカメラシェイク
		scnMng_.GetCamera().lock()->SetShakeStatus(jumpChargeCnt_ / JUMP_CHARGE_TIME, 10.0f);
		scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::SHAKE);

		if (jumpChargeCnt_ >= JUMP_CHARGE_TIME)
		{
			//アニメーションループ終了
			anim_.SetEndMidLoop(CharacterBase::ANIM_SPEED);
			soundMng_.Stop(SoundManager::SRC::ENEMY_CHARGE_SE);
			charaObj_.GetCardUI().ChangeUsedActionCard();
			deck_.EraseHandCard();
		}
	}

	else if (anim_.GetAnimStep() > JUMP_ANIM_END)
	{
		const Transform& charaTrans = charaObj_.GetTransform();

		cameraShakeCnt_ += SceneManager::GetInstance().GetDeltaTime();
		//攻撃中
		atk_.pos = Utility3D::AddPosRotate(charaTrans.pos, charaTrans.quaRot, { 0.0f,0.0f,0.0f });
		//攻撃判定有効
		isAliveAtkCol_ = true;
		charaObj_.MakeAttackCol(charaObj_.GetCharaTag(), Collider::TAG::NML_ATK, atk_.pos, atk_.atkRadius);

		//攻撃範囲拡大
		atk_.atkRadius = easing_->EaseFunc(JUMP_ATK_RADIUS, JUMP_ATK_GOAL_RADIUS, cameraShakeCnt_ / JUMP_ATK_CNT_MAX, Easing::EASING_TYPE::QUAD_IN);
		charaObj_.UpdateAttackCol(atk_.atkRadius);

		//溜めのカメラシェイク
		scnMng_.GetCamera().lock()->SetShakeStatus(cameraShakeCnt_ / JUMP_ATK_CNT_MAX, 30.0f);
		scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::SHAKE);

		//サウンド再生
		if (!soundMng_.IsPlay(SoundManager::SRC::ENEMY_JUMP_LAND_SE))
		{
			soundMng_.Play(SoundManager::SRC::ENEMY_JUMP_LAND_SE, SoundManager::PLAYTYPE::BACK);
		}


		//アニメーションループ
		anim_.SetMidLoop(53.0f, 69.0f, 10.0f);

		if (cameraShakeCnt_ > JUMP_ATK_CNT_MAX)
		{
			cameraShakeCnt_ = 0.0f;
			atk_.atkRadius = JUMP_ATK_RADIUS;
			//アニメーションループ終了
			anim_.SetEndMidLoop(CharacterBase::ANIM_SPEED);
			charaObj_.DeleteAttackCol(Collider::TAG::ENEMY1, Collider::TAG::NML_ATK);
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
		actionCntl_.GetInput().GetLookAtTargetDir();
		roleMoveDeg_ = actionCntl_.GetInput().GetLookAtTargetDeg();
		roleMoveDir_ = actionCntl_.GetInput().GetLookAtTargetDir();
		roleMoveDir_.y = 0.0f;
		isTurnable_ = true;
		return;
	}
	

	//現在の座標取得
	const Transform trans = charaObj_.GetTransform();
	const VECTOR charaPos = trans.pos;
	const VECTOR& centerPos = charaObj_.GetCharaCenterPos();
	isTurnable_ = false;
	if (preRoleAtkCnt_ < 0.0f)
	{
		roleAtkCnt_ -= deltaTIme;
		//攻撃中
		atk_.pos = Utility3D::AddPosRotate(trans.pos, trans.quaRot, { 0.0f,0.0f,0.0f });
		//転がる間の速度
		speed_ = ROLE_SPEED;
		//当たり判定の作成
		charaObj_.MakeAttackCol(Collider::TAG::ENEMY1, Collider::TAG::NML_ATK, atk_.pos, ROLE_ATK_RADIUS);
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
	charaObj_.GetCardUI().ChangeSelectState(CardUIBase::CARD_SELECT::RELOAD_WAIT);
	actType_ = CARD_ACT_TYPE::NONE;
	actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
}

void EnemyCardAction::UpdateDuel(void)
{
	if (jumpChargeCnt_ < JUMP_CHARGE_TIME)
	{
		if (IsCardFailureJumpCharge())
		{
			//アニメーションループ終了
			anim_.SetEndMidLoop(CharacterBase::ANIM_SPEED);
			charaObj_.GetCardUI().ChangeUsedActionCard();
			deck_.EraseHandCard();
			deck_.ClearDuelDeck();
			actionCntl_.ChangeAction(ActionController::ACTION_TYPE::REACT);
			return;
		}

		jumpChargeCnt_ += SceneManager::GetInstance().GetDeltaTime();
		//アニメーションループ
		anim_.SetMidLoop(JUMP_ATK_ANIM_LOOP_START, JUMP_ATK_ANIM_LOOP_END, JUMP_ATK_ANIM_LOOP_SPEED);

	}
	else if (jumpChargeCnt_ >= JUMP_CHARGE_TIME)
	{
		//アニメーションループ終了
		anim_.SetEndMidLoop(CharacterBase::ANIM_SPEED);
		ChangeCardAction(CARD_ACT_TYPE::JUMP_ATK);
	}
}

bool EnemyCardAction::IsCardFailureJumpCharge(void)
{
	//カードの勝敗判定
	deck_.CardUseUpdate();
	//相手のカードに負けたらノックバックする
	if (jampCardNum_ >= JAMP_CHARGE_CARD_NUM_MAX)
	{
		actionCntl_.GetInput().IsActioningSet();
		charaObj_.GetCardUI().ChangeReactActionCard();
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::REACT);
		return true;
	}
	if (deck_.IsCardFailure())
	{
		charaObj_.GetCardUI().ChangeReactActionCard();
		//攻撃判定無効
		deck_.EraseHandCard(true);
	}
	else if (deck_.IsNone())
	{
		PutCard();
		jampCardNum_++;
	}
	return false;
}

void EnemyCardAction::DesideCardAction(void)
{
	//ロジックから攻撃タイプを取得
	//LogicBase::ENEMY_ATTACK_TYPE attackType = actionCntl_.GetInput().GetAttackType();
	LogicBase::ENEMY_ATTACK_TYPE attackType = LogicBase::ENEMY_ATTACK_TYPE::STOMP;
	switch (attackType)
	{
	case LogicBase::ENEMY_ATTACK_TYPE::STOMP:
		ChangeCardAction(CARD_ACT_TYPE::STOMP_ATK);
		break;
	case LogicBase::ENEMY_ATTACK_TYPE::JUMP:
		ChangeCardAction(CARD_ACT_TYPE::JUMP_ATK);		//ジャンプチャージ中はデュエルフェーズへ
		break;
	case LogicBase::ENEMY_ATTACK_TYPE::ROAR:
		ChangeCardAction(CARD_ACT_TYPE::ROAR_ATK);
		break;
	case LogicBase::ENEMY_ATTACK_TYPE::ROLE:
		ChangeCardAction(CARD_ACT_TYPE::RUSH_ATK);
		break;
	}
}

void EnemyCardAction::PutCardToDuelDeck(void)
{
	deck_.MoveUsingCardToDuelDeck();
	//charaObj_.GetCardUI().ChangeSelectState(CardUIBase::CARD_SELECT::DISITION);
}

