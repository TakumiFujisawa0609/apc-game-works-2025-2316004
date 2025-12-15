#include <DxLib.h>
#include <cmath>
#include <ranges>
#include "../Utility/UtilityCommon.h"
#include"../Manager/Generic/InputManager.h"
#include"../Manager/Generic/SceneManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"

#include "../Card/CardUIController.h"
#include "../Card/CardSystem.h"
#include "PlayerCardUI.h"

PlayerCardUI::PlayerCardUI(void):
radius_({RADIUS_X,RADIUS_Y}),
cardMoveCnt_(CardUIController::SELECT_MOVE_CARD_TIME),
numPos_({0.0f,0.0f}),
centerPos_({0,0}),
isReloadEnd_(false)
{
	int i = -1;
	//複数画像はコンストラクタで初期化必須
	cardNoImg_ = &i;
}

PlayerCardUI::~PlayerCardUI(void)
{
	handCards_.clear();
	visibleCards_.clear();
	//アクション中カード
	actions_.clear();
	changeMoveState_.clear();
	initialCards_.clear();
}

void PlayerCardUI::Load(void)
{
	ResourceManager& res = ResourceManager::GetInstance();
	cardNoImg_ = res.Load(ResourceManager::SRC::NUMBERS_IMG).handleIds_;
	atkCardImg_ = res.Load(ResourceManager::SRC::PLAYER_ATK_CARD_IMG).handleId_;
	reloadCardImg_ = res.Load(ResourceManager::SRC::RELOAD_CARD_IMG).handleId_;
	reloadGage_ = res.Load(ResourceManager::SRC::RELOAD_GAGE).handleId_;
	reloadFrame_ = res.Load(ResourceManager::SRC::RELOAD_FRAME).handleId_;
	selectFrameImg_ = res.Load(ResourceManager::SRC::CARD_SELECT_FRAME_IMG).handleId_;

	reloadCardFrameImg_=res.Load(ResourceManager::SRC::RELOAD_FRAME).handleId_;
	SoundManager::GetInstance().LoadResource(SoundManager::SRC::CARD_MOVE,500.0f);
	SoundManager::GetInstance().LoadResource(SoundManager::SRC::CARD_BE_REFLECTED);
	cardWinRes_ = SoundManager::SRC::CARD_BE_REFLECTED;

}
void PlayerCardUI::Init(void)
{

	changeMoveState_ = {
		{CARD_SELECT::NONE, [this]() {ChangeNone(); } },
		{CARD_SELECT::LEFT, [this]() {ChangeLeft(); } },
		{CARD_SELECT::RIGHT, [this]() {ChangeRight(); } },
		{CARD_SELECT::DISITION, [this]() {ChangeDecision(); } },
		{CARD_SELECT::RELOAD_WAIT, [this]() {ChangeReloadWait(); } },
		{CARD_SELECT::RELOAD, [this]() {ChangeReload(); } }
	};
	InitCardUI();
	ChangeSelectState(CARD_SELECT::NONE);
	SetBasePosVisibleCards();

}

void PlayerCardUI::Update(void)
{
	CardUIBase::Update();

	MoveUpDownVisibleCards();

	//弾かれるカードの大きさ補完
	ReactMoveCard(REACT_GOAL_CARD_POS);
}

void PlayerCardUI::Draw(void)
{
	//逆順で描画
	for (auto& card : visibleCards_ | std::ranges::views::reverse)
	{
		//DrawCard(card);
		card->Draw();
		if (card->GetStatus().type_ == CardBase::CARD_TYPE::RELOAD)
		{
			card->DrawReloadGauge(reloadCardFrameImg_,reloadPer_);
		}
	}

	if (handCurrent_ != handCards_.end())
	{
		(*handCurrent_)->SelectCardDrawFrame(selectFrameImg_);
		if ((*handCurrent_)->GetStatus().type_ == CardBase::CARD_TYPE::RELOAD)
		{
			(*handCurrent_)->DrawReloadGauge(reloadCardFrameImg_,reloadPer_);
		}

	}

	//カード描画(共通)
	CardUIBase::Draw();

#ifdef _DEBUG
	//DrawDebug();
#endif // _DEBUG

}

