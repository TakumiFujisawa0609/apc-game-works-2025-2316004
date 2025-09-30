#include"../Utility/Utility3D.h"
#include"../Utility/UtilityCommon.h"
#include"../Manager/Generic/SceneManager.h"
#include"../../../Manager/Generic/Camera.h"
#include"../Player/ActionController.h"
#include "../../../Object/Common/AnimationController.h"
#include "../Base/CharacterBase.h"
#include"../Base/LogicBase.h"
#include "ActionBase.h"

ActionBase::ActionBase(ActionController& _actCntl):
	scnMng_(SceneManager::GetInstance()),
	actionCntl_(_actCntl),
	anim_(_actCntl.GetAnimation()),
	isTurnable_(true)
{
	speed_ = 0.0f;
	dir_ = {};
	moveDir_ = {};
	movePow_ = {};

	isJump_ = false;
	jumpDeceralation_ = 0.0f;
	jumpPow_ = {};
	stepJump_ = 0.0f;

}

ActionBase::~ActionBase()
{
}


