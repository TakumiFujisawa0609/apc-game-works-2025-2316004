#include"../Utility/Utility3D.h"
#include"../Utility/UtilityCommon.h"
#include"Player.h"
#include"Process/PlayerInput.h"
//#include "../../Manager/Game/GravityManager.h"
#include "../../Manager/Generic/Camera.h"
#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Resource/ResourceManager.h"
//#include "../../Object/Common/EffectController.h"
#include "../../Object/Common/AnimationController.h"

#include"../Card/CardDeck.h"
#include"../Card/CardBase.h"

#include "PlayerAction.h"

PlayerAction::PlayerAction(Player& _player, SceneManager& _scnMng, AnimationController& _animationController):
	player_(_player)
	,scnMng_(_scnMng)
	,animationController_(_animationController)
{
	//操作関連
	//----------------------------------------------------
	changeAction_.emplace(ATK_ACT::NONE, [this]() {ChangeNone(); });
	changeAction_.emplace(ATK_ACT::MOVE, [this]() {ChangeMove(); });
	changeAction_.emplace(ATK_ACT::INPUT, [this]() {ChangeInput(); });
 	changeAction_.emplace(ATK_ACT::JUMP, [this]() {ChangeJump(); });

	//エフェクト
	//effect_ = std::make_unique<EffectController>();
	
	//ジャンプ関係
	isJump_ = false;
	stepJump_ = 0.0f;
	jumpPow_ = Utility3D::VECTOR_ZERO;
	jumpDeceralation_ = POW_JUMP;
	stepRotTime_ = 0.0f;
	speed_ = 0.0f;
	punchPos_ = {};
	punchedCnt_ = 0.0f;
	punchCoolCnt_ = 0.0f;

	isPunchHitTime_ = false;
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

	//カードデッキ
	cardCenterPos_ = { 140,140 };//カードの中心位置
	deck_ = std::make_shared<CardDeck>(cardCenterPos_,PLAYER_NUM);
	//デッキに山札追加
	for (int i = 0; i < CARD_NUM_MAX; i++)
	{
		deck_->AddDrawPile(CARD_POWS[i]);
	}
	deck_->Init();

	//ジャンプ関係
	isJump_ = false;
	stepJump_ = 0.0f;
	jumpPow_ = Utility3D::VECTOR_ZERO;
	jumpDeceralation_ = POW_JUMP;
  	movePow_ = Utility3D::VECTOR_ZERO;

	//スピード
	speed_ = 0.0f;

	isPunchHitTime_ = false;

	//if (scnMng_.GetInstance().GetSceneID() == SceneManager::SCENE_ID::TITLE)
	//{
	//	cameraNo_ = 0;
	//}
	//else
	//{
	//	cameraNo_ = player_.GetPlayerNum();
	//}
	act_ = ATK_ACT::NONE;
	ChangeAction(ATK_ACT::INPUT);
}

void PlayerAction::Load(void)
{
	//auto& res = ResourceManager::GetInstance();
	//actSE_.emplace(ACT_SE::DASH,SoundManager::SRC::PLAYER_DASH_START);
	//actSE_.emplace(ACT_SE::JUMP,SoundManager::SRC::PLAYER_JUMP);
	//actSE_.emplace(ACT_SE::PUNCH, SoundManager::SRC::PLAYER_PUNCH_MOTION);
	//actSE_.emplace(ACT_SE::SLIME, SoundManager::SRC::SLIME_SE);
	//actSE_.emplace(ACT_SE::PUNCH_HIT, SoundManager::SRC::PLAYER_PUNCH_HIT);
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

	//プレイヤーの回転
	Rotate();

	//プレイヤーの方向とスピードの更新
	UpdateMoveDirAndPow();

	//カード関連
	CardUseUpdate();
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
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	if (input_->CheckAct(ACT_CNTL::MOVE))
	{
		ChangeAction(ATK_ACT::MOVE);
		return;
	}
}

void PlayerAction::ChangeAction(ATK_ACT _act)
{
	if (act_ == _act)return;
	act_ = _act;
	changeAction_[act_]();
 }

void PlayerAction::ChangeInput(void)
{
	actionUpdate_ = std::bind(&PlayerAction::ActionInputUpdate, this);
}



void PlayerAction::MoveUpdate(void)
{
	Speed();
	//移動中に入力が入った時の状態遷移
	if (CheckJumpInput())
	{
		ChangeAction(ATK_ACT::JUMP);
		return;
	}
	if (input_->CheckAct(PlayerInput::ACT_CNTL::MOVE))
	{
		ChangeAction(ATK_ACT::MOVE);
	}
	if (input_->CheckAct(PlayerInput::ACT_CNTL::DASHMOVE))
	{
		ChangeAction(ATK_ACT::DASHMOVE);
	}
	else if (input_->CheckAct(PlayerInput::ACT_CNTL::PUNCH))
	{
		speed_ = 0.0f;
		ChangeAction(ATK_ACT::PUNCH);
		return;
	}
	else if (!input_->CheckAct(PlayerInput::ACT_CNTL::MOVE)
		&&!input_->CheckAct(PlayerInput::ACT_CNTL::DASHMOVE))
	{
		speed_ = 0.0f;
		ChangeAction(ATK_ACT::INPUT);
		return;
	}

	float animationSpeed = Player::DEFAULT_ANIM_SPD * (speed_ / MOVE_SPEED)*3.0f;

	//入力方向の移動
	MoveDirFronInput();
}

