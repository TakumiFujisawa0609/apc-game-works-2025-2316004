#include "../pch.h"
#include "../Application.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Card/CardUIDraw.h"
#include "CardUIController.h"

CardUIController::CardUIController(int& _cardNumImgs) :
	typeImg_(-1),
	cardNoImg_(_cardNumImgs),
	status_({}),
	upDownMoveAngle_(0.0f),
	disitionCnt_(0.0f),
	reactCnt_(0.0f),
	state_(CARD_STATE::DRAW_PILE),
	cardPos_({0.0f,0.0f}),
	currentAngle_(0.0f),
	goalAngle_(currentAngle_),
	cardScl_(1.0),
	sclCnt_(SCL_LERP_TIME)
{
}

CardUIController::~CardUIController(void)
{
}

void CardUIController::Load(void)
{
	cardDraw_ = std::make_unique<CardUIDraw>(cardImg_, cardPos_, cardScl_);
	cardDraw_->Load();
}

void CardUIController::Init(void)
{
	cardImg_ = MakeCardUIImg();
	cardDraw_->Init();
}
void CardUIController::Update(void)
{
	cardDraw_->Update();
}
void CardUIController::Draw(void)
{
	//カードの描画
	//DrawRotaGraphF(cardPos_.x, cardPos_.y, cardScl_, 0.0f, cardImg_, true);
	cardDraw_->Draw();
}

void CardUIController::DrawModel(void)
{
	cardDraw_->DrawModel();
}

void CardUIController::DrawReloadGauge(const float&_reloadCardFrameImg,const float& _reloadPer)
{
	cardDraw_->DrawReloadGauge(_reloadCardFrameImg,_reloadPer);
}

void CardUIController::SelectCardDrawFrame(const int& _frameImg)
{
	cardDraw_->DrawSelectedFrame(_frameImg);
}

void CardUIController::DecisionMove(void)
{
	disitionCnt_ -= UtilityCommon::FIXED_DELTA_TIME;
	cardPos_ = UtilityCommon::Lerp(baseCardPos_, DISITON_CARD_POS,
		(DISITION_MOVE_CARD_TIME - disitionCnt_) / DISITION_MOVE_CARD_TIME);
}

void CardUIController::ReactMove(const Vector2F& _goalPos)
{
	cardPos_ = UtilityCommon::Lerp(baseCardPos_, _goalPos,
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
		sclCnt_ = 0.0f;
	}
}

void CardUIController::MoveOnRevolver(const float& _cnt,const float& moveTimeMax)
{
	float time = (moveTimeMax - _cnt) / moveTimeMax;
	float startRad = currentAngle_;
	float goalRad = goalAngle_;
	currentAngle_ = UtilityCommon::LerpRad(startRad
		, goalRad, time);

	cardPos_.x = CENTER_X + std::sin(currentAngle_) * RADIUS_X;
	cardPos_.y = CENTER_Y - std::cos(currentAngle_) * RADIUS_Y;
}

void CardUIController::MoveUpDown(void)
{
	upDownMoveAngle_ += UP_DOWN_MOVE_SPEED;
	cardPos_.y = baseCardPos_.y + sin(upDownMoveAngle_) * UP_DOWN_MOVE_RADIUS;
}

void CardUIController::ChangeDicisionEnemyCardMove(void)
{
	cardPos_ = ENEMY_SELECT_CARD_START_POS;
	numPos_ = cardPos_ + (NUM_LOCAL_POS * cardScl_);
}

void CardUIController::SyncCardAngleAndPos(void)
{
	currentAngle_ = goalAngle_;
	cardPos_.x = CENTER_X + std::sin(currentAngle_) * RADIUS_X;
	cardPos_.y = CENTER_Y - std::cos(currentAngle_) * RADIUS_Y;
}

void CardUIController::ResetCount(void)
{
	sclCnt_ = SCL_LERP_TIME;
	disitionCnt_ = DISITION_MOVE_CARD_TIME;
	cardScl_ = 1.0f;
	reactCnt_ = REACT_MOVE_CARD_TIME;
	state_ = CARD_STATE::DRAW_PILE;
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
}

void CardUIController::ChangeUsedCard(void)
{
	if (state_ == CARD_STATE::REACT || state_ == CARD_STATE::USED||disitionCnt_ > 0.0f)return;
	state_ = CARD_STATE::USED;
}

void CardUIController::ChangeReactCard(void)
{
	if (state_ == CARD_STATE::USED|| state_ == CARD_STATE::REACT)return;
	state_ = CARD_STATE::REACT;
	reactCnt_ = REACT_MOVE_CARD_TIME;
}

void CardUIController::ChangeUsing(void)
{
	if (state_ == CARD_STATE::USED || state_ == CARD_STATE::REACT)return;
	state_ = CARD_STATE::USING;
}

int CardUIController::MakeCardUIImg(void)
{
	//サイズ
	int img = -1;
	const int GRAPH_SIZE_X = 120;
	const int GRAPH_SIZE_Y = 160;
	constexpr float NUM_SCL = 0.18f;
	//描画可能なスクリーンの作成
	img = MakeScreen(GRAPH_SIZE_X, GRAPH_SIZE_Y, true);

	//描画先を作成したスクリーンに変更
	SetDrawScreen(img);
	//作成したスクリーンで描画
	DrawGraph(0, 0, typeImg_, true);
	//中央座標取得
	Vector2F centerPos;
	GetGraphSizeF(typeImg_, &centerPos.x, &centerPos.y);
	centerPos /= 2.0f;
	//番号サイズ取得
	Vector2F size = { 0.0f,0.0f };
	GetGraphSizeF(cardNoImg_, &size.x, &size.y);
	size *= NUM_SCL;

	//中央合わせ
	centerPos -= size / 2.0f;

	//座標計算
	Vector2F numSizeHalf = size;
	Vector2F leftTopPos = { centerPos.x + NUM_LOCAL_POS.x,centerPos.y + NUM_LOCAL_POS.y };
	Vector2F rightBottomPos = { centerPos.x+NUM_LOCAL_POS.x + numSizeHalf.x,centerPos.y+NUM_LOCAL_POS.y + numSizeHalf.y };
	DrawExtendGraphF(leftTopPos.x, leftTopPos.y, rightBottomPos.x, rightBottomPos.y, cardNoImg_, true);



	//描画先を元に戻す
	SetDrawScreen(DX_SCREEN_BACK);
	return img;


}