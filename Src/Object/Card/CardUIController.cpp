#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include "CardUIController.h"

CardUIController::CardUIController(int& _cardNumImgs) :
	typeImg_(-1),
	cardNoImgs_(_cardNumImgs),
	status_({}),
	disitionCnt_(0.0f),
	reactCnt_(0.0f),
	state_(CARD_STATE::DRAW_PILE),
	cardPos_({ Application::SCREEN_SIZE_X + 180,Application::SCREEN_HALF_Y * 222 }),
	numPos_({ Application::SCREEN_SIZE_X + 180,Application::SCREEN_HALF_Y * 222 }),
	currentAngle_(0.0f),
	goalAngle_(currentAngle_),
	cardScl_(1.0),
	sclCnt_(SCL_LERP_TIME)
{
}

CardUIController::~CardUIController(void)
{
}

void CardUIController::Init(void)
{
}
void CardUIController::Update(void)
{
	//番号座標同期
	numPos_ = cardPos_ + (NUM_LOCAL_POS * cardScl_);
}
void CardUIController::Draw(void)
{
	//GetGraphSizeF(typeImg_, &size_.x, &size_.y);

	//////左上の座標
	//Vector2F rightPos = cardPos_ - size_ * cardScl_;
	////右下の座標
	//Vector2F leftPos = cardPos_ + size_ * cardScl_;

	//DrawExtendGraphF(
	//	rightPos.x, rightPos.y,
	//	leftPos.x, leftPos.y,
	//	typeImg_,
	//	true
	//);


	constexpr double NUM_SCL = 0.18;
	//カードの描画
	DrawRotaGraphF(cardPos_.x, cardPos_.y, cardScl_, 0.0f, typeImg_, true);

	//int num = status_.pow_ - 1;
	//if (num == -1) { num = 9; }
	//DrawRotaGraphF(numPos_.x, numPos_.y, cardScl_ * NUM_SCL, 0.0f, cardNoImgs_[num], true);


}
void CardUIController::DecisionMove(void)
{
	disitionCnt_ -= UtilityCommon::FIXED_DELTA_TIME;
	cardPos_ = UtilityCommon::Lerp(cardPos_, DISITON_CARD_POS,
		(DISITION_MOVE_CARD_TIME - disitionCnt_) / DISITION_MOVE_CARD_TIME);
}

void CardUIController::ReactMove(const Vector2F& _goalPos)
{
	cardPos_ = UtilityCommon::Lerp(cardPos_, _goalPos,
		(REACT_MOVE_CARD_TIME - reactCnt_) / REACT_MOVE_CARD_TIME);
	reactCnt_ -= UtilityCommon::FIXED_DELTA_TIME;
}

void CardUIController::ReactUpdate(const Vector2F& _goalPos)
{
	if (state_ != CARD_STATE::REACT)return;
	//まだ決定移動中ならそちらを優先
	if (disitionCnt_ > 0.0f)
	{
		DecisionMove();
		return;
	}
	//弾かれ移動
	ReactMove(_goalPos);
	if (reactCnt_ <= 0.0f)
	{
		state_ = CARD_STATE::USED;
		sclCnt_ = SCL_LERP_TIME;
	}
}

void CardUIController::MoveOnRevolver(const float& _cnt)
{
	float time = (SELECT_MOVE_CARD_TIME - _cnt) / SELECT_MOVE_CARD_TIME;
	float startRad = currentAngle_;
	float goalRad = goalAngle_;
	currentAngle_ = UtilityCommon::LerpRad(startRad
		, goalRad, time);

	cardPos_.x = CENTER_X + std::sin(currentAngle_) * RADIUS_X;
	cardPos_.y = CENTER_Y - std::cos(currentAngle_) * RADIUS_Y;
}


void CardUIController::EraseUsedCard(void)
{
	if (state_ != CARD_STATE::USED)return;
	cardScl_ = UtilityCommon::Lerp(cardScl_, 0.0f, (SCL_LERP_TIME - sclCnt_) / SCL_LERP_TIME);
	sclCnt_ -= static_cast<double>(UtilityCommon::FIXED_DELTA_TIME);
}

void CardUIController::InitCard(const int& _num)
{
	currentAngle_ = static_cast<float>(ARROUND_PER_RAD * _num - ARROUND_PER_RAD);
	cardPos_.x = CENTER_X + std::sin(currentAngle_) * RADIUS_X;
	cardPos_.y = CENTER_Y - std::cos(currentAngle_) * RADIUS_Y;
	//常に強さ番号座標をローカル座標分追従させる
	numPos_ = cardPos_ + (NUM_LOCAL_POS * cardScl_);
}

void CardUIController::ChangeUsedCard(void)
{
	if (state_ == CARD_STATE::REACT || disitionCnt_ > 0.0f)return;
	state_ = CARD_STATE::USED;
}

void CardUIController::ChangeReactCard(void)
{
	state_ = CARD_STATE::REACT;
	reactCnt_ = REACT_MOVE_CARD_TIME;
}

void CardUIController::ChangeUsing(void)
{
	if (state_ == CARD_STATE::USED || state_ == CARD_STATE::REACT)return;
	state_ = CARD_STATE::USING;
}

