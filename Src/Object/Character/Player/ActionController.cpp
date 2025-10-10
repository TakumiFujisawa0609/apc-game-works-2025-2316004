#include"../Utility/Utility3D.h"
#include"../Utility/UtilityCommon.h"
#include"Player.h"
#include"./PlayerLogic.h"
//#include "../../Manager/Game/GravityManager.h"
#include "../../../Manager/Generic/Camera.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Resource/ResourceManager.h"

#include"../Object/Card/CardSystem.h"

//#include "../../Object/Common/EffectController.h"
#include "../../../Object/Common/AnimationController.h"

#include"../../Card/CardDeck.h"
#include"../../Card/CardBase.h"

#include"../Base/ActionBase.h"
#include"../Action/Idle.h"
#include"../Action/Run.h"
#include"../Action/Jump.h"
#include"../Action/React.h"
#include"../Action/CardAction.h"

#include "ActionController.h"

ActionController::ActionController(CharacterBase& _charaObj, LogicBase& _input, Transform& _trans, CardDeck& _deck, AnimationController& _anim, InputManager::JOYPAD_NO _padNum) :
	charaObj_(_charaObj)
	, logic_(_input)
	, trans_(_trans)
	, deck_(_deck)
	, anim_(_anim)
	, padNum_(_padNum)
	, scnMng_(SceneManager::GetInstance())
	, act_(ACTION_TYPE::IDLE)
	, cardActTime_(0.0f)
	, isCardAct_(false)
	, stepRotTime_(0.0f)
	, speed_(0.0f)
	, movePow_(Utility3D::VECTOR_ZERO)
	, moveDir_(Utility3D::VECTOR_ZERO)
	,dir_(Utility3D::VECTOR_ZERO)
{
	//エフェクト
	//effect_ = std::make_unique<EffectController>();
}

ActionController::~ActionController(void)
{
	StopResource();
}

void ActionController::Init(void)
{
	//auto cntl = player_.GetCntl();
	auto cntl = InputManager::CONTROLL_TYPE::ALL;

	mainAction_[ACTION_TYPE::IDLE]= std::make_unique<Idle>(*this);
	mainAction_[ACTION_TYPE::MOVE] = std::make_unique<Run>(*this);
	mainAction_[ACTION_TYPE::JUMP] = std::make_unique<Jump>(*this);
	mainAction_[ACTION_TYPE::REACT] = std::make_unique<React>(*this);
	mainAction_[ACTION_TYPE::CARD_ACTION] = std::make_unique<CardAction>(charaObj_,*this,deck_);
	mainAction_[act_]->Init();

	//カードデッキ
	cardCenterPos_ = { 140,140 };//カードの中心位置

	isCardAct_ = false;
	cardActTime_ = 0.0f;
}

void ActionController::Load(void)
{

}

void ActionController::Update(void)
{
	////重力(各アクションに重力を反映させたいので先に重力を先に書く)
	//GravityManager::GetInstance().CalcGravity(dirDown,jumpPow_, GRAVITY_PER);
	mainAction_[act_]->Update();

	MoveDirFronInput();
	charaObj_.Rotate();
	DirAndMovePowUpdate();

	CardChargeUpdate();
	CardMove();
}

void ActionController::DrawDebug(void)
{
	//int dashSeCnt = effect_->GetPlayNum(EffectController::EFF_TYPE::DASH);
	//DrawFormatString(0, 300, 0x000000, "act(%d)\ndashSESize(%d)", (int)logic_.GetAct(), dashSeCnt);
	deck_.Draw();
	
}


void ActionController::ChangeAction(const ACTION_TYPE _act)
{
	if (act_ == _act)return;
	act_ = _act;
	mainAction_[act_]->Init();
 }

void ActionController::CardChargeUpdate(void)
{
	if (logic_.GetIsAct().isCardCharge)
	{
		deck_.CardCharge();
	}
}

void ActionController::CardMove(void)
{
	if (logic_.GetIsAct().isCardMoveLeft)
	{
		deck_.CardMoveLeft();

	}
	else if (logic_.GetIsAct().isCardMoveRight)
	{
		deck_.CardMoveRight();
	}
}




const bool ActionController::GetIsAtkColAlive(void)
{
	return mainAction_[act_]->GetIsAliveAtkCol();
}

const Quaternion ActionController::GetPlayerRotY(void)
{
	return playerRotY_;
}

void ActionController::StopResource(void)
{

}

void ActionController::MoveDirFronInput(void)
{
	charaObj_.MoveDirFronInput();

	float deg = logic_.GetMoveDeg();
	if (!Utility3D::EqualsVZero(movePow_)&&mainAction_[act_]->GetIsTurnable())
	{
		//補完角度の設定(入力角度まで方向転換する)
		charaObj_.SetGoalRotate(deg);
	}
	////補完角度の設定(入力角度まで方向転換する)
	//SetGoalRotate(deg);
}

void ActionController::Rotate(void)
{
	stepRotTime_ -= SceneManager::GetInstance().GetDeltaTime();
	// 回転の球面補間
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

void ActionController::DirAndMovePowUpdate(void)
{
	//方向の更新
	//moveDir_ = charaObj_.GetRotation().dir_;
	moveDir_ = logic_.GetDir();
	float speed = mainAction_[act_]->GetSpeed();
	//移動量の更新
	movePow_ = VScale(moveDir_, speed);
}

void ActionController::SetGoalRotate(const double _deg)
{
	////カメラの角度を取得
	//VECTOR cameraRot = scnMng_.GetCamera().lock()->GetAngles();
	//Quaternion axis = Quaternion::AngleAxis(
	//	/*(double)cameraRot.y + */UtilityCommon::Deg2RadD(_deg), Utility3D::AXIS_Y);

	////現在設定されている回転との角度差を取る
	//double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	//constexpr double ANGLE_THRESHOLD = 0.0;
	//// しきい値
	//if (angleDiff > ANGLE_THRESHOLD)
	//{
	//	stepRotTime_ = TIME_ROT;
	//}
	//
	//goalQuaRot_ = axis;
}
