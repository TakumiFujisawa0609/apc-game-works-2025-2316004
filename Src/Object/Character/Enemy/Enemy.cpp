#include"../Utility/Utility3D.h"
#include"../Utility/UtilityCommon.h"
#include"../../../Application.h"
#include"../../Card/CardDeck.h"
#include"../Player/ActionController.h"
#include"../Object/Common/AnimationController.h"
#include"../Enemy/EnemyInput.h"

#include"../Manager/Generic/InputManager.h"

#include "Enemy.h"

Enemy::Enemy(void)
{
}

Enemy::~Enemy(void)
{
}
void Enemy::Load(void)
{
}
	//Transform
void Enemy::Init(void)
{
	//カードデッキ
	cardCenterPos_ = { Application::SCREEN_SIZE_X-140,140 };//カードの中心位置
	deck_ = std::make_shared<CardDeck>(cardCenterPos_,ENEMY_NUM);
	for (int i = 0; i < CARD_NUM_MAX; i++)
	{
		deck_->AddDrawPile(CARD_POWS[i]);
	}
	deck_->Init();
	input_ = std::make_unique<EnemyInput>();
	input_->Init();


	//アニメーション
	animationController_ = std::make_unique<AnimationController>(trans_.modelId);

	action_ = std::make_unique<ActionController>(*input_, trans_, *deck_, *animationController_,InputManager::JOYPAD_NO::PAD1);
	action_->Init();
	//Transformの設定
	trans_.quaRot = Quaternion();
	trans_.scl = MODEL_SCL;
	trans_.quaRotLocal =
		Quaternion::Euler({ 0.0f, UtilityCommon::Deg2RadF(MODEL_LOCAL_DEG), 0.0f });

	trans_.pos = { 20.0f,0.0f,0.0f };
	trans_.localPos = { 0.0f,-RADIUS,0.0f };
}

void Enemy::Update(void)
{
	animationController_->Update();
	input_->Update();
	action_->Update();
}

void Enemy::Draw(void)
{
	DrawSphere3D(trans_.pos, RADIUS, 4, 0xff0000, 0xff0000, true);
	deck_->Draw();
}
