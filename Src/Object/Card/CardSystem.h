#pragma once

#include "../../Template/Singleton.h"

#include<vector>
#include<memory>
class CardDeck;
class CardSystem : public Singleton<CardSystem>
{
	friend class Singleton<CardSystem>;
public:
	//プレイヤーの番号
	static constexpr int PLAYER_NO = 0;

	//CPUの番号
	static constexpr int CPU_NO = 1;

	//配列数
	static constexpr int ARRAY_NUM = 2;

	//返す勝敗判定
	enum class BATLE_RESULT
	{
		NONE,
		SUCCESS_USE,		//成功
		SUCCESS_USE_CONTINUE,//成功(連続攻撃)
		SUCCESS_CARD_BREAK,	//成功(カード破壊)
		FAILURE_USE_BE_REFLECTED,	//失敗(反射された)

		SUCCESS_REFLECT,	//成功(反射)
		NOTHING,

		GIVE_DRAW,		//引き分け
		BE_DRAW,		//引き分けにされた

		RELOAD			//リロード
	};

	//ストックカードのカードを比較する
	void CompareCards(void);

	//場にカードを出す
	void PutCard(const int pow);

	/// <summary>
	/// カードを場に出せるかの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>true:場に出せる false:場に出せない</returns>
	const bool GetCanPut(void) const { return canPut_; }
private:

	CardSystem(void);
	~CardSystem(void) = default;

	//場に出ているカード
	int putCardPow_[ARRAY_NUM];

	//カードを場に出せるか
	bool canPut_;
};

