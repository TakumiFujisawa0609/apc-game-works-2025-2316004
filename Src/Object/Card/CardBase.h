#pragma once
class CardBase
{
public:
	enum class CARD_TYPE
	{
		ATTACK
		,MAGIC
		,ITEM
		,RELOAD
	};

	struct CARD_STATUS
	{
		int pow_;			//カードの強さ
		CARD_TYPE type_;	//カードの種類
	};

	//コンストラクタ
	CardBase(const CARD_STATUS _status);
	//デストラクタ
	virtual~CardBase(void);

	virtual void Execute(void);
	
	//カードの威力の取得
	inline const CARD_STATUS GetCardStatus(void)const { return status_; }

	//カードの種類の取得
	//inline const CARD_TYPE GetCardType(void)const { return type_; }

	//カードの勝利の取得
	inline void SetIsWin(const bool _isWin) { isWin_ = _isWin; }

protected:
	//カードの強さ
	//int pow_;
	////カードの種類
	//CARD_TYPE type_;
	CARD_STATUS status_;
	//カードの勝敗
	bool isWin_;
private:
};