#ifdef _DEBUG
void PlayerCardUI::DrawDebug(void)
{
	int i = 0;
	for(auto& action:actions_)
	{
		std::wstring stateStr;
		auto state = action->GetState();
		switch (state)
		{
		case CardUIController::CARD_STATE::DRAW_PILE:
			stateStr = L"DRAW_PILE";
			break;
		case CardUIController::CARD_STATE::MOVE_DRAW:
			stateStr = L"MOVE_DRAW";
			break;
		case CardUIController::CARD_STATE::USING:
			stateStr = L"USING";
			break;
		case CardUIController::CARD_STATE::REACT:
			stateStr = L"REACT";
			break;
		case CardUIController::CARD_STATE::USED:
			stateStr = L"USED";
			break;
		default:
			break;
		}
		DrawFormatString(10, 10 + i * 20, 0xffffff, L"react(%f),Dicision(%f),state(%s)", action->GetReactCount(),action->GetDecisionCnt(), stateStr.c_str());
		i++;
	}
	DrawFormatString(10, 300, 0xffffff, L"select(%d)", selectState_);
}
#endif 
void PlayerCardUI::InitCardUI(void)
{
	handCards_.clear();
	visibleCards_.clear();
	actions_.clear();
	//手札にすべての初期札を入れる
	for (auto& it : initialCards_)
	{
		(*it).Load();
		(*it).Init();
		handCards_.emplace_back(it);
	}
	//はじめの配列にリロードカードを描画したいので、最後の配列にセットする
	auto beginit = std::prev(handCards_.end());
	auto endIt = handCards_.begin();
	//endItをbeginの５個先(６枚目)に指定する
	std::advance(endIt, VISIBLE_CARD_MAX - 1);

	//座標を指定する時に使用する
	int i = 0;
	//６枚目までイテレータを回す
	for (auto& it = beginit; it != endIt; it++)
	{
		//手札イテレータが終端なら先頭に戻す
		if(it==handCards_.end())
		{
			it = handCards_.begin();
		}

		(*it)->InitCard(i);
		//見せるカード配列に入れる
		visibleCards_.emplace_back(*it);
		float& scl = (*it)->GetScl();
		i++;
	}

	if (!handCards_.empty())
	{
		handCurrent_ = handCards_.begin();
	}
}
// _DEBUG
void PlayerCardUI::ChangeNone(void)
{
	cardMoveCnt_ = CardUIController::SELECT_MOVE_CARD_TIME;
	//目標角度を現在の角度にする
	for (auto& card : visibleCards_)
	{
		//card->SyncCardAngleAndPos();
		card->SetStartAndGoalAngle(0.0f);
	}
	cardUpdate_ = [this]() {UpdateNone(); };
}

void PlayerCardUI::ChangeLeft(void)
{

	// 時計回りにカードを回す
	// 対象は、手札UIの全てのカード
	cardMoveCnt_ = CardUIController::SELECT_MOVE_CARD_TIME;

	//カードの範囲変数を更新する
	// 手札はリロードカードが必ず残るので、最小枚数は1枚(0はあり得ないようにする)
	if (visibleCards_.size() == 1)
	{
		ChangeSelectState(CARD_SELECT::NONE);
		return;
	}

	//先頭に追加
	auto it = handCurrent_; // 手札の現在選択カード(handCards_のイテレータ
	const int rupeNum = static_cast<int>(visibleCards_.size()) - 1;
	for (int i = 0; i < rupeNum;i++)
	{
		it++;
		if (it == handCards_.end())
		{
			it = handCards_.begin();
		}
	}
	//次の角度を現在角度に代入
	//見せるカードのマックス分角度をかける
	int size = static_cast<int>(visibleCards_.size());
	//it->currentAngle_ = ARROUND_PER_RAD * (size - CARDS_BEFORE_CURRENT);
	(*it)->SetCurrentAngle(ARROUND_PER_RAD * (size - CARDS_BEFORE_CURRENT));
	visibleCards_.emplace_back(*it);

	//手札選択カードを更新
	AddHandCurrent();

	//目標角度を代入
	auto goalRadit = goalLeftRad_.begin();
	for (auto& card : visibleCards_)
	{
		//card.goalAngle_ = card.currentAngle_ - ARROUND_PER_RAD;
		float currentAngle = card->GetCurrentAngle();
		card->SetStartAndGoalAngle(currentAngle - ARROUND_PER_RAD);
	}
	//サウンドを再生
	SoundManager::GetInstance().Play(SoundManager::SRC::CARD_MOVE, SoundManager::PLAYTYPE::BACK);

	cardUpdate_ = [this]() {UpdateLeft(); };
}

