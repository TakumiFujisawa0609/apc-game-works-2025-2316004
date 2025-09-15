#include"../Utility/Utility3D.h"
#include"../Utility/UtilityCommon.h"
#include"Player.h"
#include"./PlayerInput.h"
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

PlayerAction::PlayerAction(Player& _player, SceneManager& _scnMng, AnimationController& _animationController):
	player_(_player)
	,scnMng_(_scnMng)
	,animationController_(_animationController)
{
	//エフェクト
	//effect_ = std::make_unique<EffectController>();
	input_ = nullptr;
}

PlayerAction::~PlayerAction(void)
{
	StopResource();
}

void PlayerAction::Init(void)
{
	auto num = player_.GetPadNum();
	//auto cntl = player_.GetCntl();
	auto cntl = InputManager::CONTROLL_TYPE::ALL;
	//入力
	input_ = std::make_unique<PlayerInput>(num, cntl);
	input_->Init();

	//操作関連
//----------------------------------------------------
	changeAction_.emplace(ATK_ACT::NONE, [this]() {
		action_ = std::make_shared<Idle>(*input_);
		ChangeInput();
		});
	changeAction_.emplace(ATK_ACT::MOVE, [this]() {
		action_ = std::make_shared<Run>(*input_);
		ChangeMove();
		});
	changeAction_.emplace(ATK_ACT::INPUT, [this]() {
		action_ = std::make_shared<Idle>(*input_);
		ChangeInput(); 
		});
	changeAction_.emplace(ATK_ACT::JUMP, [this]() {
		action_ = std::make_shared<Jump>(*input_);
		ChangeJump(); });
	changeAction_.emplace(ATK_ACT::CARD_USE, [this]() {ChangeCardUse(); });


	//カードデッキ
	cardCenterPos_ = { 140,140 };//カードの中心位置
	deck_ = std::make_shared<CardDeck>(cardCenterPos_,PLAYER_NUM);
	//デッキに山札追加
	for (int i = 0; i < CARD_NUM_MAX; i++)
	{
		deck_->AddDrawPile(CARD_POWS[i]);
	}
	deck_->Init();

	isCardAct_ = false;
	cardActTime_ = 0.0f;
	//スピード
	speed_ = 0.0f;

	act_ = ATK_ACT::NONE;
	ChangeAction(ATK_ACT::INPUT);
}

void PlayerAction::Load(void)
{

}

void PlayerAction::Update(void)
{
	//入力更新
	input_->Update();

	////エフェクト更新
	//effect_->Update();

	//プレイヤーの下を設定
	static VECTOR dirDown = player_.GetTransform().GetDown();

	////重力(各アクションに重力を反映させたいので先に重力を先に書く)
	//GravityManager::GetInstance().CalcGravity(dirDown,jumpPow_, GRAVITY_PER);

	//各アクションの更新
	actionUpdate_();
	action_->Update();

	CardChargeUpdate();
	CardMove();
}

void PlayerAction::DrawDebug(void)
{
	//int dashSeCnt = effect_->GetPlayNum(EffectController::EFF_TYPE::DASH);
	//DrawFormatString(0, 300, 0x000000, "act(%d)\ndashSESize(%d)", (int)input_->GetAct(), dashSeCnt);
	deck_->Draw();
	
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
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	if (input_->CheckAct(ACT_CNTL::MOVE))
	{
		ChangeAction(ATK_ACT::MOVE);
		return;
	}
	if (input_->CheckAct(ACT_CNTL::CARD_USE))
	{
		ChangeAction(ATK_ACT::CARD_USE);
	}

}

void PlayerAction::ChangeAction(ATK_ACT _act)
{
	if (act_ == _act)return;
	act_ = _act;
	changeAction_[act_]();
 }

const VECTOR PlayerAction::GetMovePow(void)
{
	return action_->GetMovePow();
}

void PlayerAction::ChangeInput(void)
{
	actionUpdate_ = std::bind(&PlayerAction::ActionInputUpdate, this);
}



void PlayerAction::MoveUpdate(void)
{
	//移動中に入力が入った時の状態遷移

	if (input_->CheckAct(PlayerInput::ACT_CNTL::MOVE))
	{
		ChangeAction(ATK_ACT::MOVE);
	}

	else if (!input_->CheckAct(PlayerInput::ACT_CNTL::MOVE)
		&&!input_->CheckAct(PlayerInput::ACT_CNTL::DASHMOVE))
	{
		speed_ = 0.0f;
		ChangeAction(ATK_ACT::INPUT);
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
	speed_ = DASH_SPEED;
	//animationController_.Play(static_cast<int>(Player::ANIM_TYPE::WALK));

	auto& trans = player_.GetTransform();
	const float SCL = 10.0f;
	//effect_->Play(EffectController::EFF_TYPE::DASH, trans.pos, trans.quaRot, { SCL,SCL,SCL }, false, 1.0f);
	actionUpdate_ = [this]() {MoveUpdate(); };
}

void PlayerAction::ChangeJump(void)
{
	//ジャンプ関係
	isJump_ = true;
	stepJump_ = 0.0f;

	//プレイヤーの情報
	Transform trans = player_.GetTransform();
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
	deck_->MoveHandToCharge();

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
		deck_->CardUseUpdate();
	}
	else
	{
		//アクション終了
		isCardAct_ = false;
		cardActTime_ = 0.0f;
		ChangeAction(ATK_ACT::INPUT);
		return;
	}
	
}

void PlayerAction::CardChargeUpdate(void)
{
	if (input_->CheckAct(PlayerInput::ACT_CNTL::CARD_CHARGE))
	{
		deck_->CardCharge();
	}
	
}

void PlayerAction::CardMove(void)
{
	if (input_->CheckAct(PlayerInput::ACT_CNTL::CARD_MOVE_LEFT))
	{
		deck_->CardMoveLeft();
	}
	else if (input_->CheckAct(PlayerInput::ACT_CNTL::CARD_MOVE_RIGHT))
	{
		deck_->CardMoveRight();
	}
}

bool PlayerAction::CheckJumpInput(void)
{
	return input_->CheckAct(PlayerInput::ACT_CNTL::JUMP);
}

void PlayerAction::StopResource(void)
{

}
