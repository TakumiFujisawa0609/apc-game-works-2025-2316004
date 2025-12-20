#include "../pch.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Application.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Common/Easing.h"
#include "../Renderer/PixelRenderer.h"
#include "../Renderer/PixelMaterial.h"
#include "HpUIBase.h"

HpUIBase::HpUIBase(float& _hpPer, float& _preHp):
	hpPer_(_hpPer),
	preHp_(_preHp),
	waitCnt_(WAIT_TIME),
	resMng_(ResourceManager::GetInstance()),
	barCoverPos_({}),
	barPos_({}),
	initBarPos_(barPos_),
	initCoverPos_(barCoverPos_)
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
	float dis = preHp_ - hpPer_;
	if (dis > 0.0f)
	{
		float lerpStart = hpPer_ + dis;

		if (waitCnt_ < 0.0f)
		{
			preHp_ = easing_->EaseFunc(lerpStart, hpPer_, (LERP_TIME - lerpCnt_) / LERP_TIME, Easing::EASING_TYPE::LERP);
			lerpCnt_ -= SceneManager::GetInstance().GetDeltaTime();
		}
		waitCnt_ -= SceneManager::GetInstance().GetDeltaTime();
	}
	else
	{
		lerpCnt_ = LERP_TIME;
		waitCnt_ = WAIT_TIME;
	}

	if (shakeCnt_ > 0.0f)
	{
		barCoverPos_.y = easing_->EaseFunc(initCoverPos_.y, initCoverPos_.y+COVER_SHAKE_POW_Y,
			(SHAKE_CNT - shakeCnt_) / SHAKE_CNT, Easing::EASING_TYPE::ELASTIC_BACK);

		barPos_.y = easing_->EaseFunc(initBarPos_.y, initBarPos_.y+BAR_SHAKE_POW_Y,
			(SHAKE_CNT - shakeCnt_) / SHAKE_CNT, Easing::EASING_TYPE::ELASTIC_BACK);

		shakeCnt_ -= SceneManager::GetInstance().GetDeltaTime();
	}
}

void HpUIBase::Shake(void)
{
	shakeCnt_ = SHAKE_CNT;
}

void HpUIBase::Draw(void)
{
}
