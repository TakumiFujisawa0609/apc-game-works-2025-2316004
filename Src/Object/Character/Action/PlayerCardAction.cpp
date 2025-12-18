#include "../Utility/Utility3D.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
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
	pushReloadCnt_(),
	soundMng_(SoundManager::GetInstance())
{
	isTurnable_ = false;
	changeAction_={
		{ CARD_ACT_TYPE::ATTACK_ONE, [this]() {ChangeAttackOne(); }},
		{ CARD_ACT_TYPE::ATTACK_TWO, [this]() {ChangeAttackTwo(); }},
		{ CARD_ACT_TYPE::ATTACK_THREE, [this]() {ChangeAttackThree(); }},
		{ CARD_ACT_TYPE::RELOAD, [this]() {ChangeReload(); }},
		{ CARD_ACT_TYPE::DUEL_FAZE, [this]() {ChangeDuel(); }},
	};
	ATK_STATUS initStatus={};
	atkStatusTable_ = {
		{CARD_ACT_TYPE::ATTACK_ONE,NORMAL_ATK_ONE},
		{CARD_ACT_TYPE::ATTACK_TWO,NORMAL_ATK_TWO},
		{CARD_ACT_TYPE::ATTACK_THREE,NORMAL_ATK_THREE},
		{CARD_ACT_TYPE::RELOAD,initStatus}		//攻撃判定がないものは初期値を入れる
	};

	atk_ = {};
}

PlayerCardAction::~PlayerCardAction(void)
{
	changeAction_.clear();
	//カード機能配列の解放
	std::queue<std::function<void(void)>> empty;
	std::swap(cardFuncs_, empty);
}

void PlayerCardAction::Load(void)
{
	soundMng_.LoadResource(SoundManager::SRC::CARD_RELOAD);
	soundMng_.LoadResource(SoundManager::SRC::CARD_RELOAD_FINISH);
}

void PlayerCardAction::Init(void)
{
	//カードの属性を受け取ってアニメーションを再生
	std::vector<CardBase::CARD_TYPE>cardTypes = deck_.GetHandCardType();
	attackStageNum_ = 0;

	//if (actionCntl_.GetInput().GetIsEnemyJumpCharge())
	//{
	//	ChangeCardAction(CARD_ACT_TYPE::DUEL_FAZE);
	//}
	//手札に移動
	if (deck_.GetDrawCardType() == CardBase::CARD_TYPE::ATTACK)
	{
		PutCard();
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
	//charaObj_.GetCardUI().ChangeUsedActionCard();

	//リロード処理中ならカードUI状態をNoneにする
	if(actType_==CARD_ACT_TYPE::RELOAD)
	{
		charaObj_.GetCardUI().ChangeSelectState(CardUIBase::CARD_SELECT::NONE);
	}

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

void PlayerCardAction::UpdateAttackThree(void)
{
	//攻撃中にカード負けしたら処理を飛ばす
	if (IsCardFailure(Collider::TAG::NML_ATK))return;
	const float animStep = anim_.GetAnimStep();			//現在のアニメステップ
	const float atkStartStep = atkStatusTable_[actType_].colStartCnt;
	const float atkEndStep = atkStatusTable_[actType_].colEndCnt;

	if (anim_.GetAnimStep() < atkStatusTable_[actType_].colStartCnt)
	{
		anim_.SetAnimSpeed(ATTACK_THREE_ANIM_SPD);
	}

	//攻撃判定処理
	else if (animStep >= atkStartStep &&
		animStep <= atkEndStep)
	{
		atkAnimLerpCnt_ += scnMng_.GetDeltaTime();
		//アニメーション速度補完
		anim_.SetAnimSpeed(CharacterBase::ANIM_SPEED, true, ATTACK_THREE_ANIM_SPD, atkAnimLerpCnt_ / ATTACK_THREE_ANIM_LERP_TIME,Easing::EASING_TYPE::QUAD_IN);
		////攻撃当たり判定の座標生成
		//atkPos_ = Utility3D::AddPosRotate(charaObj_.GetTransform().pos, charaObj_.GetTransform().quaRot, ATK_ONE_LOCAL);
		//攻撃判定有効
		isAliveAtkCol_ = true;
		charaObj_.MakeAttackCol(charaObj_.GetCharaTag(), Collider::TAG::NML_ATK, {}, 0.0f);

	}
	else if (anim_.IsEnd())		//アニメーション終了でアイドル状態変更
	{
		const float ATK_END_CNT = 0.5f;
		if (atkThreeEndCnt_ > ATK_END_CNT)
		{
			charaObj_.GetCardUI().ChangeUsedActionCard();
			actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
			return;
		}
		atkThreeEndCnt_+= scnMng_.GetDeltaTime();
		scnMng_.GetCamera().lock()->SetShakeStatus(atkThreeEndCnt_ / ATK_END_CNT, 50.0f, Easing::EASING_TYPE::ELASTIC_BACK);
		scnMng_.GetCamera().lock()->ChangeSub(Camera::SUB_MODE::ONE_SHAKE);

	}
	else if (animStep > atkEndStep)	//攻撃終了後
	{
		//攻撃判定無効
		charaObj_.DeleteAttackCol(charaObj_.GetCharaTag(), Collider::TAG::NML_ATK);
	}
}

void PlayerCardAction::UpdateReload(void)
{
	if (actionCntl_.GetInput().GetIsAct().isCardPushKeep)
	{
		pushReloadCnt_ += scnMng_.GetDeltaTime();
		float per = pushReloadCnt_ / RELOAD_TIME;
		charaObj_.GetCardUI().SetReloadCount(per);

		//アニメーションループ
		anim_.SetMidLoop(RELOAD_LOOP_START, RELOAD_LOOP_END, 10.0f);
	}
	else
	{
		cardFuncs_.pop();
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
		//カードリロード音停止
		soundMng_.Stop(SoundManager::SRC::CARD_RELOAD);
		actType_ = CARD_ACT_TYPE::NONE;
		charaObj_.GetCardUI().ChangeSelectState(CardUIBase::CARD_SELECT::NONE);
	}
	if (pushReloadCnt_ >= RELOAD_TIME)
	{
		deck_.Reload();
		cardFuncs_.pop();
		actType_ = CARD_ACT_TYPE::NONE;
		pushReloadCnt_ = 0.0f;
		//カードリロード音停止、完了音再生
		soundMng_.Stop(SoundManager::SRC::CARD_RELOAD);
		soundMng_.Play(SoundManager::SRC::CARD_RELOAD_FINISH,SoundManager::PLAYTYPE::BACK);
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
	}
}

void PlayerCardAction::UpdateSonicRave(void)
{
}

void PlayerCardAction::UpdateDuel(void)
{
	//カードの勝敗による処理
	bool w = deck_.IsCardWin();
	bool f = deck_.IsCardFailure();
	if (w)
	{
		//チャージカードに移動
		deck_.MoveChargeToUsingCard();
	}
	else if(f)
	{
		//負けたらカードを削除
		deck_.EraseHandCard();
		charaObj_.DeleteCard();

	}

	//カードをドローする
	LogicBase& logic = actionCntl_.GetInput();
	if (logic.GetIsAct().isCardUse && logic.GetJumpCardNum()< JAMP_CHARGE_CARD_NUM_MAX)
	{
		deck_.MoveUsingCardToDrawPile();
	}
	else if (logic.GetJumpCardNum() >= JAMP_CHARGE_CARD_NUM_MAX)
	{
		//一定回数カードに勝ったら大技を出す(未実装)
		actionCntl_.ChangeAction(ActionController::ACTION_TYPE::IDLE);
	}
}


void PlayerCardAction::ChangeAttackOne(void)
{
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_1), false);
	atk_ = NORMAL_ATK_ONE;
	cardFuncs_.push([this]() {UpdateAttack(); });
}

