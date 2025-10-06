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
	
	virtual void Draw(void);
	//カードの威力の取得
	inline const CARD_STATUS GetCardStatus(void)const { return status_; }

	//カードの種類の取得
	//inline const CARD_TYPE GetCardType(void)const { return type_; }

	//カードの勝利の取得
	inline void SetIsWin(const bool _isWin) { isWin_ = _isWin; }
	/// <summary>
	/// カード使用済みのセット
	/// </summary>
	/// <param name="_isUsed">true:使用済み　false:未使用</param>
	inline void SetIsUsed(const bool _isUsed) { isUsed_ = _isUsed; }

protected:
	//カードの強さ
	//int pow_;
	////カードの種類
	//CARD_TYPE type_;
	CARD_STATUS status_;
	//カードの勝敗
	bool isWin_;
	//使用済みかどうか
	bool isUsed_;
private:
};

