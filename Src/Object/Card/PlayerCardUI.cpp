#include <DxLib.h>
#include <cmath>
#include <ranges>
#include "../Utility/UtilityCommon.h"
#include"../Manager/Generic/InputManager.h"
#include"../Manager/Generic/SceneManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "PlayerCardUI.h"

PlayerCardUI::PlayerCardUI(void):
radius_({RADIUS_X,RADIUS_Y}),
cardMoveCnt_(SELECT_MOVE_CARD_TIME),
numPos_({0.0f,0.0f}),
centerPos_({0,0}),
isReloadEnd_(false)

{
	int i = -1;
	//複数画像はコンストラクタで初期化必須
	cardNoImgs_ = &i;


}

PlayerCardUI::~PlayerCardUI(void)
{
	handCards_.clear();
	visibleCards_.clear();
	//アクション中カード
	actions_.clear();
	changeMoveState_.clear();
	uiInfos_.clear();
}

void PlayerCardUI::Load(void)
{
	ResourceManager& res = ResourceManager::GetInstance();
	cardNoImgs_ = res.Load(ResourceManager::SRC::NUMBERS_IMG).handleIds_;
	atkCardImg_ = res.Load(ResourceManager::SRC::ATK_CARD_IMG).handleId_;
	reloadCardImg_ = res.Load(ResourceManager::SRC::RELOAD_CARD_IMG).handleId_;
	SoundManager::GetInstance().LoadResource(SoundManager::SRC::CARD_MOVE);

	SoundManager::GetInstance().LoadResource(SoundManager::SRC::GAME_BGM);
	SoundManager::GetInstance().Play(SoundManager::SRC::GAME_BGM, SoundManager::PLAYTYPE::LOOP);
}
void PlayerCardUI::Init(void)
{
	changeMoveState_ = {
		{CARD_SELECT::NONE, [this]() {ChangeNone(); } },
		{CARD_SELECT::LEFT, [this]() {ChangeLeft(); } },
		{CARD_SELECT::RIGHT, [this]() {ChangeRight(); } },
		{CARD_SELECT::DISITION, [this]() {ChangeDisition(); } },
		{CARD_SELECT::RELOAD_WAIT, [this]() {ChangeReloadWait(); } },
		{CARD_SELECT::RELOAD, [this]() {ChangeReload(); } }
	};

	ChangeSelectState(CARD_SELECT::NONE);
	
	InitCardUI();

}

void PlayerCardUI::Update(void)
{
	//カード状態
	cardUpdate_();
	
	//カード番号座標の追従
	UpdateCardNumPost();
	//使用済みカードの大きさ補完
	UpdateUsedCard();

	//弾かれるカードの大きさ補完
	ReactMoveCard(REACT_GOAL_CARD_POS);
}

void PlayerCardUI::Draw(void)
{
	//逆順で描画
	for (auto& card : visibleCards_ | std::ranges::views::reverse)
	{
		DrawCard(card);
	}
	//現在選択中のカードを強調表示
	if (visibleCurrent_ != visibleCards_.end())
	{
		DrawCard(*visibleCurrent_);
	}


	for (auto& card : actions_)
	{
		DrawCard(card);
	}

#ifdef _DEBUG
	DrawDebug();
#endif // _DEBUG

	
}





