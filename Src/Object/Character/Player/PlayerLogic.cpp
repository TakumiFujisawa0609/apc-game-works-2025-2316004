#include"../Object/Character/Player/Player.h"
#include"../Utility/Utility3D.h"
#include "../Object/Character/Base/CharacterBase.h"

#include "../../../Manager/Generic/Camera.h"
#include "PlayerLogic.h"

PlayerLogic::PlayerLogic(Transform& _myTrans,bool& _isCanMoveAble , InputManager::JOYPAD_NO _padNum, InputManager::CONTROLL_TYPE _cntl):
	LogicBase(_myTrans),
	isCanMoveable_(_isCanMoveAble),
	padNum_(_padNum),
	cntl_(_cntl),
	camera_(SceneManager::GetInstance().GetCamera()),
	input_(InputManager::GetInstance()),
	inputS_(InputManagerS::GetInstance())
{
	actCntl_ = ACT_CNTL::NONE;
	leftStickX_ = -1;
	leftStickY_ = -1;
	stickDeg_ = -1;
	moveDeg_ = 0.0f;
	moveDir_ = Utility3D::VECTOR_ZERO;

	inputUpdates_[InputManager::CONTROLL_TYPE::ALL] = [this]() {InputAll(); };
	inputUpdates_[InputManager::CONTROLL_TYPE::CONTROLLER] = [this]() {InputPad(); };
}

PlayerLogic::~PlayerLogic(void)
{
}

void PlayerLogic::Init(void)
{
	actCntl_ = ACT_CNTL::NONE;
	leftStickX_ = -1;
	leftStickY_ = -1;
	stickDeg_ = -1;
	moveDeg_ = 0.0f;
	moveDir_ = Utility3D::VECTOR_ZERO;
}

void PlayerLogic::Update(void)
{
	inputUpdates_[cntl_]();
}

void PlayerLogic::InputKeyBoard(void)
{
	using ATK_ACT = Player::ATK_ACT;
	//actCntl_ = ACT_CNTL::NONE;

#ifdef _DEBUG
	//if (input_.IsTrgDown(InputManager::CONTROL_TYPE::DEBUG_CHANGE_INPUT, InputManager::JOYPAD_NO::PAD1, InputManager::TYPE::PAD))
	//{
	//	ChangeInput(InputManager::TYPE::PAD);
	//}
#endif // _DEBUG
	
	//移動角度を決める
	if (input_.IsNew(MOVE_FRONT_KEY))
	{ 
		moveDeg_ = FLONT_DEG;
		moveDir_ = Utility3D::DIR_F;

	}
	else if (input_.IsNew(MOVE_LEFT_KEY))
	{ 
		moveDeg_ = LEFT_DEG; 
		moveDir_ = Utility3D::DIR_L;
	} 
	else if (input_.IsNew(MOVE_BACK_KEY))
	{ 
		moveDeg_ = BACK_DEG; 
		moveDir_ = Utility3D::DIR_B;
	}
	else if (input_.IsNew(MOVE_RIGHT_KEY))
	{
		moveDeg_ = RIGHT_DEG; 
		moveDir_ = Utility3D::DIR_R;
	}

	//カード使用
	if (input_.IsTrgDown(CARD_CHARGE_KEY)) { actCntl_ = ACT_CNTL::CARD_CHARGE; }

	//ジャンプキー
	if (input_.IsTrgDown(CARD_USE_KEY)) { actCntl_ = ACT_CNTL::CARD_USE; }
}

