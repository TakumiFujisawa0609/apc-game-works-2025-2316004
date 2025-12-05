#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Renderer/PixelMaterial.h"
#include "../Renderer/PixelRenderer.h"
#include "CardUIDraw.h"

CardUIDraw::CardUIDraw(int& _typeImg,Vector2F& _centerPos, float& _scl):
	typeImg_(_typeImg),
	centerPos_(_centerPos),
	scl_(_scl)
{
}

CardUIDraw::~CardUIDraw(void)
{
	typeImg_ = -1;
}
void CardUIDraw::Load(void)
{
	//trans_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::CARD_MDL));
}

void CardUIDraw::Init(void)
{
	//画像サイズ取得
	GetGraphSizeF(typeImg_, &size_.x, &size_.y);
	halfSize_ = size_ / 2.0f;
	//左上の座標
	rightTopPos_ = centerPos_ - halfSize_ * scl_;
	//右下の座標
	leftDownPos_ = centerPos_ + halfSize_ * scl_;

	Vector2 rightTopPos = {static_cast<int>(rightTopPos_.x),static_cast<int>(rightTopPos_.y)};
	Vector2 size = {};
	size = { static_cast<int>(size_.x),static_cast<int>(size_.y) };


	trans_.pos = CARD_INIT_POS;
	trans_.quaRot = Quaternion();
	trans_.scl = { CARD_SCL,CARD_SCL,CARD_SCL };
	trans_.quaRotLocal =
		Quaternion::Euler({ 0.0f,0.0f,0.0f });

	normalCardPSMaterial_ = std::make_unique<PixelMaterial>(L"CardNormalPS.cso", CONST_BUF_SLOT_NUM);
	normalCardPSMaterial_->AddTextureBuf(typeImg_);
	normalCardPSMaterial_->AddConstBuf({ 0.0f,0.0f, 0.0f,1.0f });		//カードの色
	normalCardPSMaterial_->AddConstBuf({ 1.0f,0.0f, 0.0f,1.0f });		//アウトラインの色
	normalCardPSMaterial_->AddConstBuf({ 0.0f,0.0f, 0.0f,1.0f });		//アウトラインの広がる時間
	normalCardPSRenderer_ = std::make_unique<PixelRenderer>(*normalCardPSMaterial_);
	normalCardPSRenderer_->MakeSquareVertex(rightTopPos_, size_);

	reloadCardPSMaterial_= std::make_unique<PixelMaterial>(L"CardReloadPS.cso", CONST_BUF_SLOT_NUM);
	reloadCardPSMaterial_->AddTextureBuf(ResourceManager::GetInstance().Load(ResourceManager::SRC::RELOAD_GAGE).handleId_);
	reloadCardPSMaterial_->AddConstBuf({ 0.0f,0.0f, 0.0f,1.0f });		//カードの色
	reloadCardPSMaterial_->AddConstBuf({ 1.0f,0.0f, 0.0f,1.0f });		//アウトラインの色
	reloadCardPSMaterial_->AddConstBuf({ 0.0f,0.0f, 0.0f,1.0f });		//アウトラインの広がる時間
	reloadCardPSRederer_ = std::make_unique<PixelRenderer>(*reloadCardPSMaterial_);
	reloadCardPSRederer_->MakeSquareVertex(rightTopPos_, size_);

	trans_.Update();
}
void CardUIDraw::Update(void)
{
	trans_.Update();
}
void CardUIDraw::Draw(void)
{
	float totalTime = SceneManager::GetInstance().GetTotalTime();
	normalCardPSMaterial_->SetConstBuf(1, { 0.0f,0.0f, 0.0f,1.0f });		//カードの色
	DrawCard();
}

void CardUIDraw::DrawSelectedFrame(const int& _frameImg)
{

	float totalTime = SceneManager::GetInstance().GetTotalTime();
	normalCardPSMaterial_->SetConstBuf(1, { SELECT_FOG_STRENGTH,totalTime, 0.0f,1.0f });		//カードの色
	//カード描画
	DrawCard();

	//左上の座標
	//Vector2F rightTopPos = centerPos_ - halfSize * scl_;//選択中のカード追
	Vector2F rightTopPos = LEFT_UP_FRAME_POS;
	//右下の座標
	//Vector2F leftDownPos = centerPos_ + halfSize * scl_;
	Vector2F leftDownPos = RIGHT_DOWN_FRAME_POS;
	DrawExtendGraphF(
		rightTopPos.x, rightTopPos.y,
		leftDownPos.x, leftDownPos.y,
		_frameImg,
		true
	);
}

void CardUIDraw::DrawModel(void)
{
	MV1DrawModel(trans_.modelId);
}

void CardUIDraw::DrawCard(void)
{
	//画像サイズ取得
	GetGraphSizeF(typeImg_, &size_.x, &size_.y);

	halfSize_ = size_ / 2.0f;
	//左上の座標
	Vector2F rightTopPos = centerPos_ - halfSize_ * scl_;
	//右下の座標
	Vector2F leftDownPos = centerPos_ + halfSize_ * scl_;

	normalCardPSRenderer_->SetSize(size_ * scl_);
	normalCardPSRenderer_->Draw(rightTopPos.x, rightTopPos.y);
}

void CardUIDraw::DrawReloadGauge(const int& _reloadFrameImg,const float& _reloadPer)
{
	//画像サイズ取得
	GetGraphSizeF(typeImg_, &size_.x, &size_.y);

	halfSize_ = size_ / 2.0f;
	//左上の座標
	Vector2F rightTopPos = centerPos_ - halfSize_ * scl_;
	//右下の座標
	Vector2F leftDownPos = centerPos_ + halfSize_ * scl_;
	reloadCardPSMaterial_->SetConstBuf(1, { 0.0f,0.0f,_reloadPer,0.0f });
	reloadCardPSRederer_->SetSize(size_ * scl_);
	reloadCardPSRederer_->Draw(rightTopPos.x, rightTopPos.y);

	DrawExtendGraphF(
		rightTopPos.x, rightTopPos.y,
		leftDownPos.x, leftDownPos.y,
		_reloadFrameImg,
		true
	);
}
