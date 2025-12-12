#include<algorithm>
#include "../../../Utility/Utility3D.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Utility/Utility2D.h"

#include "../../../Manager/Resource/ResourceManager.h"
//#include "../../Manager/System/SoundManager.h"
#include "../../../Manager/Generic/SceneManager.h"
//#include"../../Manager/System/DateBank.h"

#include "../../../Manager/Generic/Camera.h"
#include "../Manager/Game/GravityManager.h"
//#include "../../Renderer/ModelMaterial.h"
//#include "../../Renderer/ModelRenderer.h"

#include "../Manager/Game/CharacterManager.h"
//#include"../../Object/Common/EffectController.h"
#include"../../Common/Geometry/Capsule.h"
#include"../../Common/Geometry/Sphere.h"
#include"../../Common/Geometry/Line.h"
#include"../Object/Card/CardDeck.h"
#include"../Object/Card/PlayerCardUI.h"
#include "../../../Object/Common/AnimationController.h"
#include"./ActionController.h"
#include"../Base/CharacterOnHitBase.h"
#include"./PlayerOnHit.h"

#include"../Base/ActionBase.h"
#include"../Action/Idle.h"
#include"../Action/Run.h"
#include"../Action/Jump.h"
#include"../Action/React.h"
#include"../Action/Dodge.h"
#include"../Action/PlayerCardAction.h"
#include "./PlayerLogic.h"


#include "Player.h"


namespace
{
	
}
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
	changeStates_.clear();
	collider_.clear();
}

void Player::Load(void)
{
	trans_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal =
		Quaternion::Euler({ 0.0f, UtilityCommon::Deg2RadF(0.0f), 0.0f });

	animationController_ = std::make_unique<AnimationController>(trans_.modelId, SPINE_FRAME_NO);
	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE),ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::P_IDLE));
	animationController_->Add(static_cast<int>(ANIM_TYPE::RUN), ANIM_SPEED*2.0f, resMng_.LoadModelDuplicate(ResourceManager::SRC::P_RUN));
	animationController_->Add(static_cast<int>(ANIM_TYPE::REACT), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::REACT));
	animationController_->Add(static_cast<int>(ANIM_TYPE::JUMP), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::P_JUMP));
	animationController_->Add(static_cast<int>(ANIM_TYPE::DODGE), DODGE_ANIM_SPD, resMng_.LoadModelDuplicate(ResourceManager::SRC::P_DODGE));
	animationController_->Add(static_cast<int>(ANIM_TYPE::CARD_RELOAD), DODGE_ANIM_SPD, resMng_.LoadModelDuplicate(ResourceManager::SRC::P_RELOAD));
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATTACK_1), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::P_ATTACK_1));
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATTACK_2), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::P_ATTACK_2));
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATTACK_3), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::P_ATTACK_3));

	//animType_.emplace(
	//	{ ANIM_TYPE::P_IDLE,static_cast<int>(ANIM_TYPE::P_IDLE) }
	//	, { ANIM_TYPE::P_RUN,static_cast<int>(ANIM_TYPE::P_RUN) }
	//)


	//プレイヤー入力
	logic_ = std::make_unique<PlayerLogic>(padNum_, InputManager::CONTROLL_TYPE::ALL);
	logic_->Init();

	//カードデッキ
	cardCenterPos_ = { 140,140 };//カードの中心位置
	deck_ = std::make_shared<CardDeck>(cardCenterPos_, PLAYER_NUM);

	cardUI_ = std::make_unique<PlayerCardUI>();
	cardUI_->Load();
	
	deck_->Load();

	//アクション
	action_ = std::make_unique<ActionController>(*this,*logic_,trans_,*deck_,*animationController_,padNum_);
	action_->Load();

	//各ステータスの設定
	SetStatus(MOVE_SPEED, MAX_HP, MAX_ATK, MAX_DEF);

}

