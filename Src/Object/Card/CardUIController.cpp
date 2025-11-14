#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include "CardUIController.h"

CardUIController::CardUIController(int& _typeImg, CardBase::CARD_STATUS& _status) :
	typeImg_(_typeImg),
	status_(_status),
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
	//size_.x = GetGraphSizeF(typeImg_, &size_.x, &size_.y);
	//size_.y = GetGraphSizeF(typeImg_, &size_.x, &size_.y);

	////左上の座標
	//Vector2F rightPos = centerPos_ - size_ * scl_;
	////右下の座標
	//Vector2F leftPos = centerPos_ + size_ * scl_;

	//DrawExtendGraphF(
	//	0, 0,
	//	static_cast<int>(size_.x), static_cast<int>(size_.y),
	//	typeImg_,
	//	true
	//);
}
void CardUIController::DisitionMove(void)
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
		DisitionMove();
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
	currentAngle_ = ARROUND_NUM_PER_QUAD * _num - ARROUND_NUM_PER_QUAD;
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