void PlayerCardUI::ChangeRight(void)
{
	cardMoveCnt_ = CardUIController::SELECT_MOVE_CARD_TIME;

	//visible配列に入れる前に現在の番地を引くことで、
	//結果的に1番目が選択されていることになる
	if(visibleCards_.size()==1)
	{
		ChangeSelectState(CARD_SELECT::NONE);
		return;
	}
	
	//先頭に追加
	auto it = handCurrent_;
	//現在位置より２枚遡って配列に入れる
	for (int i = 0; i < PREV_CARD_COUNT; i++)
	{
		if (it == handCards_.begin())
		{
			it = handCards_.end();
		}
		it--;
	}
	
	//it->currentAngle_ = -ARROUND_PER_RAD * PREV_CARD_COUNT;
	(*it)->SetCurrentAngle(-ARROUND_PER_RAD * PREV_CARD_COUNT);
	visibleCards_.emplace_front(*it);
	//手札選択カードを更新
	SubHandCurrent();

	//目標角度をずらす
	for (auto& card : visibleCards_)
	{
		//card.goalAngle_ = card.currentAngle_ + ARROUND_PER_RAD;
		float currentAngle = card->GetCurrentAngle();
		card->SetStartAndGoalAngle(currentAngle + ARROUND_PER_RAD);
	}
	//サウンドを再生
	SoundManager::GetInstance().Play(SoundManager::SRC::CARD_MOVE, SoundManager::PLAYTYPE::BACK);
	
	cardUpdate_ = [this]() {UpdateRight(); };
}

void PlayerCardUI::ChangeDecision(void)
{
	// カードを使う処理

	if (selectState_ == CARD_SELECT::NONE&&(*handCurrent_)->GetStatus().type_ == CardBase::CARD_TYPE::RELOAD)
	{
		ChangeSelectState(CARD_SELECT::RELOAD_WAIT);
		return;
	}
	//disitionCnt_ = DISITION_MOVE_CARD_TIME;


	actions_.emplace_back(*handCurrent_);

	//決定カウントをセット
	for(auto& act:actions_)
	{
		if (act->GetState() == CardUIController::CARD_STATE::REACT)continue;
		act->ChangeUsing();
		act->SetDecisionCount(CardUIController::DISITION_MOVE_CARD_TIME);
	}
	
	//手札に6枚よりカードが多かったら配列に入れる
	UpdateVisibleCard();
	//手札から使用するカードを消去
	EraseHandCard();
	//カードの範囲変数を更新する
	DesideGoalAngle();

	cardUpdate_ = [this]() {UpdateDecision(); };
}

void PlayerCardUI::ChangeReloadWait(void)
{
	cardMoveCnt_ = 0.0f;
	cardUpdate_ = [this]() {UpdateReloadWait(); };
}
void PlayerCardUI::ChangeReload(void)
{
	handCards_.clear();
	visibleCards_.clear();
	//手札にすべての初期札を入れる
	for (auto& it : initialCards_)
	{
		it->ResetCount();
		handCards_.emplace_back(it);
	}
	isReloadEnd_ = false;
	//一番最後の配列を見る
	reloadAnimCurr_ = std::prev(handCards_.end());
	//visibleCurrent_ = visibleCards_.end();
	handCurrent_ = handCards_.end();
	cardUpdate_ = [this]() {UpdateReload(); };
}
void PlayerCardUI::UpdateNone(void)
{

}

void PlayerCardUI::UpdateLeft(void)
{
	cardMoveCnt_ -= DELTA;
	if (cardMoveCnt_ < 0.0f)
	{
		visibleCards_.pop_front();
		SetBasePosVisibleCards();
		ChangeSelectState(CARD_SELECT::NONE);
		return;
	}
	MoveCardAll(CardUIController::SELECT_MOVE_CARD_TIME);
}

void PlayerCardUI::UpdateRight(void)
{
	cardMoveCnt_ -= DELTA;
	if (cardMoveCnt_ < 0.0f)
	{
		visibleCards_.pop_back();
		SetBasePosVisibleCards();
		ChangeSelectState(CARD_SELECT::NONE);
		return;
	}
	MoveCardAll(CardUIController::SELECT_MOVE_CARD_TIME);
}

