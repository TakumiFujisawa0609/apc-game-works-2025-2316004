#include"../Utility/Utility3D.h"
#include"../Utility/UtilityCommon.h"
#include"Player.h"
#include"./InputController.h"
//#include "../../Manager/Game/GravityManager.h"
#include "../../../Manager/Generic/Camera.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Resource/ResourceManager.h"

#include"../Object/Card/CardSystem.h"

//#include "../../Object/Common/EffectController.h"
#include "../../../Object/Common/AnimationController.h"

#include"../../Card/CardDeck.h"
#include"../../Card/CardBase.h"

#include"../Base/ActionBase.h"
#include"../Action/Idle.h"
#include"../Action/Run.h"
#include"../Action/Jump.h"

#include "PlayerAction.h"

PlayerAction::PlayerAction(InputBase& _input, Transform& _trans, CardDeck& _deck, InputManager::JOYPAD_NO _padNum):
	input_(_input)
	,trans_(_trans)
	,deck_(_deck)
	,padNum_(_padNum)
	,scnMng_(SceneManager::GetInstance())
{
	//エフェクト
	//effect_ = std::make_unique<EffectController>();
}

PlayerAction::~PlayerAction(void)
{
	StopResource();
}

void PlayerAction::Init(void)
{
	//auto cntl = player_.GetCntl();
	auto cntl = InputManager::CONTROLL_TYPE::ALL;

	actionObject_[ACTION_TYPE::NONE]= std::make_unique<Idle>(input_);
	actionObject_[ACTION_TYPE::INPUT]= std::make_unique<Idle>(input_);
	actionObject_[ACTION_TYPE::MOVE] = std::make_unique<Run>(input_);
	actionObject_[ACTION_TYPE::JUMP] = std::make_unique<Jump>(input_);
	actionObject_[ACTION_TYPE::CARD_ACTION] = std::make_unique<Idle>(input_);

	act_ = ACTION_TYPE::NONE;
	changeAction_.emplace(ACTION_TYPE::NONE, [this]() {ChangeInput();});
	changeAction_.emplace(ACTION_TYPE::MOVE, [this]() {ChangeMove();});
	changeAction_.emplace(ACTION_TYPE::INPUT, [this]() {ChangeInput(); });
	changeAction_.emplace(ACTION_TYPE::JUMP, [this]() {ChangeJump(); });
	changeAction_.emplace(ACTION_TYPE::CARD_ACTION, [this]() {ChangeCardUse(); });
	ChangeAction(ACTION_TYPE::INPUT);


	isCardAct_ = false;
	cardActTime_ = 0.0f;
	//スピード
	speed_ = 0.0f;
}

void PlayerAction::Load(void)
{

}

void PlayerAction::Update(void)
{
	////エフェクト更新
	//effect_->Update();

	//プレイヤーの下を設定
	static VECTOR dirDown = trans_.GetDown();

	////重力(各アクションに重力を反映させたいので先に重力を先に書く)
	//GravityManager::GetInstance().CalcGravity(dirDown,jumpPow_, GRAVITY_PER);

	//各アクションの更新
	actionUpdate_();
	//action_->Update();
	actionObject_[act_]->Update();

	CardChargeUpdate();
	CardMove();
}

void PlayerAction::DrawDebug(void)
{
	//int dashSeCnt = effect_->GetPlayNum(EffectController::EFF_TYPE::DASH);
	//DrawFormatString(0, 300, 0x000000, "act(%d)\ndashSESize(%d)", (int)input_.GetAct(), dashSeCnt);
	deck_.Draw();
	
}

void PlayerAction::NoneUpdate(void)
{
	//何もしない
}

void PlayerAction::ChangeNone(void)
{
	actionUpdate_ = [this]() {NoneUpdate(); };
}

void PlayerAction::ActionInputUpdate(void)
{
	//入力に応じてアクションを変える
	//移動
	using ACT_CNTL = InputBase::ACT_CNTL;
	if (input_.GetMoveDeg() >= 0.0f)
	{
		ChangeAction(ACTION_TYPE::MOVE);
		return;
	}
	if (input_.GetIsAct().isCardUse)
	{
		ChangeAction(ACTION_TYPE::CARD_ACTION);
	}

}

void PlayerAction::ChangeAction(ACTION_TYPE _act)
{
	if (act_ == _act)return;
	act_ = _act;
	changeAction_[act_]();
 }

const VECTOR PlayerAction::GetMovePow(void)
{
	return actionObject_[act_]->GetMovePow();
}

void PlayerAction::ChangeInput(void)
{
	actionUpdate_ = std::bind(&PlayerAction::ActionInputUpdate, this);
}



void PlayerAction::MoveUpdate(void)
{
	//移動中に入力が入った時の状態遷移
	if (input_.GetMoveDeg() < 0.0f)
	{
		ChangeAction(ACTION_TYPE::INPUT);
		return;
	}
}


void PlayerAction::ChangeMove(void)
{
	speed_ = MOVE_SPEED;
	//animationController_.Play(static_cast<int>(Player::ANIM_TYPE::WALK));
	actionUpdate_ = std::bind(&PlayerAction::MoveUpdate, this);
}

void PlayerAction::ChangeDashMove(void)
{
	//animationController_.Play(static_cast<int>(Player::ANIM_TYPE::WALK));

	const float SCL = 10.0f;
	//effect_->Play(EffectController::EFF_TYPE::DASH, trans.pos, trans.quaRot, { SCL,SCL,SCL }, false, 1.0f);
	actionUpdate_ = [this]() {MoveUpdate(); };
}

void PlayerAction::ChangeJump(void)
{
	//エフェクトのスケール
	const VECTOR EFF_SCL = { 10.0f,10.0f,10.0f };

	//状態遷移
	actionUpdate_ = [this]() {JumpUpdate(); };
}



void PlayerAction::UpdateMoveDirAndPow(void)
{
	//方向の更新
	moveDir_ = dir_;
	//移動量の更新
	movePow_ = VScale(moveDir_, speed_);
	
}

void PlayerAction::JumpUpdate(void)
{
	//ここでジャンプ終了を受け取って状態遷移
}


void PlayerAction::ChangeCardUse(void)
{
	//手札に移動
	deck_.MoveHandToCharge();

	//アクション中にする
	isCardAct_ = true;

	//カード使用状態へ
	actionUpdate_ = [this]() {CardUseUpdate(); };
}

void PlayerAction::CardUseUpdate(void)
{
	if (cardActTime_ < CARD_ACT_TIME_MAX)
	{
		cardActTime_ += SceneManager::GetInstance().GetDeltaTime();
		deck_.CardUseUpdate();
	}
	else
	{
		//アクション終了
		isCardAct_ = false;
		cardActTime_ = 0.0f;
		ChangeAction(ACTION_TYPE::INPUT);
		return;
	}
	
}

void PlayerAction::CardChargeUpdate(void)
{
	if (input_.GetIsAct().isCardCharge)
	{
		deck_.CardCharge();
	}
	
}

void PlayerAction::CardMove(void)
{
	if (input_.GetIsAct().isCardMoveLeft)
	{
		deck_.CardMoveLeft();
	}
	else if (input_.GetIsAct().isCardMoveRight)
	{
		deck_.CardMoveRight();
	}
}

bool PlayerAction::CheckJumpInput(void)
{
	return input_.CheckAct(InputBase::ACT_CNTL::JUMP);
}

void PlayerAction::StopResource(void)
{

}
