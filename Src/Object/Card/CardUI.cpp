#include <DxLib.h>
#include <cmath>
#include "../Utility/UtilityCommon.h"
#include"../Manager/Generic/InputManager.h"
#include"../Manager/Generic/SceneManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "CardUI.h"

CardUI::CardUI(void):
atkCardImg_(-1),
cardPos_({ 100,100 }),
cardScl_(1.0),
radius_({RADIUS_X,RADIUS_Y}),
cardMoveCnt_(SELECT_MOVE_CARD_TIME)
{
	int i = -1;
	//複数画像はコンストラクタで初期化必須
	cardNoImgs_ = &i;


}

CardUI::~CardUI(void)
{
	handCards_.clear();
	visibleCards_.clear();
	//アクション中カード
	actions_.clear();
}

void CardUI::Load(void)
{
	ResourceManager& res = ResourceManager::GetInstance();
	cardNoImgs_ = res.Load(ResourceManager::SRC::NUMBERS_IMG).handleIds_;
	atkCardImg_ = res.Load(ResourceManager::SRC::ATK_CARD_IMG).handleId_;
	reloadCardImg_ = res.Load(ResourceManager::SRC::RELOAD_CARD_IMG).handleId_;
	SoundManager::GetInstance().LoadResource(SoundManager::SRC::CARD_MOVE);
}

void CardUI::Init(void)
{
	int visible = 6;
	float angleOff = UtilityCommon::Deg2RadF(22.5f);
	changeMoveState_.emplace(CARD_SELECT::NONE, [this]() {ChangeNone(); });
	changeMoveState_.emplace(CARD_SELECT::LEFT, [this]() {ChangeLeft(); });
	changeMoveState_.emplace(CARD_SELECT::RIGHT, [this]() {ChangeRight(); });
	changeMoveState_.emplace(CARD_SELECT::DISITION, [this]() {ChangeDisition(); });

	ChangeSelectState(CARD_SELECT::NONE);
	

	//手札にすべての初期札を入れる
	for (auto& it : uiInfos_)
	{
		handCards_.emplace_back(it);
	}
	auto beginit = handCards_.begin();
	auto endIt = handCards_.begin();
	//endItをbeginの５個先(６枚目)に指定する
	std::advance(endIt, VISIBLE_CARD_MAX);
	//座標を指定する時に使用する
	int i = 0;
	//６枚目までイテレータを回す


	for (auto it = beginit; it != endIt; it++)
	{
		it->currentAngle = ARROUND_PER_RAD * i- ARROUND_PER_RAD;
		it->cardPos.x = CENTER_X + std::sin(it->currentAngle) * radius_.x;
		it->cardPos.y = CENTER_Y - std::cos(it->currentAngle) * radius_.y;
		//常に強さ番号座標をローカル座標分追従させる
		it->numPos = it->cardPos + (NUM_LOCAL_POS * cardScl_);
		//見せるカード配列に入れる
		visibleCards_.emplace_back(*it);
		i++;
	}

	visibleEndPushIndex_ = static_cast<int>(visibleCards_.size());
	if (!visibleCards_.empty())
	{
		current_ = visibleCards_.begin();
		current_++;
	}
	if (!handCards_.empty())
	{
		handCurrent_ = handCards_.begin();
		handCurrent_++;
	}
}

void CardUI::Update(void)
{
	//カード状態
	cardUpdate_();

	//常に強さ番号座標をローカル座標分追従させる
	for (auto& card : visibleCards_)
	{
		card.numPos = card.cardPos + (NUM_LOCAL_POS * cardScl_);
	}
	for (auto& card : actions_)
	{
		card.numPos = card.cardPos + (NUM_LOCAL_POS * cardScl_);
	}
}

void CardUI::Draw(void)
{
	auto visibleCards = visibleCards_;
	//描画の時のみ逆に回す
	visibleCards.reverse();
	for (auto& card : visibleCards)
	{
		DrawCard(card);
	}

	//現在選択中のカードを強調表示
	DrawCard(*current_);

	for (auto& card : actions_)
	{
		DrawCard(card);
	}

#ifdef _DEBUG
	DrawDebug();
#endif // _DEBUG

	
}