void Player::Init(void)
{
	//Transformの設定
	trans_.quaRot = Quaternion();
	trans_.scl = MODEL_SCL;
	trans_.quaRotLocal = 
		Quaternion::Euler({ 0.0f, UtilityCommon::Deg2RadF(MODEL_LOCAL_DEG), 0.0f });

	float posX = PLAYER_ONE_POS_X + DISTANCE_POS * playerNum_;
	trans_.pos={ 0.0f,0.0f,-500.0f };
	trans_.localPos = { 0.0f,Player::RADIUS,-0.0f };

	tag_ = Collider::TAG::PLAYER1;

	capRadius_ = RADIUS;

	MakeColliderGeometry();

	onHit_ = std::make_unique<PlayerOnHit>(*this,movedPos_, moveDiff_, *action_, collider_, trans_);

	//プレイヤー状態
	changeStates_.emplace(PLAYER_STATE::ALIVE, [this]() {ChangeAlive(); });
	changeStates_.emplace(PLAYER_STATE::DEATH, [this]() {ChangeDeath(); });
	changeStates_.emplace(PLAYER_STATE::GOAL, [this]() {ChangeGoal(); });

	using ACTION_TYPE = ActionController::ACTION_TYPE;
	action_->AddMainAction<Idle>(ACTION_TYPE::IDLE, *action_);
	action_->AddMainAction<Run>(ACTION_TYPE::MOVE, *action_,status_.speed);
	action_->AddMainAction<Jump>(ACTION_TYPE::JUMP, *action_,*this,jumpPow_);
	action_->AddMainAction<Dodge>(ACTION_TYPE::DODGE, *action_,trans_ ,status_.speed);
	action_->AddMainAction<React>(ACTION_TYPE::REACT, *action_);
	action_->AddMainAction<PlayerCardAction>(ACTION_TYPE::CARD_ACTION,*action_, *this, *deck_);

	//atkTable_.emplace(ATK_TYPE::NML_ATK_1,)

	//生存状態
	ChangeState(PLAYER_STATE::ALIVE);

	action_->Init();
	deck_->Init();
	
	cardUI_->MakeObject();
	//デッキに山札追加
	for (int i = 0; i < CARD_NUM_MAX; i++)
	{
		deck_->AddDrawPile(CARD_POWS[i]);
		cardUI_->AddCardUi(CARD_POWS[i]);
	}
	//デッキの先頭にリロードカード追加
	deck_->AddDrawPile(RELOAD_CARD_STATUS);
	cardUI_->AddCardUi(RELOAD_CARD_STATUS);
	cardUI_->Init();
	//cardUI_->InitCardUI();

	//更新
	trans_.Update();
}

void Player::Update(void)
{
	//animationController_->Update();
#ifdef DEBUG_ON
	//CubeMove();
#endif // DEBUG_ON

	animationController_->Update();

	////プレイヤーの下を設定
	////static VECTOR dirDown = charaObj_.GetTransform().GetDown();
	//static VECTOR dirDown = trans_.GetDown();
	////重力(各アクションに重力を反映させたいので先に重力を先に書く)
	//VECTOR jumpPow = action_->GetJumpPow();
	//GravityManager::GetInstance().CalcGravity(dirDown, jumpPow, 90.0f);

	//プレイヤー状態更新
	stateUpdate_();

	cardUI_->Update();

	//回転の同期
	trans_.quaRot = charaRot_.playerRotY_;
	trans_.Update();
}