#ifdef _DEBUG
void PlayerCardUI::DrawDebug(void)
{
	//DrawFormatString(10, 50, 0xffffff, L"pow : %d", handCurrent_->status.pow_);
}
#endif 
void PlayerCardUI::InitCardUI(void)
{
	handCards_.clear();
	visibleCards_.clear();
	//手札にすべての初期札を入れる
	for (auto& it : uiInfos_)
	{
		handCards_.emplace_back(it);
	}
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
		it->currentAngle = ARROUND_PER_RAD * i - ARROUND_PER_RAD;
		it->cardPos.x = CENTER_X + std::sin(it->currentAngle) * radius_.x;
		it->cardPos.y = CENTER_Y - std::cos(it->currentAngle) * radius_.y;
		//常に強さ番号座標をローカル座標分追従させる
		it->numPos = it->cardPos + (NUM_LOCAL_POS * it->cardScl);
		//見せるカード配列に入れる
		visibleCards_.emplace_back(*it);
		i++;
	}
	//見せるカードの現在位置イテレータを初期化
	if (!visibleCards_.empty())
	{
		visibleCurrent_ = visibleCards_.begin();
		visibleCurrent_++;
	}
	if (!handCards_.empty())
	{
		handCurrent_ = handCards_.begin();
	}
}
// _DEBUG
void PlayerCardUI::ChangeNone(void)
{
	cardMoveCnt_ = SELECT_MOVE_CARD_TIME;
	//目標角度を現在の角度にする
	for (auto& card : visibleCards_)
	{
		card.goalAngle = 0.0f;
	}
	cardUpdate_ = [this]() {UpdateNone(); };
}

void PlayerCardUI::ChangeLeft(void)
{
	cardMoveCnt_ = SELECT_MOVE_CARD_TIME;

	//カードの範囲変数を更新する
	if (visibleCards_.size()==1)
	{
		ChangeSelectState(CARD_SELECT::NONE);
		return;
	}
	else if(std::next(visibleCurrent_)==visibleCards_.end())
	{
		ChangeSelectState(CARD_SELECT::RIGHT);
		return;
	}
	else
	{
		visibleCurrent_++;
	}

	//先頭に追加
	auto it = handCurrent_;
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
	it->currentAngle = ARROUND_PER_RAD * (size - CARDS_BEFORE_CURRENT);

	visibleCards_.emplace_back(*it);

	//手札選択カードを更新
	AddHandCurrent();

	//目標角度を代入
	auto goalRadit = goalLeftRad_.begin();
	for (auto& card : visibleCards_)
	{
		card.goalAngle = card.currentAngle - ARROUND_PER_RAD;
		//goalRadit++;
	}
	//サウンドを再生
	SoundManager::GetInstance().Play(SoundManager::SRC::CARD_MOVE, SoundManager::PLAYTYPE::BACK);

	cardUpdate_ = [this]() {UpdateLeft(); };
}

void PlayerCardUI::ChangeRight(void)
{
	cardMoveCnt_ = SELECT_MOVE_CARD_TIME;

	//visible配列に入れる前に現在の番地を引くことで、
	//結果的に1番目が選択されていることになる
	if(visibleCards_.size()==1)
	{
		ChangeSelectState(CARD_SELECT::NONE);
		return;
	}
	else
	{
		visibleCurrent_--;
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
	
	it->currentAngle = -ARROUND_PER_RAD * PREV_CARD_COUNT;
	visibleCards_.emplace_front(*it);

	//手札選択カードを更新
	SubHandCurrent();


	//目標角度をずらす
	for (auto& card : visibleCards_)
	{
		card.goalAngle = card.currentAngle + ARROUND_PER_RAD;
	}
	//サウンドを再生
	SoundManager::GetInstance().Play(SoundManager::SRC::CARD_MOVE, SoundManager::PLAYTYPE::BACK);
	
	cardUpdate_ = [this]() {UpdateRight(); };
}

void PlayerCardUI::ChangeDisition(void)
{
	if (visibleCurrent_->status.type_ == CardBase::CARD_TYPE::RELOAD)
	{
		ChangeSelectState(CARD_SELECT::RELOAD_WAIT);
		return;
	}
	//disitionCnt_ = DISITION_MOVE_CARD_TIME;

	//使用中カード配列に入れる
	actions_.emplace_back(*visibleCurrent_);

	//決定カウントをセット
	for(CARD_UI_INFO& act:actions_)
	{
		act.disitionCnt_ = DISITION_MOVE_CARD_TIME;
	}
	
	//手札に6枚よりカードが多かったら配列に入れる
	UpdateVisibleCard();
	//手札から使用するカードを消去
	EraseHandCard();
	//カードの範囲変数を更新する
	DesideGoalAngle();
	//見せるカードの更新
	UpdateVisibleCurrent();


	cardUpdate_ = [this]() {UpdateDisition(); };
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
	for (auto& it : uiInfos_)
	{
		handCards_.emplace_back(it);
	}
	isReloadEnd_ = false;
	//一番最後の配列を見る
	reloadAnimCurr_ = std::prev(handCards_.end());
	visibleCurrent_ = visibleCards_.end();
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
		CurrentAngle();
		ChangeSelectState(CARD_SELECT::NONE);
		return;
	}
	MoveCardAll();
}

