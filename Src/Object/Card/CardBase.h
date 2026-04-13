#pragma once
#include "../Object/ObjectBase.h"
#include"../Common/Vector2F.h"

class CardBase
	:public ObjectBase
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

	/// @brief ロード
	/// @param  
	virtual void Load(void);

	/// @brief 初期化
	/// @param  
	virtual void Init(void);
	
	/// @brief 更新
	/// @param  
	virtual void Update(void);
	
	/// @brief 描画
	/// @param  
	virtual void Draw(void);

	/// @brief カードの威力の取得
	/// @param  
	/// @return カードのステータス
	inline const CARD_STATUS& GetCardStatus(void)const { return status_; }

	//カードの種類の取得
	//inline const CARD_TYPE GetHandCardType(void)const { return type_; }

	/// @brief カードの勝利の取得
	/// @param _isWin 
	inline void SetIsWin(const bool& _isWin) { isWin_ = _isWin; }

	/// @brief カード使用済みのセット
	/// @param _isUsed true:使用済み　false:未使用
	inline void SetIsUsed(const bool& _isUsed) { isUsed_ = _isUsed; }

protected:

	//カードのナンバーローカル座標
	static constexpr Vector2F NUM_LOCAL_POS = { 35.0f,53.0f };
	//カードのスケール
	static constexpr float CARD_SCALE = 1.4f;

	////カードの種類
	CARD_STATUS status_;
	//カードの勝敗
	bool isWin_;
	//使用済みかどうか
	bool isUsed_;
	//カード番号イメージ
	int* cardNoImg_;
	//攻撃カード画像
	int atkCardImg_;
	//カードの大きさ
	double cardScl_;
	//カードの座標
	Vector2F cardPos_;
	//カードナンバー座標
	Vector2F numPos_;
private:
};

