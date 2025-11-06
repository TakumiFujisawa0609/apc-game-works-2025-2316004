#include"../Utility/Utility3D.h"
#include"../Utility/UtilityCommon.h"
#include"../../../Application.h"
#include"../../Card/CardDeck.h"
#include"../Object/Card/CardUI.h"
#include"../Player/ActionController.h"

#include"../Player/Player.h"

#include"../Player/PlayerOnHit.h"
#include"../Object/Common/AnimationController.h"
#include"../Enemy/EnemyLogic.h"
#include"../../Common/Geometry/Capsule.h"
#include"../../Common/Geometry/Line.h"
#include"../Manager/Resource/ResourceManager.h"
#include"../Manager/Generic/Camera.h"
#include"../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/InputManager.h"

#include"../Base/ActionBase.h"
#include"../Action/Idle.h"
#include"../Action/Run.h"
#include"../Action/Jump.h"
#include"../Action/React.h"
#include"../Action/EnemyCardAction.h"

#include "Enemy.h"

Enemy::Enemy(CharacterBase& _playerChara):
	playerChara_(_playerChara),
	cardCenterPos_({})
{
	//noneHitTag_.emplace({ Collider::TAG::PLAYER1,Collider::TAG::NML_ATK })
}

Enemy::~Enemy(void)
{
	collider_.clear();
}
void Enemy::Load(void)
{
	trans_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::ENEMY_1));
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal =
		Quaternion::Euler({ 0.0f,UtilityCommon::Deg2RadF(MODEL_LOCAL_DEG), 0.0f });

	//敵のカードUI生成
	cardUI_ = std::make_unique<CardUI>();
	//cardUI_->Load();
	//アニメーション
	animationController_ = std::make_unique<AnimationController>(trans_.modelId, SPINE_FRAME_NO);
	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::E_IDLE));
	animationController_->Add(static_cast<int>(ANIM_TYPE::RUN), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::E_RUN));
	animationController_->Add(static_cast<int>(ANIM_TYPE::REACT), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::REACT));
	animationController_->Add(static_cast<int>(ANIM_TYPE::SWIP_ATK), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::E_SWIP_ATK));
	animationController_->Add(static_cast<int>(ANIM_TYPE::JUMP_ATK), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::E_JUMP_ATK));
	animationController_->Add(static_cast<int>(ANIM_TYPE::ROAR_ATK), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::E_ROAR_ATK));

}

void Enemy::Init(void)
{
	//カードデッキ
	cardCenterPos_ = { Application::SCREEN_SIZE_X-140,140 };//カードの中心位置
	deck_ = std::make_shared<CardDeck>(cardCenterPos_,ENEMY_NUM);
	for (int i = 0; i < CARD_NUM_MAX; i++)
	{
		deck_->AddDrawPile(CARD_POWS[i]);
		//cardUI_->AddCardUi(CARD_POWS[i]);
	}
	deck_->Init();
	logic_ = std::make_unique<EnemyLogic>(trans_,playerChara_);
	logic_->Init();

	//各ステータスの設定
	SetStatus(MOVE_SPEED, MAX_HP, MAX_ATK, MAX_DEF);

	action_ = std::make_unique<ActionController>(*this, *logic_, trans_, *deck_, *animationController_,InputManager::JOYPAD_NO::PAD1);

	using ACTION_TYPE = ActionController::ACTION_TYPE;
	action_->AddMainAction<Idle>(ACTION_TYPE::IDLE, *action_);
	action_->AddMainAction<Run>(ACTION_TYPE::MOVE, *action_,status_.speed);
	action_->AddMainAction<Jump>(ACTION_TYPE::JUMP, *action_, *this, jumpPow_);
  	action_->AddMainAction<React>(ACTION_TYPE::REACT, *action_);
	action_->AddMainAction<EnemyCardAction>(ACTION_TYPE::CARD_ACTION, *action_, *this, *deck_);
	action_->Init();
	tag_ = Collider::TAG::ENEMY1;

	//カプセル
	std::unique_ptr<Geometry>geo = std::make_unique<Capsule>(trans_.pos, trans_.quaRot, CAP_LOCAL_TOP, CAP_LOCAL_DOWN, CAP_RADIUS);
	MakeCollider({ tag_ }, std::move(geo));
	//現在の座標と移動後座標を結んだ線のコライダ(落下時の当たり判定)
	geo = std::make_unique<Line>(trans_.pos, trans_.quaRot, Utility3D::VECTOR_ZERO, Utility3D::VECTOR_ZERO);
	MakeCollider({ tag_ }, std::move(geo),{Collider::TAG::NML_ATK});
	//上下ライン
	geo = std::make_unique<Line>(trans_.pos, trans_.quaRot, CAP_LOCAL_TOP, CAP_LOCAL_DOWN);
	MakeCollider({ tag_ }, std::move(geo), { Collider::TAG::NML_ATK });

	onHit_ = std::make_unique<PlayerOnHit>(*this, movedPos_, moveDiff_, *action_, collider_, trans_, tag_);

	//敵のカードUI生成
	//cardUI_->Init();
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

	//static VECTOR dirDown = trans_.GetDown();
	//////重力(各アクションに重力を反映させたいので先に重力を先に書く)
	//GravityManager::GetInstance().CalcGravity(dirDown, jumpPow_, 100.0f);

	logic_->Update();
	action_->Update();
	//cardUI_->Update();
	//回転の同期
	trans_.quaRot = charaRot_.playerRotY_;
	UpdatePost();

	//if (!deck_->IsCardFailure())
	//{
	//	int i = 0;
	//}

	trans_.Update();
}

void Enemy::Draw(void)
{
	//通常描画
	MV1DrawModel(trans_.modelId);
	deck_->Draw();
	//cardUI_->DrawPlayerUI();
	const int BOX_START_X = 200;
	const int BOX_START_Y = 50;
	const int BOX_END_X = BOX_START_X + 400;
	const int BOX_END_Y = BOX_START_Y + 30;

	float hpPer = static_cast<float>(status_.hp) / static_cast<float>(maxStatus_.hp);
	float hpBoxEnd= hpPer * 400.0f;
	int hpBox_x = (BOX_START_X - 1) + static_cast<int>(hpBoxEnd);
	DrawBox(BOX_START_X, BOX_START_Y, BOX_END_X, BOX_END_Y, 0x000000, -1);
	DrawBox(BOX_START_X-1, BOX_START_Y-1, hpBox_x, BOX_END_Y, 0x0000ff, -1);
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
	charaRot_.dir_ = getDir;
}
void Enemy::SetGoalRotate(const double _deg)
{
	Quaternion axis= Quaternion::Euler
	(Utility3D::GetRotAxisToTarget(trans_.pos, playerChara_.GetTransform().pos, Utility3D::AXIS_Y));

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
	for (auto& col : collider_)
	{
		col->GetGeometry().Draw();
	}

	DrawFormatString(600, 300, 0x000000, L"action(%d)", static_cast<int>(action_->GetAct()));

	logic_->DebugDraw();
}
#endif // _DEBUG