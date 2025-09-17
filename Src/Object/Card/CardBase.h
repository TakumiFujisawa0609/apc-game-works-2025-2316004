#pragma once
class CardBase
{
public:
	enum class CARD_TYPE
	{
		ATTACK
		,MAGIC
		,ITEM
	};
	//コンストラクタ
	CardBase(const int _pow);
	//デストラクタ
	virtual~CardBase(void);

	virtual void Execute(void);
	
	//カードの威力の取得
	inline const int GetPow(void)const { return pow_; }

	//カードの勝利の取得
	inline void SetIsWin(const bool _isWin) { isWin_ = _isWin; }

protected:
	//カードの強さ
	int pow_;
	//カードの種類
	CARD_TYPE type_;
	//カードの勝敗
	bool isWin_;
private:
};

