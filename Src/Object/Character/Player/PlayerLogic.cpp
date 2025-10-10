#include"../Object/Character/Player/Player.h"
#include"../Utility/Utility3D.h"
#include "PlayerLogic.h"

PlayerLogic::PlayerLogic(InputManager::JOYPAD_NO _padNum, InputManager::CONTROLL_TYPE _cntl):
	padNum_(_padNum),
	cntl_(_cntl)
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
	auto& ins = InputManager::GetInstance();
	using ATK_ACT = Player::ATK_ACT;
	//actCntl_ = ACT_CNTL::NONE;

#ifdef _DEBUG
	//if (ins.IsTrgDown(InputManager::CONTROL_TYPE::DEBUG_CHANGE_INPUT, InputManager::JOYPAD_NO::PAD1, InputManager::TYPE::PAD))
	//{
	//	ChangeInput(InputManager::TYPE::PAD);
	//}
#endif // _DEBUG
	
	//移動角度を決める
	if (ins.IsNew(MOVE_FRONT_KEY))
	{ 
		moveDeg_ = FLONT_DEG;
		moveDir_ = Utility3D::DIR_F;

	}
	else if (ins.IsNew(MOVE_LEFT_KEY))
	{ 
		moveDeg_ = LEFT_DEG; 
		moveDir_ = Utility3D::DIR_L;
	} 
	else if (ins.IsNew(MOVE_BACK_KEY))
	{ 
		moveDeg_ = BACK_DEG; 
		moveDir_ = Utility3D::DIR_B;
	}
	else if (ins.IsNew(MOVE_RIGHT_KEY))
	{
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

void PlayerLogic::InputAll(void)
{
	auto& ins = InputManager::GetInstance();
	using ATK_ACT = Player::ATK_ACT;
	isAct_ = {};
	moveDir_ = Utility3D::VECTOR_ZERO;
	//移動角度を決める
	if (ins.IsNew(MOVE_FRONT_KEY))
	{
		moveDeg_ = FLONT_DEG;
		moveDir_ = Utility3D::DIR_F;
	}
	else if (ins.IsNew(MOVE_LEFT_KEY))
	{
		moveDeg_ = LEFT_DEG;
		moveDir_ = Utility3D::DIR_L;
	}
	else if (ins.IsNew(MOVE_BACK_KEY))
	{
		moveDeg_ = BACK_DEG;
		moveDir_ = Utility3D::DIR_B;
	}
	else if (ins.IsNew(MOVE_RIGHT_KEY))
	{
		moveDeg_ = RIGHT_DEG;
		moveDir_ = Utility3D::DIR_R;
	}

	auto& inputS = InputManagerS::GetInstance();
	
	//スティックの倒れ値が200以上だったら
	if (inputS.IsPressed(INPUT_EVENT::UP) || inputS.IsPressed(INPUT_EVENT::DOWN)
		|| inputS.IsPressed(INPUT_EVENT::RIGHT) || inputS.IsPressed(INPUT_EVENT::LEFT))
	{
		//スティックサイズの取得
		LStickAngleSize_ = inputS.GetKnockLStickSize(padNum_);

		//スティックの角度を求める
		stickDeg_ = inputS.GetLStickDeg(padNum_);

		//スティックの角度によって移動方向を決める
		moveDeg_ = stickDeg_;

		//ベクトルの計算
		VECTOR stickDir = { static_cast<float>(LStickAngleSize_.x) ,0.0f,static_cast<float>(-LStickAngleSize_.y)};
		moveDir_ = VNorm(stickDir);
	}
	//カード使用
	if (ins.IsPadBtnTrgDown(padNum_, CARD_CHARGE_BTN) || ins.IsTrgDown(CARD_CHARGE_KEY)) { isAct_.isCardCharge = true; }
	//ジャンプキー
	if (ins.IsPadBtnTrgDown(padNum_, CARD_USE_BTN) || ins.IsTrgDown(CARD_USE_KEY)) { isAct_.isCardUse = true; }

	if (ins.IsPadBtnTrgDown(padNum_, CARD_MOVE_LEFT_BTN) || ins.IsTrgDown(CARD_MOVE_LEFT_KEY)) {
		isAct_.isCardMoveLeft = true; }
	if (ins.IsPadBtnTrgDown(padNum_, CARD_MOVE_RIGHT_BTN)|| ins.IsTrgDown(CARD_MOVE_RIGHT_KEY)) { isAct_.isCardMoveRight = true; }
	if (ins.IsPadBtnNew(padNum_, CARD_USE_BTN)|| ins.IsNew(CARD_USE_KEY)) { isAct_.isCardPushKeep = true; }
}

void PlayerLogic::InputPad(void)
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
}

void PlayerLogic::KeyBoard(void)
{
}

void PlayerLogic::Pad(void)
{
}
