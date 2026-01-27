#include "../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/Camera.h"
#include "../Object/Character/Base/CharacterBase.h"
#include "../Object/Character/Player/Player.h"
#include "../Object/Character/Enemy/Enemy.h"

#include "CharacterManager.h"

void CharacterManager::Load(void)
{
	player_ = std::make_shared<Player>();
	player_->Load();
	enemy_ = std::make_shared<Enemy>();
	enemy_->Load();


	SceneManager::GetInstance().GetCamera().lock()->SetTarget(&enemy_->GetTransform());
}

void CharacterManager::Init(void)
{
	//プレイヤーのローカル中心座標をカプセルの中央に合わせる
	const float playerCenterLocalPosY = (Player::CAP_LOCAL_TOP.y - Player::CAP_LOCAL_DOWN.y) * 0.5f;
	SceneManager::GetInstance().GetCamera().lock()->SetFollow(&player_->GetTransform(),{0.0f,playerCenterLocalPosY ,0.0f});
	SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::START_DIRECTION);

	player_->SetLogicTargetCharacter(enemy_);
	enemy_->SetLogicTargetCharacter(player_);
	player_->Init();
	enemy_->Init();
}

void CharacterManager::Update(void)
{
	//プレイヤーの更新
	player_->Update();
	//敵の更新
	enemy_->Update();
}

void CharacterManager::DirectionUpdate(void)
{
	//if()

	//enemy_->UpdateDirection();
	//player_->UpdateDirection();
}

void CharacterManager::Draw(void)
{
	enemy_->Draw();
	player_->Draw();
}

void CharacterManager::Draw2D(void)
{
	enemy_->Draw2D();
	player_->Draw2D();
}

void CharacterManager::Release(void)
{
}
const bool CharacterManager::IsSceneChageClearCondition(void) const
{
	return enemy_->GetStatus().hp <= 0;
}
const bool CharacterManager::IsSceneChangeGameOverCondition(void) const
{
	return player_->GetStatus().hp <= 0;
}
const bool CharacterManager::GetIsEnemyRoar(void) const
{
	return enemy_->GetIsRoar();
}
void CharacterManager::ChangeCharacterNormalUpdate(void)
{
	player_->ChangeDirectToNormal();
	enemy_->ChangeDirectToNormal();
}
void CharacterManager::ChangeCharacterDirectionUpdate(void)
{
	player_->ChangeUpdatePhase(CharacterBase::UPDATE_PHASE::DIRECTION);
	enemy_->ChangeUpdatePhase(CharacterBase::UPDATE_PHASE::DIRECTION);
}
CharacterManager::CharacterManager(void)
{

}
CharacterManager::~CharacterManager(void)
{
}