void PlayerCardUI::UpdateRight(void)
{
	cardMoveCnt_ -= DELTA;
	if (cardMoveCnt_ < 0.0f)
	{
		visibleCards_.pop_back();
		CurrentAngle();
		ChangeSelectState(CARD_SELECT::NONE);
		return;
	}
	MoveCardAll();
}

void PlayerCardUI::UpdateDisition(void)
{
	DisitionMoveCardAll();

	//cardMoveCnt_-= SceneManager::GetInstance().GetDeltaTime();
	cardMoveCnt_-= DELTA;
	for (auto it = visibleCurrent_; it != visibleCards_.end(); it++)
	{
		MoveSpecificCard(*it);
	}
	auto it = std::find_if(actions_.begin(), actions_.end(), [this](const auto& act) {return act.disitionCnt_ < 0.0f; });
	if(it!=actions_.end())
	{
		ChangeSelectState(CARD_SELECT::NONE);
	}
}

void PlayerCardUI::UpdateReloadWait(void)
{
	if(reloadPer_>=1.0f)
	{
		//InitCardUI();
		ChangeSelectState(CARD_SELECT::RELOAD);
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
			MoveCardAll();
		}
		else
		{
			if (!visibleCards_.empty())
			{
				visibleCurrent_ = visibleCards_.begin();
				visibleCurrent_++;
			}
			if (!handCards_.empty())
			{
				handCurrent_ = handCards_.begin();
			}
			ChangeSelectState(CARD_SELECT::NONE);
		}
		
	}
}

void PlayerCardUI::MoveCardAll(void)
{
	for (auto& card : visibleCards_)
	{
		MoveSpecificCard(card);
	}
}

void PlayerCardUI::MoveSpecificCard(CARD_UI_INFO& _card)
{
	float time = (SELECT_MOVE_CARD_TIME - cardMoveCnt_) / SELECT_MOVE_CARD_TIME;
	float startRad = _card.currentAngle;
	float goalRad = _card.goalAngle;
	_card.currentAngle = UtilityCommon::LerpRad(startRad
		, goalRad, time);

	_card.cardPos.x = CENTER_X + std::sin(_card.currentAngle) * radius_.x;
	_card.cardPos.y = CENTER_Y - std::cos(_card.currentAngle) * radius_.y;
}



void PlayerCardUI::CurrentAngle(void)
{
	for (auto& card : visibleCards_)
	{
		card.goalAngle = card.currentAngle;
	}
}

void PlayerCardUI::UpdateVisibleCurrent(void)
{
	//
	int size = static_cast<int>(visibleCards_.size());
	auto next = std::next(visibleCurrent_);
	if(next!=visibleCards_.end())
	{
		//現在選択中を更新
		visibleCurrent_++;
		//更新前のカード(更新後のひとつ前)を見せるカードから消す
		auto prev = std::prev(visibleCurrent_);
		visibleCards_.erase(prev);
	}
	else
	{
		//現在選択中を更新
		visibleCurrent_--;
		//更新前のカード(更新後のひとつ前)を見せるカードから消す
		auto next = std::next(visibleCurrent_);
		visibleCards_.erase(next);
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
		auto visibleIt = visibleCurrent_;

		//表示カードの次の配列になるまで手札のイテレータを回す
		for (; visibleIt != visibleCards_.end(); visibleIt++)
		{
			endit++;
			if (endit == handCards_.end())
			{
				endit = handCards_.begin();
			}
		}
		
		endit->currentAngle = ARROUND_PER_QUAD_RAD + ARROUND_PER_RAD;
		visibleCards_.emplace_back(*endit);
	}
	//2枚の時は前にあるカードを持ってくる
	else if (size <= PREV_CARD_COUNT)
	{
		visibleCurrent_->goalAngle = visibleCurrent_->currentAngle + ARROUND_PER_RAD;
	}
}

