#include "../pch.h"
#include "../Utility/UtilityCommon.h"
#include"../Manager/Generic/InputManager.h"
#include"../Manager/Generic/SceneManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Card/CardUIDraw.h"
#include "../Card/CardUIController.h"
#include "../Renderer/PixelMaterial.h"
#include "../Renderer/PixelRenderer.h"
#include "CardUIBase.h"

//const std::wstring Application::PATH_IMAGE = L"Data/Image/";
//const std::wstring CardUIBase::CARD_ZERO_NAME = L"Zero";
//const std::wstring CardUIBase::CARD_ONE_NAME = L"One";
//const std::wstring CardUIBase::CARD_TWO_NAME = L"Two";
//const std::wstring CardUIBase::CARD_THREE_NAME = L"Three";
//const std::wstring CardUIBase::CARD_FOUR_NAME = L"Four";
//const std::wstring CardUIBase::CARD_FIVE_NAME = L"Five";
//const std::wstring CardUIBase::CARD_SIX_NAME = L"Six";
//const std::wstring CardUIBase::CARD_SEVEN_NAME = L"Seven";
//const std::wstring CardUIBase::CARD_EIGHT_NAME = L"Eight";
//const std::wstring CardUIBase::CARD_NINE_NAME = L"Nine";


CardUIBase::CardUIBase(void):
selectState_(CARD_SELECT::RELOAD_WAIT),
atkCardImg_(-1),
reloadCardImg_(-1)
{
	//material_ = std::make_unique<PixelMaterial>(L"CardUI",0);
	//renderer_ = std::make_unique<PixelRenderer>(*material_);
	//renderer_->MakeSquereVertex(Vector2{},GetGraphSize(ResourceManager::GetInstance().Load(ResourceManager::SRC::))
}

CardUIBase::~CardUIBase(void)
{
}

void CardUIBase::ChangeSelectState(const CARD_SELECT _select)
{
	if (selectState_ == _select)return;
	selectState_ = _select;
	changeMoveState_[selectState_]();
}

void CardUIBase::CardDecision(void)
{

}
void CardUIBase::ChangeUsedActionCard(void)
{
	for (auto& act : actions_)
	{
		act->ChangeUsedCard();
	}
}

void CardUIBase::ChangeReactActionCard(void)
{
	for (auto& act : actions_)
	{
		act->ChangeReactCard();
	}
}

void CardUIBase::ChangeUsingActionCard(void)
{
	for (auto& act : actions_)
	{
		act->ChangeUsing();
	}
}

void CardUIBase::AddCardUi(const CardBase::CARD_STATUS _status)
{
	CardBase::CARD_STATUS status = {};
	int typeImg = -1;
	std::wstring numImgStr = L"";
	//属性によって画像を変える
	typeImg = GetTypeImg(_status);
	numImgStr = numImgNames_[static_cast<CARD_NUMBER>(_status.pow_)];
	int num = _status.pow_ - 1;
	if (num == -1) { num = 9; }
	int drawNumImg = cardNoImg_[num];
	std::shared_ptr<CardUIController> info = std::make_shared<CardUIController>(drawNumImg, numImgStr);
	info->SetTypeImg_(typeImg);
	info->SetStatus(_status);
	info->Load();
	info->Init();

	//配列に挿入
	initialCards_.emplace_back(info);
}

void CardUIBase::InitNumImgNames(void)
{
	numImgNames_ = {
		{CARD_NUMBER::CARD_ZERO,L"Zero"},
		{CARD_NUMBER::CARD_ONE,L"One"},
		{CARD_NUMBER::CARD_TWO,L"Two"},
		{CARD_NUMBER::CARD_THREE,L"Three"},
		{CARD_NUMBER::CARD_FOUR,L"Four"},
		{CARD_NUMBER::CARD_FIVE,L"Five"},
		{CARD_NUMBER::CARD_SIX,L"Six"},
		{CARD_NUMBER::CARD_SEVEN,L"Seven"},
		{CARD_NUMBER::CARD_EIGHT,L"Eight"},
		{CARD_NUMBER::CARD_NINE,L"Nine"}
	};
}

void CardUIBase::InitTypeImgs(void)
{
	typeImgs_ = {
		{CardBase::CARD_TYPE::ATTACK,atkCardImg_},
		{CardBase::CARD_TYPE::RELOAD,reloadCardImg_}
	};
}

void CardUIBase::DecisionMoveCardAll(void)
{
	//disitionCnt_ -= SceneManager::GetInstance().GetDeltaTime();
	//選択したカードの情報を取得
	for (auto& card : actions_)
	{
		if (card->GetState() == CardUIController::CARD_STATE::REACT)continue;
		//card->ChangeUsing();

		card->DecisionMove();
		//DecisionMoveSpecificCard(card);
	}
}



void CardUIBase::UpdateUsedCard(void)
{
	if (actions_.empty())return;
	for (auto& act : actions_)
	{
		act->EraseUsedCard();
	}

	//消去アニメーションが終わったカードはアクション配列から削除
	std::erase_if(actions_, [](auto& act) {return act->GetSclCnt() < 0.0f; });
}

void CardUIBase::ReactMoveCard(const Vector2F& _goalPos)
{
	//選択したカードの情報を取得
	for (auto& card : actions_)
	{
		card->ReactUpdate(_goalPos);
	}
	
}

void CardUIBase::ReactMoveSpecificCard(CARD_UI_INFO& _card, const Vector2F& _goalPos)
{
	//弾かれ移動
	_card.cardPos_ = UtilityCommon::Lerp(_card.cardPos_, _goalPos,
		(CardUIController::REACT_MOVE_CARD_TIME - _card.reactCnt_) / CardUIController::REACT_MOVE_CARD_TIME);
	_card.reactCnt_ -= DELTA;
}

void CardUIBase::AddHandCurrent(void)
{
	handCurrent_++;
	if (handCurrent_ == handCards_.end())
	{
		handCurrent_ = handCards_.begin();
	}
}

void CardUIBase::SubHandCurrent(void)
{
	if (handCurrent_ == handCards_.begin())
	{
		handCurrent_ = handCards_.end();
	}
	handCurrent_--;
}



const int CardUIBase::GetTypeImg(const CardBase::CARD_STATUS _status)
{
	int typeImg = -1;
	switch (_status.type_)
	{
	case CardBase::CARD_TYPE::ATTACK:
		typeImg = atkCardImg_;
		break;
	case CardBase::CARD_TYPE::RELOAD:
		typeImg = reloadCardImg_;
	}

	return typeImg;
}