void Player::Draw(void)
{
	//通常描画
	MV1DrawModel(trans_.modelId);

	//action_->DrawDebug();
	
	//カードUI描画
	cardUI_->Draw();

	float hpPer = static_cast<float>(status_.hp) / static_cast<float>(maxStatus_.hp);

	Utility2D::DrawBarGraph(START_HPBAR_POS, HPBAR_SIZE, hpPer, 0x000000, 0x00ff00);

#ifdef _DEBUG
	DrawDebug();
#endif // _DEBUG
}
void Player::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	onHit_->OnHitUpdate(_hitCol);
}
void Player::MoveDirFromInput(void)
{
	//プレイヤー入力クラスから角度を取得
	VECTOR getDir = logic_->GetDir();
	//カメラの入力角度でプレイヤーの方向を変える
	Quaternion cameraRot = scnMng_.GetCamera().lock()->GetQuaRotOutX();
	charaRot_.dir_ = cameraRot.PosAxis(getDir);
	charaRot_.dir_ = VNorm(charaRot_.dir_);
	//charaRot_.dir_ = getDir;

}
void Player::SetGoalRotate(const double _deg)
{
	//カメラの角度を取得
	VECTOR cameraRot = scnMng_.GetCamera().lock()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis(
		static_cast<double>(cameraRot.y) + UtilityCommon::Deg2RadD(_deg), Utility3D::AXIS_Y);

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
void Player::DrawDebug(void)
{
	unsigned int color = 0xffffff;
	const int HIGH = 10;
	const int WIDTH = 200;
	//DrawSphere3D(trans_.cardPos, RADIUS_X, 4, 0xff0000, 0xff0000, true);
	for (auto& col : collider_)
	{
		//colParam.geometry_->Draw();
		col.second->GetGeometry().Draw();
	}
	
	VECTOR pos = trans_.pos;
	//DrawFormatString(0, 300, 0x000000, L"action(%d)\n\nisDamage(%d)", static_cast<int>(action_->GetAct()),isDamage_);
	DrawFormatString(0, 300, 0x000000, L"pos(%f,%f,%f)", pos.x, pos.y,pos.z);
	//// フレームの取得
	//int frmNo = MV1SearchFrame(trans_.modelId, L"Maria_sword");
	//if (frmNo == -1) {
	//	// エラー処理またはログ出力
	//	return;
	//}



	//// 手の位置とグローバルマトリクスを取得
	//const auto& posFream = MV1GetFramePosition(trans_.modelId, frmNo);
	VECTOR atkPos = Utility3D::AddPosRotate(trans_.pos, trans_.quaRot, { 0.0f,100.0f,40.0f });
	DrawSphere3D(atkPos, 10, 10, 0xffffff, 0xffffff, false);

}

#endif // _DEBUG

void Player::MakeColliderGeometry(void)
{
	//カプセル
	std::unique_ptr<Geometry>geo = std::make_unique<Capsule>(trans_.pos, trans_.quaRot, CAP_LOCAL_TOP, CAP_LOCAL_DOWN, RADIUS);
	MakeCollider(TAG_PRIORITY::BODY, { tag_ }, std::move(geo));
	tagPrioritys_.emplace_back(TAG_PRIORITY::BODY);

	//現在の座標と移動後座標を結んだ線のコライダ(落下時の当たり判定)
	geo = std::make_unique<Line>(trans_.pos, trans_.quaRot, Utility3D::VECTOR_ZERO, Utility3D::VECTOR_ZERO);
	MakeCollider(TAG_PRIORITY::MOVE_LINE, { tag_ }, std::move(geo));
	tagPrioritys_.emplace_back(TAG_PRIORITY::MOVE_LINE);

	//上下ライン
	geo = std::make_unique<Line>(trans_.pos, trans_.quaRot, CAP_LOCAL_TOP, CAP_LOCAL_DOWN);
	MakeCollider(TAG_PRIORITY::UPDOWN_LINE, { tag_ }, std::move(geo));
	tagPrioritys_.emplace_back(TAG_PRIORITY::UPDOWN_LINE);
}

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
	if (!onHit_->GetHitPoint().isOverHead)
	{
		static VECTOR dirDown = trans_.GetDown();
		//重力(各アクションに重力を反映させたいので先に重力を先に書く)
		//GravityManager::GetInstance().CalcGravity(dirDown, jumpPow_, 100.0f);
	}



	//アクション関係の更新
	logic_->Update();

	action_->Update();

	UpdatePost();


}