void CardUI::AddCardUi(const CardBase::CARD_STATUS _status)
{
	CARD_UI_INFO info={};

	//属性によって画像を変える
	switch (_status.type_)
	{
	case CardBase::CARD_TYPE::ATTACK:
		info.typeImg = atkCardImg_;
		break;
	case CardBase::CARD_TYPE::RELOAD:
		info.typeImg = reloadCardImg_;
	}

	//呼び出し元の配列からの情報を代入する
	info.status = _status;

	//配列に挿入
	uiInfos_.emplace_back(info);
}

void CardUI::ChangeSelectState(const CARD_SELECT _select)
{
	if (cardMoveCnt_ < SELECT_MOVE_CARD_TIME)return;
	selectState_ = _select;
	changeMoveState_[selectState_]();
}

void CardUI::CardDisition(void)
{

}
#ifdef _DEBUG
void CardUI::DrawDebug(void)
{
	DrawFormatString(10, 50, 0xffffff, L"pow : %d", handCurrent_->status.pow_);
}
#endif // _DEBUG
void CardUI::ChangeNone(void)
{
	cardMoveCnt_ = SELECT_MOVE_CARD_TIME;
	//目標角度を現在の角度にする
	for (auto& card : visibleCards_)
	{
		card.goalAngle = 0.0f;
	}
	cardUpdate_ = [this]() {UpdateNone(); };
}

void CardUI::ChangeLeft(void)
{
	cardMoveCnt_ = SELECT_MOVE_CARD_TIME;

	//カードの範囲変数を更新する
	current_++;


	//先頭に追加
	auto it = handCurrent_;
	const int NEXT_CARD_NO = 5;
	for (int i = 0; i < NEXT_CARD_NO;i++)
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
	it->currentAngle=ARROUND_PER_RAD* size - ARROUND_PER_RAD;
	visibleCards_.emplace_back(*it);

	//手札選択カードを更新
	AddHandCurrent();

	//目標角度を代入
	for (auto& card : visibleCards_)
	{
		card.goalAngle = card.currentAngle - ARROUND_PER_RAD;
	}
	SoundManager::GetInstance().Play(SoundManager::SRC::CARD_MOVE, SoundManager::PLAYTYPE::BACK);
	cardUpdate_ = [this]() {UpdateLeft(); };
}

