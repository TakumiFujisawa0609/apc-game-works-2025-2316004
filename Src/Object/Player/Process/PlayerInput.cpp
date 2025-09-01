#include"../Object/Player/Player.h"
#include"../Utility/Utility3D.h"
#include "PlayerInput.h"

PlayerInput::PlayerInput(InputManager::JOYPAD_NO _padNum, InputManager::CONTROLL_TYPE _cntl):padNum_(_padNum),cntl_(_cntl)
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

PlayerInput::~PlayerInput(void)
{
}

void PlayerInput::Init(void)
{
	actCntl_ = ACT_CNTL::NONE;
	leftStickX_ = -1;
	leftStickY_ = -1;
	stickDeg_ = -1;
	moveDeg_ = 0.0f;
	moveDir_ = Utility3D::VECTOR_ZERO;
}

void PlayerInput::Update(void)
{
	inputUpdates_[cntl_]();
	//InputKeyBoard();
	//InputPad();
	//switch (cntl_)
	//{
	//case DateBank::TYPE::ERORE:
	//	break;
	//case DateBank::TYPE::ALL:
	//	InputKeyBoard();
	//	break;
	//case DateBank::TYPE::CONTROLLER:
	//	InputPad();
	//	break;
	//default:
	//	break;
	//}
}

void PlayerInput::InputKeyBoard(void)
{
	auto& ins = InputManager::GetInstance();
	using ATK_ACT = Player::ATK_ACT;
	actCntl_ = ACT_CNTL::NONE;

#ifdef _DEBUG
	//if (ins.IsTrgDown(InputManager::CONTROL_TYPE::DEBUG_CHANGE_INPUT, InputManager::JOYPAD_NO::PAD1, InputManager::TYPE::PAD))
	//{
	//	ChangeInput(InputManager::TYPE::PAD);
	//}
#endif // _DEBUG
	
	//移動角度を決める
	if (ins.IsNew(MOVE_FRONT_KEY))
	{ 
		actCntl_ = ACT_CNTL::MOVE;
		moveDeg_ = FLONT_DEG;
		moveDir_ = Utility3D::DIR_F;
	}
	else if (ins.IsNew(MOVE_LEFT_KEY))
	{ 
		actCntl_ = ACT_CNTL::MOVE;
		moveDeg_ = LEFT_DEG; 
		moveDir_ = Utility3D::DIR_L;
	} 
	else if (ins.IsNew(MOVE_BACK_KEY))
	{ 
		actCntl_ = ACT_CNTL::MOVE;
		moveDeg_ = BACK_DEG; 
		moveDir_ = Utility3D::DIR_B;
	}
	else if (ins.IsNew(MOVE_RIGHT_KEY))
	{
		actCntl_ = ACT_CNTL::MOVE;
		moveDeg_ = RIGHT_DEG; 
		moveDir_ = Utility3D::DIR_R;
	}
	//if (actCntl_==ACT_CNTL::MOVE&&ins.IsNew(DASH_KEY))
	//{
	//	actCntl_ = ACT_CNTL::DASHMOVE;
	//}

	//カード使用
	if (ins.IsTrgDown(CARD_CHARGE_KEY)) { actCntl_ = ACT_CNTL::CARD_CHARGE; }

	//ジャンプキー
	if (ins.IsTrgDown(CARD_USE_KEY)) { actCntl_ = ACT_CNTL::CARD_USE; }
}