void PlayerCardUI::UpdateCardNumPost(void)
{
	//常に強さ番号座標をローカル座標分追従させる
	for (auto& card : visibleCards_)
	{
		card.numPos = card.cardPos + (NUM_LOCAL_POS * card.cardScl);
	}
	for (auto& card : actions_)
	{
		card.numPos = card.cardPos + (NUM_LOCAL_POS * card.cardScl);
	}
}

void PlayerCardUI::EraseHandCard(void)
{
	auto next = std::next(handCurrent_);
	auto visibleNext = std::next(visibleCurrent_);
	if (visibleNext != visibleCards_.end())
	{
		AddHandCurrent();
		//現在選択の手札が手札配列の最初だった場合、
		//手札の一番最後の配列に戻る
		if (handCurrent_ == handCards_.begin())
		{
			auto prev = std::prev(handCards_.end());
			handCards_.erase(prev);
		}
		else
		{
			auto prev = std::prev(handCurrent_);
			handCards_.erase(prev);
		}
	}
	else
	{
		SubHandCurrent();
		auto next = std::next(handCurrent_);
		handCards_.erase(next);
	}
}

void PlayerCardUI::DesideGoalAngle(void)
{
	//カードの範囲変数を更新する
	auto visibleIt = std::next(visibleCurrent_);
	auto goalDegIt = goalLeftRad_.begin();
	for (; visibleIt != visibleCards_.end(); visibleIt++)
	{
		visibleIt->goalAngle += visibleIt->currentAngle - ARROUND_PER_RAD;
	}
}

void PlayerCardUI::ReloadAnimation(void)
{
	//リロード終了時は処理しない
	if (isReloadEnd_)return;

	//見せカードの0番目にリロードカードが来たら終了
	//endItをbeginの５個先(６枚目)に指定する
	//６枚目までイテレータを回す
	cardMoveCnt_ -= DELTA;
	//定期的に見せカード配列に格納する
	if (cardMoveCnt_ < 0.0f)
	{
		reloadAnimCurr_->currentAngle = static_cast<float>(-ARROUND_PER_RAD * PREV_CARD_COUNT);

		//見せるカード配列に追加
		//サウンドを再生
		SoundManager::GetInstance().Play(SoundManager::SRC::CARD_MOVE, SoundManager::PLAYTYPE::BACK);
		visibleCards_.emplace_front(*reloadAnimCurr_);
		if(visibleCards_.size()>VISIBLE_CARD_MAX&& reloadAnimCurr_==std::prev(handCards_.end()))
		{
			isReloadEnd_ = true;
		}


		cardMoveCnt_ = RELOAD_MOVE_CARD_TIME_PER;
		reloadAnimCurr_--;
	}



	//見せカードの移動
	//座標を指定する時に使用する
	int i = 0;
	for (auto& card : visibleCards_)
	{
		card.goalAngle = ARROUND_PER_RAD * (i - CARDS_BEFORE_CURRENT);
		MoveSpecificCard(card);
		i++;
	}
	//見せカードが7枚以上お時はポップ
	if(visibleCards_.size()>VISIBLE_CARD_MAX)
	{
		visibleCards_.pop_back();
	}
	if(reloadAnimCurr_==handCards_.begin())
	{
		reloadAnimCurr_ = std::prev(handCards_.end());
	}
}