void CardUI::ChangeRight(void)
{
	cardMoveCnt_ = SELECT_MOVE_CARD_TIME;

	//visible配列に入れる前に現在の番地を引くことで、
	//結果的に1番目が選択されていることになる
	current_--;


	//先頭に追加
	auto it = handCurrent_;
	for (int i = 0; i < 2; i++)
	{
		if (it == handCards_.begin())
		{
			it = handCards_.end();
		}
		it--;
	}
	//std::advance(it, visibleStartCardIndex_);
	it->currentAngle = -ARROUND_PER_RAD * 2;
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

void CardUI::ChangeDisition(void)
{
	disitionCnt_ = DISITION_MOVE_CARD_TIME;
	actions_.emplace_back(*current_);
	
	//現在選択中を更新
	current_++;
	
	//更新前のカード(更新後のひとつ前)を見せるカードから消す
	auto prev = current_;
	std::advance(prev, -1);
	visibleCards_.erase(prev);


	
	if (handCards_.size() > VISIBLE_CARD_MAX)
	{
		//先頭に追加
		auto endit = handCurrent_;
		const int NEXT_CARD_NO = 5;
		for (int i = 0; i < NEXT_CARD_NO; i++)
		{
			endit++;
			if (endit == handCards_.end())
			{
				endit = handCards_.begin();
			}
		}
		//次の角度を現在角度に代入
		endit->currentAngle = ARROUND_PER_QUAD_RAD + ARROUND_PER_RAD;
		visibleCards_.emplace_back(*endit);
	}


	AddHandCurrent();
	auto hand = handCurrent_;
	if (hand == handCards_.begin())
	{
		hand = std::prev(handCards_.end());
	}
	else
	{
		std::advance(hand, -1);
	}
	handCards_.erase(hand);


	//カードの範囲変数を更新する

	auto it = std::next(current_);
	for (; it != visibleCards_.end(); it++)
	{
		it->goalAngle+= it->currentAngle - ARROUND_PER_RAD;
	}
	cardUpdate_ = [this]() {UpdateDisition(); };
}

void CardUI::UpdateNone(void)
{
	//配列ごとに座標を追従させる
	int i = 0;
	int visible = 7;

	////何もしない
	//for(auto& card:visibleCards_)
	//{
	//	card.currentAngle = ARROUND_PER_RAD * i - ARROUND_PER_RAD;
	//	card.cardPos.x = CENTER_X + std::sin(card.currentAngle) * radius_.x;
	//	card.cardPos.y = CENTER_Y - std::cos(card.currentAngle) * radius_.y;
	//	i++;
	//}

}

void CardUI::UpdateLeft(void)
{
	cardMoveCnt_ -= SceneManager::GetInstance().GetDeltaTime();
	if (cardMoveCnt_ < 0.0f)
	{
		visibleCards_.pop_front();
		CurrentAngle();
		changeMoveState_[CARD_SELECT::NONE]();
		return;
	}
	MoveCardAll();
}

void CardUI::UpdateRight(void)
{
	cardMoveCnt_ -= SceneManager::GetInstance().GetDeltaTime();
	if (cardMoveCnt_ < 0.0f)
	{
		visibleCards_.pop_back();
		CurrentAngle();
		changeMoveState_[CARD_SELECT::NONE]();
		return;
	}
	MoveCardAll();
}

void CardUI::UpdateDisition(void)
{
	int i = 0;
	disitionCnt_ -= SceneManager::GetInstance().GetDeltaTime();
	int cardSize = static_cast<int>(visibleCards_.size());
	//選択したカードの情報を取得
	for (auto& card : actions_)
	{
		card.state = CARD_STATE::USING;
		card.cardPos = UtilityCommon::Lerp(card.cardPos, DISITON_CARD_POS,
			(DISITION_MOVE_CARD_TIME - disitionCnt_) / DISITION_MOVE_CARD_TIME);
	}

	cardMoveCnt_-= SceneManager::GetInstance().GetDeltaTime();
	for (auto it = current_; it != visibleCards_.end(); it++)
	{
		MoveSpecificCard(*it);
	}
	if (disitionCnt_ < 0.0f)
	{

		changeMoveState_[CARD_SELECT::NONE]();
	}
	
}

void CardUI::MoveCardAll(void)
{
	for (auto& card : visibleCards_)
	{
		MoveSpecificCard(card);
	}
}

void CardUI::MoveSpecificCard(CARD_UI_INFO& _card)
{
	float time = (SELECT_MOVE_CARD_TIME - cardMoveCnt_) / SELECT_MOVE_CARD_TIME;
	float startRad = _card.currentAngle;
	float goalRad = _card.goalAngle;
	_card.currentAngle = UtilityCommon::LerpRad(startRad
		, goalRad, time);

	_card.cardPos.x = CENTER_X + std::sin(_card.currentAngle) * radius_.x;
	_card.cardPos.y = CENTER_Y - std::cos(_card.currentAngle) * radius_.y;
}

void CardUI::AddIndex(int& _index)
{
	_index++;
	int handSize = static_cast<int>(handCards_.size());
	if (_index > handSize - 1)
	{
		_index = _index - handSize;
	}
}

void CardUI::SubIndex(int& _index)
{
	_index--;
	int handSize = static_cast<int>(handCards_.size());
	if (_index < 0)
	{
		_index = handSize + _index;
	}
}

void CardUI::AddHandCurrent(void)
{
	handCurrent_++;
	if (handCurrent_ == handCards_.end())
	{
		handCurrent_ = handCards_.begin();
	}
}

void CardUI::SubHandCurrent(void)
{
	if (handCurrent_ == handCards_.begin())
	{
		handCurrent_ = handCards_.end();
	}
	handCurrent_--;
}

void CardUI::DrawCard(const CARD_UI_INFO _card)
{
	constexpr double NUM_SCL = 0.18;
	//カードの描画
	DrawRotaGraphF(_card.cardPos.x, _card.cardPos.y, cardScl_, 0.0f, _card.typeImg, true);

	int num = _card.status.pow_ - 1;
	if (num == -1) { num = 9; }
	DrawRotaGraphF(_card.numPos.x, _card.numPos.y, cardScl_ * NUM_SCL, 0.0f, cardNoImgs_[num], true);
	DrawLine(CENTER_X, CENTER_Y, _card.cardPos.x, _card.cardPos.y, 0xFFFFFF);
}

void CardUI::CurrentAngle(void)
{
	for (auto& card : visibleCards_)
	{
		card.goalAngle = card.currentAngle;
	}
}
