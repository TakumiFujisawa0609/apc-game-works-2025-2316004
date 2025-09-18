#include"../Utility/Utility3D.h"
#include"../Utility/UtilityCommon.h"
#include"Player.h"
#include"./InputController.h"
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
#include"../Action/CardAction.h"

#include "ActionController.h"

ActionController::ActionController(InputBase& _input, Transform& _trans, CardDeck& _deck, InputManager::JOYPAD_NO _padNum) :
	input_(_input)
	, trans_(_trans)
	, deck_(_deck)
	, padNum_(_padNum)
	, scnMng_(SceneManager::GetInstance())
	, act_(ACTION_TYPE::IDLE)
	, cardActTime_(0.0f)
	,isCardAct_(false)
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
	mainAction_[ACTION_TYPE::CARD_ACTION] = std::make_unique<CardAction>(*this,deck_);

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

	CardChargeUpdate();
	CardMove();
}

void ActionController::DrawDebug(void)
{
	//int dashSeCnt = effect_->GetPlayNum(EffectController::EFF_TYPE::DASH);
	//DrawFormatString(0, 300, 0x000000, "act(%d)\ndashSESize(%d)", (int)input_.GetAct(), dashSeCnt);
	deck_.Draw();
	
}


void ActionController::ChangeAction(const ACTION_TYPE _act)
{
	if (act_ == _act)return;
	act_ = _act;
	mainAction_[act_]->Init();
 }

const VECTOR ActionController::GetMovePow(void)
{
	return mainAction_[act_]->GetMovePow();
}

void ActionController::CardChargeUpdate(void)
{
	if (input_.GetIsAct().isCardCharge)
	{
		deck_.CardCharge();
	}
	
}

void ActionController::CardMove(void)
{
	if (input_.GetIsAct().isCardMoveLeft)
	{
		deck_.CardMoveLeft();
	}
	else if (input_.GetIsAct().isCardMoveRight)
	{
		deck_.CardMoveRight();
	}
}


void ActionController::StopResource(void)
{

}
