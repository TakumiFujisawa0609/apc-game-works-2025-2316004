#include<algorithm>
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
#include"../../Common/Geometry/Capsule.h"
#include"../../Common/Geometry/Sphere.h"
#include"../../Common/Geometry/Line.h"
#include"../Object/Card/CardDeck.h"
#include"../Object/Card/CardUI.h"
#include "../../../Object/Common/AnimationController.h"
#include"./ActionController.h"
#include"./PlayerOnHit.h"
#include "./PlayerLogic.h"


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
	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE),ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::IDLE));
	animationController_->Add(static_cast<int>(ANIM_TYPE::RUN), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::RUN));
	animationController_->Add(static_cast<int>(ANIM_TYPE::REACT), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::REACT));
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATTACK_1), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::P_ATTACK_1));
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATTACK_2), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::P_ATTACK_2));
	animationController_->Add(static_cast<int>(ANIM_TYPE::ATTACK_3), ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::P_ATTACK_3));

	//animType_.emplace(
	//	{ ANIM_TYPE::IDLE,static_cast<int>(ANIM_TYPE::IDLE) }
	//	, { ANIM_TYPE::RUN,static_cast<int>(ANIM_TYPE::RUN) }
	//)


	//プレイヤー入力
	logic_ = std::make_unique<PlayerLogic>(padNum_, InputManager::CONTROLL_TYPE::ALL);
	logic_->Init();

	//カードデッキ
	cardCenterPos_ = { 140,140 };//カードの中心位置
	deck_ = std::make_shared<CardDeck>(cardCenterPos_, PLAYER_NUM);

	cardUI_ = std::make_unique<CardUI>();
	cardUI_->Load();
	//デッキに山札追加
	for (int i = 0; i < CARD_NUM_MAX; i++)
	{
		deck_->AddDrawPile(CARD_POWS[i]);
		cardUI_->AddCardUi(CARD_POWS[i]);
	}
	deck_->Load();

	//アクション
	action_ = std::make_unique<ActionController>(*this,*logic_,trans_,*deck_,*animationController_,padNum_);
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

	tag_ = Collider::TAG::PLAYER1;

	//カプセル
	std::unique_ptr<Geometry>geo = std::make_unique<Capsule>(trans_.pos, trans_.quaRot, CAP_LOCAL_TOP, CAP_LOCAL_DOWN, CAP_RADIUS);
	MakeCollider({ tag_ }, std::move(geo));
	//現在の座標と移動後座標を結んだ線のコライダ(落下時の当たり判定)
	geo = std::make_unique<Line>(trans_.pos, trans_.quaRot, Utility3D::VECTOR_ZERO, Utility3D::VECTOR_ZERO);
	MakeCollider({ tag_ }, std::move(geo));

	onHit_ = std::make_unique<PlayerOnHit>(movedPos_, moveDiff_, *action_, colParam_, trans_, tag_);

	//プレイヤー状態
	changeStates_.emplace(PLAYER_STATE::ALIVE, [this]() {ChangeAlive(); });
	changeStates_.emplace(PLAYER_STATE::DEATH, [this]() {ChangeDeath(); });
	changeStates_.emplace(PLAYER_STATE::GOAL, [this]() {ChangeGoal(); });

	//生存状態
	ChangeState(PLAYER_STATE::ALIVE);

	action_->Init();
	deck_->Init();
	cardUI_->Init();

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

	//プレイヤー状態更新
	stateUpdate_();

	cardUI_->Update();

	if (logic_->GetIsAct().isCardMoveLeft)
	{
		cardUI_->CardMoveSelect(CardUI::CARD_SELECT::LEFT);

	}
	else if (logic_->GetIsAct().isCardMoveRight)
	{
		cardUI_->CardMoveSelect(CardUI::CARD_SELECT::RIGHT);
	}



	//回転の同期
	trans_.quaRot = charaRot_.playerRotY_;

	UpdatePost();
	trans_.Update();
}

void Player::Draw(void)
{
	//通常描画
	MV1DrawModel(trans_.modelId);

	action_->DrawDebug();
	
	//カードUI描画
	cardUI_->Draw();

#ifdef DEBUG_ON
	DrawDebug();
#endif // DEBUG_ON
}
void Player::OnHit(const std::weak_ptr<Collider> _hitCol)
{
	onHit_->OnHitUpdate(_hitCol);
}
void Player::MoveDirFronInput(void)
{
	//プレイヤー入力クラスから角度を取得
	VECTOR getDir = logic_->GetDir();
	//カメラの入力角度でプレイヤーの方向を変える
	Quaternion cameraRot = scnMng_.GetCamera().lock()->GetQuaRotOutX();
	charaRot_.dir_ = cameraRot.PosAxis(getDir);
	charaRot_.dir_ = VNorm(charaRot_.dir_);
	charaRot_.dir_ = getDir;

}
void Player::SetGoalRotate(const double _deg)
{
	//カメラの角度を取得
	VECTOR cameraRot = scnMng_.GetCamera().lock()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis(
		static_cast<double >(cameraRot.y) + UtilityCommon::Deg2RadD(_deg), Utility3D::AXIS_Y);

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
#ifdef DEBUG_ON
void Player::DrawDebug(void)
{
	unsigned int color = 0xffffff;
	const int HIGH = 10;
	const int WIDTH = 200;
	//DrawSphere3D(trans_.cardPos, RADIUS_X, 4, 0xff0000, 0xff0000, true);
	for (auto& colParam : colParam_)
	{
		colParam.geometry_->Draw();
	}


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
	logic_->Update();
	action_->Update();

}

