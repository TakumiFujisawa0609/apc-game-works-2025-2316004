#include"../Utility/Utility3D.h"
#include"../Utility/UtilityCommon.h"
#include"../../../Application.h"
#include"../../Card/CardDeck.h"
#include"../Player/ActionController.h"
#include"../Object/Common/AnimationController.h"
#include"../Enemy/EnemyInput.h"
#include"../Manager/Resource/ResourceManager.h"
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
	trans_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY_1));
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal =
		Quaternion::Euler({ 0.0f, UtilityCommon::Deg2RadF(0.0f), 0.0f });

	//アニメーション
	animationController_ = std::make_unique<AnimationController>(trans_.modelId);
	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::IDLE));
	animationController_->Add(static_cast<int>(ANIM_TYPE::RUN), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::RUN));
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATTACK_1), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::P_ATTACK_1));
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATTACK_2), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::P_ATTACK_2));
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATTACK_3), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::P_ATTACK_3));
}

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
	//通常描画
	MV1DrawModel(trans_.modelId);
	deck_->Draw();
}
#ifdef _DEBUG
void Enemy::DrawDebug(void)
{
	DrawSphere3D(trans_.pos, RADIUS, 4, 0xff0000, 0xff0000, true);
}
#endif // _DEBUG