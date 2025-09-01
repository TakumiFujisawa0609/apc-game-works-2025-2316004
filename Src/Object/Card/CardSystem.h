#pragma once

#include "../../Template/Singleton.h"

#include<vector>
#include<memory>
class CardDeck;
class CardSystem : public Singleton<CardSystem>
{
	friend class Singleton<CardSystem>;
public:
	//プレイヤーの配列番号
	static constexpr int PLAYER_NO = 0;

	//CPUのの配列番号
	static constexpr int CPU_NO = 1;

	//先出しの番号
	static constexpr int FIRST_ATK = 0;

	//後だしの番号
	static constexpr int SECOND_ATK = 1;

	//配列数
	static constexpr int ARRAY_NUM = 2;

	//返す勝敗判定
	enum class BATTLE_RESULT
	{
		NONE,
		SUCCESS_USE,		//成功
		SUCCESS_USE_CONTINUE,//成功(連続攻撃)
		SUCCESS_CARD_BREAK,	//成功(カード破壊)
		FAILURE_USE_BE_REFLECTED,	//失敗(反射された)

		SUCCESS_REFLECT,	//成功(反射)
		NOTHING,

		GIVE_DRAW,		//引き分けにした
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

	/// <summary>
	/// 勝敗結果の取得
	/// </summary>
	/// <param name="_cardPlayerNo"></param>
	/// <returns>カード勝負プレイヤーの番号</returns>
	const BATTLE_RESULT GetResult(int _cardPlayerNo)const;
private:

	CardSystem(void);
	~CardSystem(void) = default;

	//場に出ているカード
	int putCardPow_[ARRAY_NUM];

	//カードを場に出せるか
	bool canPut_;

	//勝敗結果
	BATTLE_RESULT result_[ARRAY_NUM];

	//結果返す時のプレイヤーの識別
	BATTLE_RESULT playerResult_[ARRAY_NUM];
};