void PlayerCardUI::UpdateDecision(void)
{
	
	DecisionMoveCardAll();

	//cardMoveCnt_-= SceneManager::GetInstance().GetDeltaTime();

	std::list<std::shared_ptr<CardUIController>>::iterator visibleCurrent = GetVisibleCurrentIt();
	//for (auto it = visibleCurrent_; it != visibleCards_.end(); it++)
	for (auto it = visibleCurrent; it != visibleCards_.end(); it++)
	{
		(*it)->MoveOnRevolver(cardMoveCnt_,CardUIController::DISITION_MOVE_CARD_TIME);
	}
	cardMoveCnt_ -= DELTA;
	//決定移動が終わったらnone状態に戻す
	auto it = std::find_if(actions_.begin(), actions_.end(), [this](auto& act) {return act->GetDecisionCnt() > 0.0f; });

	if(it==actions_.end())
	{
		SetBasePosVisibleCards();
		SetBasePosActionCards();
		ChangeSelectState(CARD_SELECT::NONE);
	}

}

void PlayerCardUI::UpdateReloadWait(void)
{
	if(reloadPer_>= UtilityCommon::PERCENT_MAX)
	{
		//一瞬none状態にする
		ChangeSelectState(CARD_SELECT::NONE);
		//InitCardUI();
		ChangeSelectState(CARD_SELECT::RELOAD);

		reloadPer_ = 0.0f;
		return;
	}
}
void PlayerCardUI::UpdateReload(void)
{
	ReloadAnimation();
	if (isReloadEnd_)
	{
		//見せるカードの現在位置イテレータを初期化
		if (cardMoveCnt_ >= 0.0f)
		{
			cardMoveCnt_ -= DELTA;
			MoveCardAll(CardUIController::RELOAD_MOVE_CARD_TIME_PER);
		}
		else
		{
			if (!handCards_.empty())
			{
				handCurrent_ = handCards_.begin();
				//handCurrent_++;
			}
			SetBasePosVisibleCards();
			ChangeSelectState(CARD_SELECT::NONE);
		}
	}
}

void PlayerCardUI::MoveCardAll(const float& _moveTImeMax)
{
	for (auto& card : visibleCards_)
	{
		//MoveSpecificCard(card);
		card->MoveOnRevolver(cardMoveCnt_, _moveTImeMax);
	}
}

void PlayerCardUI::UpdateDrawCardUI(void)
{
	//見えている部分だけ更新
	for (auto& card : visibleCards_)
	{
		//DrawCard(card);
		card->Update();
	}
}


void PlayerCardUI::SetBasePosVisibleCards(void)
{
	for (auto& card : visibleCards_)
	{
		card->SetBaseCardPos();
		card->SetStartAngle();
	}
}


void PlayerCardUI::UpdateVisibleCard(void)
{
	const int size = static_cast<int>(handCards_.size());
	//手札に6枚よりカードが多かったら配列に入れる
	if (size > VISIBLE_CARD_MAX)
	{
		//先頭に追加
		auto endit = handCurrent_;
		//auto visibleIt = visibleCurrent_;
		auto visibleIt = GetVisibleCurrentIt();

		//表示カードの次の配列になるまで手札のイテレータを回す
		for (; visibleIt != visibleCards_.end(); visibleIt++)
		{
			endit++;
			if (endit == handCards_.end())
			{
				endit = handCards_.begin();
			}
		}
		
		//(*endit)->currentAngle_ = ARROUND_PER_QUAD_RAD + ARROUND_PER_RAD;
		(*endit)->SetCurrentAngle(ARROUND_PER_QUAD_RAD + ARROUND_PER_RAD);
		visibleCards_.emplace_back(*endit);
	}
}


void PlayerCardUI::EraseHandCard(void)
{
	std::list<std::shared_ptr<CardUIController>>::iterator eraseHandIt = handCurrent_;
	std::list<std::shared_ptr<CardUIController>>::iterator eraseVisibleIt = GetVisibleCurrentIt();
	std::list<std::shared_ptr<CardUIController>>::iterator visibleCurrentIt = eraseVisibleIt;

	// 使用した手札を削除するので、新しい手札カレントを設定する処理
	// 見えている手札UIカード上の右側(時計回りの次)を手札とする
	// ただし、見えている手札UIカードが最後尾の場合は左側(反時計回りの次)を手札とする
	if (std::next(visibleCurrentIt) == visibleCards_.end())
	{
		visibleCurrentIt--;
	}
	else
	{
		visibleCurrentIt++;
	}

	// UIカードを元に、手札からイテレータを見つけ出す
	handCurrent_ = GetSearchHandIt(*visibleCurrentIt);

	// 手札削除
	handCards_.erase(eraseHandIt);
	visibleCards_.erase(eraseVisibleIt);

}

