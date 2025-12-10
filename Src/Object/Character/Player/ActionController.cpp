#include"../Utility/Utility3D.h"
#include"../Utility/UtilityCommon.h"
#include"Player.h"
#include"./PlayerLogic.h"
#include "../../../Manager/Generic/Camera.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Resource/ResourceManager.h"

#include"../Object/Card/CardSystem.h"

//#include "../../Object/Common/EffectController.h"
#include "../../../Object/Common/AnimationController.h"

#include"../../Card/CardDeck.h"
#include"../../Card/CardBase.h"
#include"../../Card/CardUIBase.h"

#include"../Base/ActionBase.h"
#include"../Action/Idle.h"
#include"../Action/Run.h"
#include"../Action/Jump.h"
#include"../Action/React.h"
#include"../Action/PlayerCardAction.h"

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
	,cardCenterPos_({0,0})
{
	//エフェクト
	//effect_ = std::make_unique<EffectController>();
}

ActionController::~ActionController(void)
{
	StopResource();
	mainAction_.clear();
	subAction_.clear();
	changeAction_.clear();
}

void ActionController::Init(void)
{
	//auto cntl = player_.GetCntl();
	auto cntl = InputManager::CONTROLL_TYPE::ALL;

	//初期化の前に追加したアクションの初期化
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
	mainAction_[act_]->Update();

	MoveDirFromInput();
	charaObj_.Rotate();
	DirAndMovePowUpdate();


	CardChargeUpdate();
	CardMove();
}


const ActionBase& ActionController::GetMainAction(void)
{
	return *mainAction_.at(act_);
}

#ifdef _DEBUG
void ActionController::DrawDebug(void)
{
	//int dashSeCnt = effect_->GetPlayNum(EffectController::EFF_TYPE::DASH);
	//DrawFormatString(0, 300, 0x000000, "act(%d)\ndashSESize(%d)", (int)logic_.GetAct(), dashSeCnt);
	//deck_.Draw();
	DrawFormatString(0, 320, 0x000000, L"pos(%f,%f,%f)", trans_.pos.x, trans_.pos.y, trans_.pos.z);

}
#endif // _DEBUG



const float& ActionController::GetSpd(void) const
{
	return mainAction_.at(act_)->GetSpeed();
}


void ActionController::ChangeAction(const ACTION_TYPE _act)
{
	if (act_ == _act)return;
	ACTION_TYPE revType = act_;
	mainAction_[revType]->Release();
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
	CardUIBase& cardUI = charaObj_.GetCardUI();
	if (cardUI.GetSelectState() == CardUIBase::CARD_SELECT::DISITION
		||cardUI.GetSelectState()== CardUIBase::CARD_SELECT::LEFT
		||cardUI.GetSelectState()== CardUIBase::CARD_SELECT::RIGHT)return;
	if (IsCardLeftMoveable())
	{
		deck_.CardMoveLeft();
		cardUI.ChangeSelectState(CardUIBase::CARD_SELECT::LEFT);

	}
	else if (IsCardRightMoveable())
	{
		deck_.CardMoveRight();
		cardUI.ChangeSelectState(CardUIBase::CARD_SELECT::RIGHT);
	}
}

const bool& ActionController::GetIsAtkColAlive(void)
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

const bool ActionController::IsCardDisitionControll(void)
{
	CardUIBase& cardUI = charaObj_.GetCardUI();
	const CardUIBase::CARD_SELECT selectState = cardUI.GetSelectState();
	//return selectState !=CardUI::CARD_SELECT::LEFT&& selectState != CardUI::CARD_SELECT::RIGHT;
	return selectState == CardUIBase::CARD_SELECT::NONE;
}

void ActionController::SetFlinchCnt(const float _flinchTime)
{
	mainAction_[ACTION_TYPE::REACT]->SetFlinchCnt(_flinchTime);
}

void ActionController::MoveDirFromInput(void)
{
	charaObj_.MoveDirFromInput();

	float deg = logic_.GetMoveDeg();
	if (/*!Utility3D::EqualsVZero(movePow_)&&*/mainAction_[act_]->GetIsTurnable())
	{
		//補完角度の設定(入力角度まで方向転換する)
		charaObj_.SetGoalRotate(deg);
	}
	////補完角度の設定(入力角度まで方向転換する)
	//SetGoalRotate(deg);
}

void ActionController::DirAndMovePowUpdate(void)
{
	//方向の更新
	moveDir_ = charaObj_.GetRotation().dir_;
	float speed = mainAction_[act_]->GetSpeed();
	//移動量の更新
	movePow_ = VScale(moveDir_, speed);

	//jumpPow_ = VScale(charaObj_.GetTransform().GetUp(), mainAction_[act_]->GetJumpPow().y);
}
const bool ActionController::IsCardLeftMoveable(void)
{
	//カードアクション種類
	const ActionBase::CARD_ACT_TYPE cardAct = mainAction_.at(act_)->GetCardAction();

	//カードUIの選択状態
	const CardUIBase::CARD_SELECT selectState = charaObj_.GetCardUI().GetSelectState();

	return logic_.GetIsAct().isCardMoveLeft 
		&& cardAct != ActionBase::CARD_ACT_TYPE::RELOAD
		&& selectState !=CardUIBase::CARD_SELECT::RELOAD;
}
const bool ActionController::IsCardRightMoveable(void)
{
	//カードアクション種類
	const ActionBase::CARD_ACT_TYPE cardAct = mainAction_.at(act_)->GetCardAction();

	//カードUIの選択状態
	CardUIBase::CARD_SELECT selectState = charaObj_.GetCardUI().GetSelectState();

	return logic_.GetIsAct().isCardMoveRight 
		&& cardAct != ActionBase::CARD_ACT_TYPE::RELOAD
		&& selectState != CardUIBase::CARD_SELECT::RELOAD;
}
const ActionBase::CARD_ACT_TYPE& ActionController::GetCardAction(void)const
{
	return mainAction_.at(act_)->GetCardAction();
}

