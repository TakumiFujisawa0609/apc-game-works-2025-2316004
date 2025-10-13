#include"../Utility/Utility3D.h"
#include"../Utility/UtilityCommon.h"
#include"../../../Application.h"
#include"../../Card/CardDeck.h"
#include"../Player/ActionController.h"

#include"../Player/Player.h"


#include"../Player/PlayerOnHit.h"
#include"../Object/Common/AnimationController.h"
#include"../Enemy/EnemyLogic.h"
#include"../../Common/Geometry/Capsule.h"
#include"../../Common/Geometry/Line.h"
#include"../Manager/Resource/ResourceManager.h"
#include"../Manager/Generic/InputManager.h"

#include "Enemy.h"

Enemy::Enemy(CharacterBase& _playerChara):
	playerChara_(_playerChara)
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
		Quaternion::Euler({ 0.0f,UtilityCommon::Deg2RadF(MODEL_LOCAL_DEG), 0.0f });

	//アニメーション
	animationController_ = std::make_unique<AnimationController>(trans_.modelId);
	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::IDLE));
	animationController_->Add(static_cast<int>(ANIM_TYPE::RUN), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::RUN));
	animationController_->Add(static_cast<int>(ANIM_TYPE::REACT), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::REACT));
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
	logic_ = std::make_unique<EnemyLogic>(playerChara_,trans_);
	logic_->Init();

	action_ = std::make_unique<ActionController>(*this, *logic_, trans_, *deck_, *animationController_,InputManager::JOYPAD_NO::PAD1);
	action_->Init();
	tag_ = Collider::TAG::ENEMY1;

	std::unique_ptr<Geometry>geo = std::make_unique<Capsule>(trans_.pos, trans_.quaRot, CAP_LOCAL_TOP, CAP_LOCAL_DOWN, CAP_RADIUS);
	MakeCollider({ tag_ }, std::move(geo));

	//現在の座標と移動後座標を結んだ線のコライダ(落下時の当たり判定)
	geo = std::make_unique<Line>(trans_.pos, trans_.quaRot, Utility3D::VECTOR_ZERO, Utility3D::VECTOR_ZERO);
	MakeCollider({ tag_ }, std::move(geo));

	onHit_ = std::make_unique<PlayerOnHit>(*this, movedPos_, moveDiff_, *action_, colParam_, trans_, tag_);

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
	logic_->Update();
	action_->Update();
	//回転の同期
	trans_.quaRot = charaRot_.playerRotY_;
	UpdatePost();
	trans_.Update();
}

void Enemy::Draw(void)
{
	//通常描画
	MV1DrawModel(trans_.modelId);
	deck_->Draw();

#ifdef _DEBUG
	DrawDebug();
#endif // _DEBUG

}
void Enemy::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	onHit_->OnHitUpdate(_hitCol);
}
void Enemy::MoveDirFronInput(void)
{
	//プレイヤー入力クラスから角度を取得
	VECTOR getDir = logic_->GetDir();
	Quaternion playerRot = playerChara_.GetTransform().quaRot;
	charaRot_.dir_ = playerRot.PosAxis(getDir);
	charaRot_.dir_ = VNorm(charaRot_.dir_);
	//charaRot_.dir_ = getDir;
}
void Enemy::SetGoalRotate(const double _deg)
{
	//Quaternion axis = Quaternion::AngleAxis(
	//	UtilityCommon::Deg2RadD(_deg), Utility3D::AXIS_Y);

	Quaternion axis= Quaternion::Euler(Utility3D::GetRotAxisToTarget(trans_.pos, playerChara_.GetTransform().pos, Utility3D::AXIS_Y));

	//現在設定されている回転との角度差を取る
	double angleDiff = Quaternion::Angle(axis, charaRot_.goalQuaRot_);

	constexpr double ANGLE_THRESHOLD = 0.1;
	// しきい値
	if (angleDiff > ANGLE_THRESHOLD)
	{
		charaRot_.stepRotTime_ = TIME_ROT;
	}
	charaRot_.goalQuaRot_ = axis;
}
#ifdef _DEBUG
void Enemy::DrawDebug(void)
{
	//DrawSphere3D(trans_.pos, RADIUS, 4, 0xff0000, 0xff0000, true);
	VECTOR euler = trans_.quaRot.ToEuler();
	DrawFormatString(100, 100, 0xffffff, L"(%f,%f,%f)", euler.x, logic_->GetMoveDeg(), euler.z);
	for (auto& colParam : colParam_)
	{
		colParam.geometry_->Draw();
	}
}
#endif // _DEBUG