void PlayerAction::MoveDirFronInput(void)
{
	//移動量を0にリセット
	movePow_ = Utility3D::VECTOR_ZERO;

	//プレイヤー入力クラスから角度を取得
	VECTOR getDir = input_->GetDir();
	float deg = input_->GetMoveDeg();

	//カメラの角度ど入力角度でプレイヤーの方向を変える
	Quaternion cameraRot = scnMng_.GetCamera().lock()->GetQuaRotOutX();
	dir_ = cameraRot.PosAxis(getDir);
	dir_ = VNorm(dir_);

	if (!Utility3D::EqualsVZero(dir_))
	{
		//補完角度の設定(入力角度まで方向転換する)
		SetGoalRotate(deg);
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



void PlayerAction::UpdateMoveDirAndPow(void)
{
	//方向の更新
	moveDir_ = dir_;
	//移動量の更新
	movePow_ = VScale(moveDir_, speed_);
	
}

void PlayerAction::Speed(void)
{
	if (input_->CheckAct(PlayerInput::ACT_CNTL::MOVE))
	{
		speed_ = MOVE_SPEED;
	}
	else if (input_->CheckAct(PlayerInput::ACT_CNTL::DASHMOVE))
	{
		speed_ = DASH_SPEED;
	}
	else
	{
		speed_ = 0.0f;
	}
}

void PlayerAction::JumpUpdate(void)
{
	//移動入力があったらスピードセット
	Speed();
	//ジャンプ処理
	Jump();
}

void PlayerAction::Jump(void)
{
	//ステップジャンプを基準にジャンプ減衰量を決める
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();
	stepJump_ += deltaTime;

	////空中アニメーションステップのループ設定
	//animationController_.SetEndLoop(JUMP_ANIM_LOOP_START_FRAME
	//	, JUMP_ANIM_LOOP_END_FRAME, JUMP_ANIM_ATTACK_BLEND_TIME);

	//ジャンプ中も移動できるようにする
	MoveDirFronInput();

	//ジャンプカウントが0以上なら
	if (stepJump_ > 0.0f)
	{
		stepJump_ += deltaTime;
		//プレイヤーが落下していたら
		if (jumpDeceralation_ < 0.0f)
		{
			//animationController_.Play(static_cast<int>(Player::ANIM_TYPE::LAND));
		}
		//減衰量の計算
		float deceralation = stepJump_ * TIME_JUMP_SCALE;
		jumpDeceralation_ -= deceralation;

		//ジャンプ量に掛ける
		jumpPow_ = VScale(player_.GetTransform().GetUp(), jumpDeceralation_);
	}

	//地面に着いたらジャンプ関係の変数リセット
	if (!isJump_)
	{
		jumpDeceralation_ = POW_JUMP;
		jumpPow_ = Utility3D::VECTOR_ZERO;
		stepJump_ = 0.0f;

		//着地エフェクト
		Transform trans = player_.GetTransform();
		const float EFF_SCL = 30.0f;

		//動いていた場合の移動量リセット
		speed_ = 0.0f;
		ChangeAction(ATK_ACT::INPUT);
		return;
	}
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

void PlayerAction::CardUseUpdate(void)
{
	if (input_->CheckAct(PlayerInput::ACT_CNTL::_CARD_USE))
	{
		deck_->CardUse();
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

void PlayerAction::Rotate(void)
{
	stepRotTime_ -= SceneManager::GetInstance().GetDeltaTime();
	// 回転の球面補間
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

void PlayerAction::SetGoalRotate(double _deg)
{
	//カメラの角度を取得
	VECTOR cameraRot = scnMng_.GetCamera().lock()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis(
		(double)cameraRot.y + UtilityCommon::Deg2RadD(_deg), Utility3D::AXIS_Y);


	 //現在設定されている回転との角度差を取る
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	constexpr double ANGLE_THRESHOLD = 0.1;
	// しきい値
	if (angleDiff > ANGLE_THRESHOLD)
	{
		stepRotTime_ = TIME_ROT;
	}
	goalQuaRot_ = axis;
}

void PlayerAction::StopResource(void)
{
	//SEの停止
	//SoundManager::GetInstance().Stop(actSE_[ACT_SE::DASH]);
	//SoundManager::GetInstance().Stop(actSE_[ACT_SE::JUMP]);
	//SoundManager::GetInstance().Stop(actSE_[ACT_SE::PUNCH]);
	//SoundManager::GetInstance().Stop(actSE_[ACT_SE::SLIME]);
	//SoundManager::GetInstance().Stop(actSE_[ACT_SE::PUNCH_HIT]);
	//エフェクトの停止
	//effect_->StopAll();
}
