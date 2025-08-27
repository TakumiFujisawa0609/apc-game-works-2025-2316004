#include "../../Utility/Utility3D.h"
#include "../../Utility/UtilityCommon.h"
#include "../Application.h"

//#include "../../Manager/Game/GravityManager.h"
#include "../../Manager/Game/PlayerManager.h"
#include "../../Manager/Resource/ResourceManager.h"
//#include "../../Manager/System/SoundManager.h"
#include "../../Manager/Generic/SceneManager.h"
//#include"../../Manager/System/DateBank.h"

#include "../../Manager/Generic/Camera.h"

//#include "../../Renderer/ModelMaterial.h"
//#include "../../Renderer/ModelRenderer.h"

//#include "../../Object/Common/Geometry/Sphere.h"
//#include "../../Object/Common/Geometry/Line.h"
//#include"../../Object/Common/Geometry/Model.h"
//#include"../../Object/Common/EffectController.h"

#include "../../Object/Common/AnimationController.h"
#include"./PlayerAction.h"
//#include"./PlayerOnHit.h"
#include "./Process/PlayerInput.h"
#include<algorithm>


#include "Player.h"
Player::Player(int _playerNum, InputManager::CONTROLL_TYPE _cntl)
	:playerNum_(_playerNum)
	, cntl_(_cntl)
{
	trans_ = Transform();

	//初めのJOYPADがkey_padなのでパッドの番号に合わせる
	//パッド番号を設定
	padNum_ = static_cast<InputManager::JOYPAD_NO>(playerNum_ + 1);

	//プレイヤー状態
	changeStates_.emplace(PLAYER_STATE::ALIVE, [this]() {ChangeAlive();});
	changeStates_.emplace(PLAYER_STATE::DEATH, [this]() {ChangeDeath(); });
	changeStates_.emplace(PLAYER_STATE::GOAL, [this]() {ChangeGoal(); });

	ChangeState(PLAYER_STATE::ALIVE);
}

Player::~Player(void)
{
}

void Player::Load(void)
{
	////アニメーションでmodelIdを使うので先にモデルセットする
	//trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::CHICKEN));

	////リソースの読み込みなど
	//animationController_ = std::make_unique<AnimationController>(trans_.modelId);
	//animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE), DEFAULT_ANIM_SPD);
	//animationController_->Add(static_cast<int>(ANIM_TYPE::WALK), DEFAULT_ANIM_SPD);
	//animationController_->Add(static_cast<int>(ANIM_TYPE::FALL), DEFAULT_ANIM_SPD);
	//animationController_->Add(static_cast<int>(ANIM_TYPE::JUMP), DEFAULT_ANIM_SPD);
	//animationController_->Add(static_cast<int>(ANIM_TYPE::LAND), DEFAULT_ANIM_SPD);
	//animationController_->Add(static_cast<int>(ANIM_TYPE::GOAL), DEFAULT_ANIM_SPD);
	//animationController_->Add(static_cast<int>(ANIM_TYPE::PUNCH), DEFAULT_ANIM_SPD / PlayerAction::PUNCH_TIME_MAX);

	//アクション
	action_ = std::make_unique<PlayerAction>(*this, scnMng_, *animationController_);
	action_->Load();
}

void Player::Init(void)
{
	//Transformの設定
	trans_.quaRot = Quaternion();
	trans_.scl = MODEL_SCL;
	trans_.quaRotLocal = 
		Quaternion::Euler({ 0.0f, UtilityCommon::Deg2RadF(MODEL_LOCAL_DEG), 0.0f });

	float posX = PLAYER_ONE_POS_X + DISTANCE_POS * playerNum_;
	trans_.pos={ posX,0.0f,0.0f };
	trans_.localPos = { 0.0f,-Player::RADIUS,0.0f };

	//生存状態
	ChangeState(PLAYER_STATE::ALIVE);

	action_->Init();

	//更新
	trans_.Update();
}

void Player::Update(void)
{
	//animationController_->Update();
#ifdef DEBUG_ON
	//CubeMove();
#endif // DEBUG_ON

	//プレイヤー状態更新
	stateUpdate_();

	//回転の同期
	trans_.quaRot = action_->GetPlayerRotY();
	
	trans_.pos = VAdd(trans_.pos, action_->GetMovePow());


	trans_.Update();
}

void Player::Draw(void)
{
	//if (IsDeath())return;
	//モデル描画のZBufferを無効にする
	MV1SetWriteZBuffer(trans_.modelId, false);

	////アウトライン描画
	//renderer_->Draw();

	////モデル描画のZBufferを戻す
	//MV1SetWriteZBuffer(trans_.modelId, true);

	//通常描画
	//MV1DrawModel(trans_.modelId);

	action_->DrawDebug();
	
	////影の描画
	//shadow_->Draw();

#ifdef DEBUG_ON
	DrawDebug();
#endif // DEBUG_ON
}
#ifdef DEBUG_ON
void Player::DrawDebug(void)
{
	unsigned int color = 0xffffff;
	const int HIGH = 10;
	const int WIDTH = 200;
	DrawSphere3D(trans_.pos, RADIUS, 4, 0xff0000, 0xff0000, true);

}

#endif // DEBUG_ON

void Player::ChangeState(PLAYER_STATE _state)
{
	state_ = _state;
	changeStates_[state_]();
}
void Player::ChangeAlive(void)
{
	stateUpdate_ = std::bind(&Player::AliveUpdate, this);
}
void Player::AliveUpdate(void)
{
	//アクション関係更新
	Action();
}
void Player::ChangeDeath(void)
{
	stateUpdate_ = std::bind(&Player::DeathUpdate, this);
}
void Player::DeathUpdate(void)
{

}
void Player::ChangeGoal(void)
{

}
void Player::GoalUpdate(void)
{

}

void Player::Action(void)
{
	//アクション関係の更新
	action_->Update();
	//
	////死んだら何もしないようにする
	//if (IsDeath())
	//{
	//	//何もできないようにする
	//	action_->ChangeAction(PlayerAction::ATK_ACT::NONE);
	//}
}

