#include "../../../Utility/Utility3D.h"
#include "../../../Utility/UtilityCommon.h"
#include "../Application.h"
//#include "../../Manager/Game/GravityManager.h"
#include "../../../Manager/Game/PlayerManager.h"
#include "../../../Manager/Resource/ResourceManager.h"
//#include "../../Manager/System/SoundManager.h"
#include "../../../Manager/Generic/SceneManager.h"
//#include"../../Manager/System/DateBank.h"

#include "../../../Manager/Generic/Camera.h"

//#include "../../Renderer/ModelMaterial.h"
//#include "../../Renderer/ModelRenderer.h"

//#include "../../Object/Common/Geometry/Sphere.h"
//#include "../../Object/Common/Geometry/Line.h"
//#include"../../Object/Common/Geometry/Model.h"
//#include"../../Object/Common/EffectController.h"
#include"../Object/Card/CardDeck.h"
#include "../../../Object/Common/AnimationController.h"
#include"./ActionController.h"
//#include"./PlayerOnHit.h"
#include "./InputController.h"
#include<algorithm>


#include "Player.h"
Player::Player(void)
	:playerNum_()
	, cntl_()
	, padNum_()
	, state_()
	, stateUpdate_()
{
	trans_ = Transform();
	playerNum_ = 0;
	cntl_ = InputManager::CONTROLL_TYPE::ALL;
	//初めのJOYPADがkey_padなのでパッドの番号に合わせる
	//パッド番号を設定
	padNum_ = static_cast<InputManager::JOYPAD_NO>(playerNum_ + 1);


}

Player::~Player(void)
{
}

void Player::Load(void)
{
	trans_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal =
		Quaternion::Euler({ 0.0f, UtilityCommon::Deg2RadF(0.0f), 0.0f });

	animationController_ = std::make_unique<AnimationController>(trans_.modelId);

	//プレイヤー入力
	input_ = std::make_unique<InputController>(padNum_, InputManager::CONTROLL_TYPE::ALL);
	input_->Init();

	//カードデッキ
	cardCenterPos_ = { 140,140 };//カードの中心位置
	deck_ = std::make_shared<CardDeck>(cardCenterPos_, PLAYER_NUM);
	//デッキに山札追加
	for (int i = 0; i < CARD_NUM_MAX; i++)
	{
		deck_->AddDrawPile(CARD_POWS[i]);
	}
	deck_->Init();

	//アクション
	action_ = std::make_unique<ActionController>(*input_,trans_,*deck_,padNum_);
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

	//プレイヤー状態
	changeStates_.emplace(PLAYER_STATE::ALIVE, [this]() {ChangeAlive(); });
	changeStates_.emplace(PLAYER_STATE::DEATH, [this]() {ChangeDeath(); });
	changeStates_.emplace(PLAYER_STATE::GOAL, [this]() {ChangeGoal(); });

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
	// 両面描画を有効にする
	SetUseBackCulling(FALSE);

	//通常描画
	MV1DrawModel(trans_.modelId);

	// 元に戻す
	SetUseBackCulling(TRUE);

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
	input_->Update();
	action_->Update();

}