void PlayerLogic::InputAll(void)
{
	using ATK_ACT = Player::ATK_ACT;
	isAct_ = {};
	float deg = 0.0f;
	VECTOR dir = {};
	//moveDir_ = Utility3D::VECTOR_ZERO;
	//移動角度を決める
	if (isCanMoveable_)
	{
		if (input_.IsNew(MOVE_FRONT_KEY))
		{
			isAct_.isRun = true;
			deg = FLONT_DEG;
			dir = Utility3D::DIR_F;
			prevMoveDir_ = moveDir_;
		}
		else if (input_.IsNew(MOVE_LEFT_KEY))
		{
			isAct_.isRun = true;
			deg = LEFT_DEG;
			dir = Utility3D::DIR_L;
			prevMoveDir_ = moveDir_;
		}
		else if (input_.IsNew(MOVE_BACK_KEY))
		{
			isAct_.isRun = true;
			deg = BACK_DEG;
			dir = Utility3D::DIR_B;
			prevMoveDir_ = moveDir_;
		}
		else if (input_.IsNew(MOVE_RIGHT_KEY))
		{
			isAct_.isRun = true;
			deg = RIGHT_DEG;
			dir = Utility3D::DIR_R;
			prevMoveDir_ = moveDir_;
		}

		//スティックが傾いたら
		if (inputS_.IsPressed(INPUT_EVENT::UP) || inputS_.IsPressed(INPUT_EVENT::DOWN)
			|| inputS_.IsPressed(INPUT_EVENT::RIGHT) || inputS_.IsPressed(INPUT_EVENT::LEFT))
		{
			//スティックサイズの取得
			LStickAngleSize_ = inputS_.GetKnockLStickSize(padNum_);

			//スティックの角度を求める
			stickDeg_ = inputS_.GetLStickDeg(padNum_);

			//スティックの角度によって移動方向を決める
			deg = stickDeg_;

			//ベクトルの計算
			VECTOR stickDir = { static_cast<float>(LStickAngleSize_.x) ,0.0f,static_cast<float>(-LStickAngleSize_.y) };
			dir = VNorm(stickDir);
			prevMoveDir_ = moveDir_;
			isAct_.isRun = true;
		}
	}


	//移動操作の時はカメラの角度を参照する
	if (isAct_.isRun)
	{
		//カメラの角度を取得
		VECTOR cameraRot = camera_.lock()->GetAngles();
		Quaternion cameraQuaRot = camera_.lock()->GetQuaRotOutX();
		moveDeg_ = static_cast<double>(cameraRot.y) + UtilityCommon::Deg2RadD(deg);
		moveDir_= cameraQuaRot.PosAxis(dir);
	}


	//カードチャージ
	if (input_.IsPadBtnTrgDown(padNum_, CARD_CHARGE_BTN) || input_.IsTrgDown(CARD_CHARGE_KEY)) { isAct_.isCardCharge = true; }
	//カード使用
	if (input_.IsPadBtnTrgDown(padNum_, CARD_USE_BTN) || input_.IsTrgDown(CARD_USE_KEY)) { isAct_.isCardUse = true; }
	//カード移動
	if (IsCardLeft()) {
		isAct_.isCardMoveLeft = true; }
	if (IsCardRight()) {
		isAct_.isCardMoveRight = true; 
	}
	//カード使用キー長押し(リロード用)
	if (input_.IsPadBtnNew(padNum_, CARD_USE_BTN)|| input_.IsNew(CARD_USE_KEY)) { isAct_.isCardPushKeep = true; }
	//回避
	if (input_.IsPadBtnTrgDown(padNum_, DODGE_BTN) || input_.IsTrgDown(DODGE_KEY)) { isAct_.isDodge = true;}
}

void PlayerLogic::InputPad(void)
{
	using ATK_ACT = Player::ATK_ACT;
	actCntl_ = ACT_CNTL::NONE;

#ifdef _DEBUG
	//if (input_.IsTrgDown(InputManager::CONTROL_TYPE::DEBUG_CHANGE_INPUT, InputManager::JOYPAD_NO::PAD1, InputManager::TYPE::PAD))
	//{
	//	ChangeInput(InputManager::TYPE::ALL);
	//}
#endif // _DEBUG


	//スティックの倒れ値が200以上だったら
	if (inputS_.IsPressed(INPUT_EVENT::UP) || inputS_.IsPressed(INPUT_EVENT::DOWN)
		|| inputS_.IsPressed(INPUT_EVENT::RIGHT) || inputS_.IsPressed(INPUT_EVENT::LEFT))
	{ 
		actCntl_ = ACT_CNTL::MOVE;

		//スティックの角度を求める
		stickDeg_ = inputS_.GetLStickDeg(padNum_);
	}

	//スティックの角度によって移動方向を決める
	moveDeg_ = stickDeg_;
	VECTOR stickDir = { static_cast<float>(LStickAngleSize_.x) ,0.0f,static_cast<float>(-LStickAngleSize_.y) };
	//moveDir_ = { leftStickX_ ,0.0f,leftStickX_ };
	moveDir_ = VNorm(stickDir);
}

void PlayerLogic::KeyBoard(void)
{
}

void PlayerLogic::Pad(void)
{
}

bool PlayerLogic::IsCardRight(void)
{
	if (input_.IsKeyKeepPressed(CARD_MOVE_RIGHT_KEY, PUSH_KEEP_TIME)
		|| input_.IsBtnKeepPressed(padNum_, CARD_MOVE_RIGHT_BTN, PUSH_KEEP_TIME))
	{
		return true;
	}
	else if (input_.IsPadBtnTrgDown(padNum_, CARD_MOVE_RIGHT_BTN) || input_.IsTrgDown(CARD_MOVE_RIGHT_KEY))
	{
		return true;
	}
	return false;
}

bool PlayerLogic::IsCardLeft(void)
{
	if (input_.IsKeyKeepPressed(CARD_MOVE_LEFT_KEY, PUSH_KEEP_TIME)
		|| input_.IsBtnKeepPressed(padNum_, CARD_MOVE_LEFT_BTN, PUSH_KEEP_TIME))
	{
		return true;
	}
	else if (input_.IsPadBtnTrgDown(padNum_, CARD_MOVE_LEFT_BTN) || input_.IsTrgDown(CARD_MOVE_LEFT_KEY))
	{
		return true;
	}
	return false;
;
}