void PlayerCardUI::DesideGoalAngle(void)
{
	//カードの範囲変数を更新する
	//auto visibleIt = std::next(visibleCurrent_);
	auto visibleIt = std::next(GetVisibleCurrentIt());
	auto goalDegIt = goalLeftRad_.begin();
	for (; visibleIt != visibleCards_.end(); visibleIt++)
	{
		float currentAngle = (*visibleIt)->GetCurrentAngle();
		(*visibleIt)->SetStartAndGoalAngle(currentAngle - ARROUND_PER_RAD);
	}
}

void PlayerCardUI::ReloadAnimation(void)
{
	//リロード終了時は処理しない
	if (isReloadEnd_)
	{
		return;
	}
		

	//見せカードの0番目にリロードカードが来たら終了
	//endItをbeginの５個先(６枚目)に指定する
	//６枚目までイテレータを回す
	cardMoveCnt_ -= DELTA;
	//定期的に見せカード配列に格納する
	if (cardMoveCnt_ < 0.0f)
	{
		//reloadAnimCurr_->currentAngle_ = static_cast<float>(-ARROUND_PER_RAD * PREV_CARD_COUNT);
		(*reloadAnimCurr_)->SetCurrentAngle(static_cast<float>(-ARROUND_PER_RAD * PREV_CARD_COUNT));

		//見せるカード配列に追加
		//サウンドを再生
		SoundManager::GetInstance().Play(SoundManager::SRC::CARD_MOVE, SoundManager::PLAYTYPE::BACK);
		visibleCards_.emplace_front(*reloadAnimCurr_);
		if(visibleCards_.size()>VISIBLE_CARD_MAX&& reloadAnimCurr_==std::prev(handCards_.end()))
		{
			isReloadEnd_ = true;
		}
		cardMoveCnt_ = CardUIController::RELOAD_MOVE_CARD_TIME_PER;
		reloadAnimCurr_--;
	}

	//見せカードの移動
	//座標を指定する時に使用する
	int i = 0;
	for (auto& card : visibleCards_)
	{
		//MoveSpecificCard(card);

		card->SetStartAndGoalAngle(ARROUND_PER_RAD * (i - CARDS_BEFORE_CURRENT));
		card->MoveOnRevolver(cardMoveCnt_, CardUIController::RELOAD_MOVE_CARD_TIME_PER);
		i++;
	}
	//見せカードが7枚以上お時はポップ
	if(visibleCards_.size()>VISIBLE_CARD_MAX)
	{
		visibleCards_.pop_back();
		//visibleDrawCard_.pop_back();
	}
	if(reloadAnimCurr_==handCards_.begin())
	{
		(*reloadAnimCurr_)->SetCurrentAngle(static_cast<float>(-ARROUND_PER_RAD * PREV_CARD_COUNT));
		visibleCards_.emplace_front(*reloadAnimCurr_);
		reloadAnimCurr_ = std::prev(handCards_.end());
	}
}

void PlayerCardUI::MoveUpDownVisibleCards(void)
{
	//カードが動いていないときに上下に動かす
	if (selectState_ == CARD_SELECT::NONE || selectState_ == CARD_SELECT::RELOAD_WAIT)
	{
		for (auto& visible : visibleCards_)
		{
			visible->MoveUpDown();
		}
	}
}

std::list<std::shared_ptr<CardUIController>>::iterator PlayerCardUI::GetVisibleCurrentIt(void)
{
	for (auto it = visibleCards_.begin(); it != visibleCards_.end(); it++)
	{
		if (*it == *handCurrent_)
		{
			return it;
		}
	}
	return visibleCards_.end();
}

std::list<std::shared_ptr<CardUIController>>::iterator PlayerCardUI::GetSearchHandIt(
	std::shared_ptr<CardUIController> target)
{

	for (auto it = handCards_.begin(); it != handCards_.end(); it++)
	{
		if (*it == target)
		{
			return it;
		}
	}
	return handCards_.end();
}

