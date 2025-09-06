#include"../Utility/Utility3D.h"
#include"../Utility/UtilityCommon.h"
#include"../../Application.h"
#include"../Card/CardDeck.h"
#include"../Player/Process/EnemyInput.h"


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
	input_->Update();
	if (input_->CheckAct(EnemyInput::ACT_CNTL::CARD_MOVE_LEFT))
	{
		deck_->CardMoveLeft();
	}
	else if(input_->CheckAct(EnemyInput::ACT_CNTL::CARD_MOVE_RIGHT) )
	{
		deck_->CardMoveRight();
	}
	else if (input_->CheckAct(EnemyInput::ACT_CNTL::CARD_USE))
	{
		deck_->MoveHandToCharge();
		
	}
	deck_->CardUseUpdate();
}

void Enemy::Draw(void)
{
	DrawSphere3D(trans_.pos, RADIUS, 4, 0xff0000, 0xff0000, true);
	deck_->Draw();
}