void PlayerInput::InputAll(void)
{
	auto& ins = InputManager::GetInstance();
	using ATK_ACT = Player::ATK_ACT;
	actCntl_ = ACT_CNTL::NONE;


	//移動角度を決める
	if (ins.IsNew(MOVE_FRONT_KEY))
	{
		actCntl_ = ACT_CNTL::MOVE;
		moveDeg_ = FLONT_DEG;
		moveDir_ = Utility3D::DIR_F;
	}
	else if (ins.IsNew(MOVE_LEFT_KEY))
	{
		actCntl_ = ACT_CNTL::MOVE;
		moveDeg_ = LEFT_DEG;
		moveDir_ = Utility3D::DIR_L;
	}
	else if (ins.IsNew(MOVE_BACK_KEY))
	{
		actCntl_ = ACT_CNTL::MOVE;
		moveDeg_ = BACK_DEG;
		moveDir_ = Utility3D::DIR_B;
	}
	else if (ins.IsNew(MOVE_RIGHT_KEY))
	{
		actCntl_ = ACT_CNTL::MOVE;
		moveDeg_ = RIGHT_DEG;
		moveDir_ = Utility3D::DIR_R;
	}

	auto& inputS = InputManagerS::GetInstance();
	
	//スティックの倒れ値が200以上だったら
	if (inputS.IsPressed(INPUT_EVENT::UP) || inputS.IsPressed(INPUT_EVENT::DOWN)
		|| inputS.IsPressed(INPUT_EVENT::RIGHT) || inputS.IsPressed(INPUT_EVENT::LEFT))
	{
		actCntl_ = ACT_CNTL::MOVE;
		//if (ins.IsNew(InputManager::CONTROLL_TYPE::PLAYER_DASH, padNum_, InputManager::TYPE::PAD))
		//{
		//	actCntl_ = ACT_CNTL::DASHMOVE;
		//}
		//スティックの角度を求める
		stickDeg_ = inputS.GetLStickDeg(padNum_);
		//スティックの角度によって移動方向を決める
		moveDeg_ = stickDeg_;
		VECTOR stickDir = { static_cast<float>(LStickAngleSize_.x) ,0.0f,static_cast<float>(-LStickAngleSize_.y)};
		//moveDir_ = { leftStickX_ ,0.0f,leftStickX_ };
		moveDir_ = VNorm(stickDir);
	}
	//カード使用
	if (ins.IsPadBtnTrgDown(padNum_,CARD_CHARGE_BTN)|| ins.IsTrgDown(CARD_CHARGE_KEY)) { actCntl_ = ACT_CNTL::CARD_CHARGE; }

	//ジャンプキー
	if (ins.IsPadBtnTrgDown(padNum_, CARD_USE_BTN)|| ins.IsTrgDown(CARD_USE_KEY)) { actCntl_ = ACT_CNTL::CARD_USE; }


	if (ins.IsPadBtnTrgDown(padNum_, CARD_MOVE_LEFT_BTN)|| ins.IsTrgDown(CARD_MOVE_LEFT_KEY)) { actCntl_ = ACT_CNTL::CARD_MOVE_LEFT; }
	if (ins.IsPadBtnTrgDown(padNum_, CARD_MOVE_RIGHT_BTN)|| ins.IsTrgDown(CARD_MOVE_RIGHT_KEY)) { actCntl_ = ACT_CNTL::CARD_MOVE_RIGHT; }
}

void PlayerInput::InputPad(void)
{
	auto& ins = InputManager::GetInstance();
	using ATK_ACT = Player::ATK_ACT;
	actCntl_ = ACT_CNTL::NONE;

#ifdef _DEBUG
	//if (ins.IsTrgDown(InputManager::CONTROL_TYPE::DEBUG_CHANGE_INPUT, InputManager::JOYPAD_NO::PAD1, InputManager::TYPE::PAD))
	//{
	//	ChangeInput(InputManager::TYPE::ALL);
	//}
#endif // _DEBUG




	// 左スティックの横軸
	//leftStickX_ = ins.GetJPadInputState(padNum_).AKeyLX;
	//縦軸
	//leftStickY_ = ins.GetJPadInputState(padNum_).AKeyLY;
	//auto stickRad = static_cast<float>(atan2(static_cast<double>(leftStickY_), static_cast<double>(leftStickX_)));

	auto& inputS = InputManagerS::GetInstance();

	//スティックの倒れ値が200以上だったら
	if (inputS.IsPressed(INPUT_EVENT::UP) || inputS.IsPressed(INPUT_EVENT::DOWN)
		|| inputS.IsPressed(INPUT_EVENT::RIGHT) || inputS.IsPressed(INPUT_EVENT::LEFT))
	{ 
		actCntl_ = ACT_CNTL::MOVE;
		//if (ins.IsNew(InputManager::CONTROL_TYPE::PLAYER_DASH, padNum_, InputManager::TYPE::PAD))
		//{
		//	actCntl_ = ACT_CNTL::DASHMOVE;
		//}
		//スティックの角度を求める
		stickDeg_ = inputS.GetLStickDeg(padNum_);
	}

	//スティックの角度によって移動方向を決める
	moveDeg_ = stickDeg_;
	VECTOR stickDir = { static_cast<float>(LStickAngleSize_.x) ,0.0f,static_cast<float>(-LStickAngleSize_.y) };
	//moveDir_ = { leftStickX_ ,0.0f,leftStickX_ };
	moveDir_ = VNorm(stickDir);

	//if (ins.IsTrgDown(InputManager::CONTROL_TYPE::PLAYER_PUNCH,padNum_,InputManager::TYPE::PAD) )
	//{ 
	//	actCntl_ = ACT_CNTL::PUNCH; 
	//}
	//if(ins.IsTrgDown(InputManager::CONTROL_TYPE::PLAYER_JUMP, padNum_, InputManager::TYPE::PAD))
	//{
	//	actCntl_ = ACT_CNTL::JUMP; 
	//}
}

void PlayerInput::KeyBoard(void)
{
}

void PlayerInput::Pad(void)
{
}
