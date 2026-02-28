#include "../pch.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Application.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Common/Easing.h"
#include "../Renderer/PixelRenderer.h"
#include "../Renderer/PixelMaterial.h"
#include "HpUIBase.h"

HpUIBase::HpUIBase(void):
	//hpPer_(1.0f),
	//preHp_(1.0f),
	hpData_{1.0f,1.0f},
	waitCnt_(WAIT_TIME),
	resMng_(ResourceManager::GetInstance()),
	barCoverPos_({}),
	barPos_({}),
	initBarPos_(barPos_),
	initCoverPos_(barCoverPos_),
	hpDis_()
{
}

HpUIBase::~HpUIBase(void)
{
}

void HpUIBase::Load(void)
{
}

void HpUIBase::Init(void)
{
	easing_ = std::make_unique<Easing>();
}

void HpUIBase::Update(void)
{

	//HPの減らし幅ゲージの補完
	if (hpDis_ > 0.0f)
	{
		float lerpStart = hpData_.hpPer + hpDis_;

		if (waitCnt_ < 0.0f)
		{
			hpData_.preHpPer = easing_->EaseFunc(lerpStart, hpData_.hpPer, (LERP_TIME - lerpCnt_) / LERP_TIME, Easing::EASING_TYPE::LERP);
			hpDis_= hpData_.preHpPer - hpData_.hpPer;
			lerpCnt_ -= SceneManager::GetInstance().GetDeltaTime();
		}
		waitCnt_ -= SceneManager::GetInstance().GetDeltaTime();
	}
	else
	{
		lerpCnt_ = LERP_TIME;
		waitCnt_ = WAIT_TIME;
	}

	//HPバーのシェイク
	if (shakeCnt_ > 0.0f)
	{
		barCoverPos_.y = easing_->EaseFunc(initCoverPos_.y, initCoverPos_.y+COVER_SHAKE_POW_Y,
			(SHAKE_CNT - shakeCnt_) / SHAKE_CNT, Easing::EASING_TYPE::ELASTIC_BACK);

		barPos_.y = easing_->EaseFunc(initBarPos_.y, initBarPos_.y+BAR_SHAKE_POW_Y,
			(SHAKE_CNT - shakeCnt_) / SHAKE_CNT, Easing::EASING_TYPE::ELASTIC_BACK);

		shakeCnt_ -= SceneManager::GetInstance().GetDeltaTime();
	}
}

void HpUIBase::SetShakeTime(void)
{
	shakeCnt_ = SHAKE_CNT;
}

void HpUIBase::RefreshHp(const HP_DATA& _hpData)
{
	hpData_.preHpPer = _hpData.preHpPer;
	hpData_.hpPer = _hpData.hpPer;
	hpDis_= hpData_.preHpPer - hpData_.hpPer;
}

void HpUIBase::Draw(void)
{
}
