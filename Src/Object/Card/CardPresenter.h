#pragma once
#include <memory>
#include "../Character/UIData/CharacterUIData.h"
#include "../Card/CardUiBase.h"
class CardDeck;
class CardPresenter
{
public:
	CardPresenter(CHARACTER_TYPE _type,CardDeck& _deck);
	~CardPresenter(void);

	//カード使用済み
	void FinishCard(void);

	//カード負け
	void FailureCard(void);

	//カードを引く
	void PutCard(void);

	/// @brief リボルバーの回転
	/// @param _moveLR 動かす方向
	void RoleRevolver(const CardUIBase::CARD_SELECT _moveLR);

	/// @brief カードの種類を取得
	/// @param  
	/// @return カードの種類
	const CardBase::CARD_TYPE GetCardType(void)const;

	/// @brief 手札の取得
	/// @param  
	/// @return 
	const std::vector<CardBase::CARD_TYPE> GetHandCardType(void)const;

	/// @brief デッキクラスでのリロード処理
	/// @param  
	void DeckReload(void);
	
	/// @brief カード変更
	/// @param  
	void ChangeCard(void);

	/// @brief 敵のリロード
	/// @param  
	void EnemyCardReload(void);

	/// @brief アクション遷移時にカードをアクション中のカードを開放
	/// @param  
	void ChangeAction(void);

	/// @brief カード負けしたかどうか
	/// @param  
	const bool IsCardFailure(void)const;
	
protected:
	//カードデッキ
	CardDeck& deck_;

	//カードUI
	CardUIBase& cardUI_;

	//キャラクター
	CHARACTER_TYPE type_;
	
};