void PlayerCardAction::ChangeAttackTwo(void)
{
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_2), false);
	//攻撃段階を増やす
	ChangeActionCardInit();
	atk_ = NORMAL_ATK_TWO;
	cardFuncs_.push([this]() {UpdateAttack(); });
}

void PlayerCardAction::ChangeAttackThree(void)
{
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::ATTACK_3), false,60.0f,86.0f);
	atkThreeEndCnt_ = 0.0f;
	atkAnimLerpCnt_ = 0.0f;
	atk_ = NORMAL_ATK_THREE;
	ChangeActionCardInit();
	cardFuncs_.push([this]() {UpdateAttackThree(); });
}

void PlayerCardAction::ChangeReload(void)
{
	if (!cardFuncs_.empty())
	{
		cardFuncs_.pop();
	}
	//現在使っているカードを捨てる
	deck_.EraseHandCard();
	anim_.Play(static_cast<int>(CharacterBase::ANIM_TYPE::CARD_RELOAD), true, RELOAD_START_STEP, RELOAD_END_STEP);
	//リロード音再生
	soundMng_.Play(SoundManager::SRC::CARD_RELOAD, SoundManager::PLAYTYPE::LOOP);
	float per = pushReloadCnt_ / RELOAD_TIME;
	charaObj_.GetCardUI().SetReloadCount(per);
	charaObj_.GetCardUI().ChangeSelectState(CardUIBase::CARD_SELECT::RELOAD_WAIT);
	
	cardFuncs_.push([this]() {UpdateReload(); });
}

void PlayerCardAction::ChangeSonicRave(void)
{
}

void PlayerCardAction::ChangeDuel(void)
{
	isDuelWait_ = true;
	cardFuncs_.push([this]() {UpdateDuel(); });
}

void PlayerCardAction::ChangeComboAction(void)
{
	if (deck_.GetDrawCardType() == CardBase::CARD_TYPE::RELOAD && actionCntl_.GetInput().GetIsAct().isCardUse)
	{
		if (charaObj_.GetCardUI().GetSelectState() == CardUIBase::CARD_SELECT::NONE)
		{
			ChangeCardAction(CARD_ACT_TYPE::RELOAD);
		}
		
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
