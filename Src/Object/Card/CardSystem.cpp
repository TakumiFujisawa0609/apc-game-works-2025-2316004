
#include "CardBase.h"
#include"CardDeck.h"
#include "CardSystem.h"

CardSystem::CardSystem(void):
	putCardPow_{-1,-1},
	canPut_(true),
	isFirstAtk_{ false,false },
	playerResult_{BATTLE_RESULT::NONE,BATTLE_RESULT::NONE }
{
	
}


const CardSystem::BATTLE_RESULT CardSystem::GetResult(const int _cardPlayerNo) const
{
	//配列数より大きい数字を指定されたら何も返さない
	if (_cardPlayerNo >= ARRAY_NUM)return CardSystem::BATTLE_RESULT::NONE;
	CardSystem::BATTLE_RESULT result = playerResult_[_cardPlayerNo];

	return result;
}

void CardSystem::InitPutCardPow(const int _playerNo)
{
	//bool isFirstAtk[ARRAY_NUM] = { false,false };
	for (int i = 0; i < ARRAY_NUM; i++)
	{
		//isFirstAtk[i] = isFirstAtk_[i];
		if (isFirstAtk_[_playerNo])
		{
			putCardPow_[FIRST_ATK] = -1;
			isFirstAtk_[_playerNo] = false;
			break;
		}
		else
		{
			putCardPow_[SECOND_ATK] = -1;
			break;
		}
	}
}

void CardSystem::LoseInitPutCardPow(const int _playerNo)
{
	if (isFirstAtk_[_playerNo] == true)
	{
		//先出しが負けた場合の処理

		//先出しのカードを消す
		putCardPow_[FIRST_ATK] = -1;
		isFirstAtk_[_playerNo] = false;

		//後出しのカードを先出しにする
		if (putCardPow_[SECOND_ATK] != -1)
		{
			putCardPow_[FIRST_ATK] = putCardPow_[SECOND_ATK];
			putCardPow_[SECOND_ATK] = -1;

			for(int i=0;i<ARRAY_NUM;i++)
			{
				if (i!=_playerNo)
				{
					isFirstAtk_[i] = true;
					break;
				}
			}
		}
	}
	else
	{
		putCardPow_[SECOND_ATK] = -1;
	}
}

void CardSystem::JudgeIsFirstAtk(const int _playerNo)
{
	if (putCardPow_[FIRST_ATK] == -1 && putCardPow_[SECOND_ATK] == -1)
	{
		isFirstAtk_[_playerNo] = true;
	}
}

void CardSystem::DrawDebug(void)
{
	DrawFormatString(200, 200, 0x000000, L"CardPow(%d,%d)\nisFirst(%d,%d)"
		, putCardPow_[0], putCardPow_[1],isFirstAtk_[0],isFirstAtk_[1]);

}


void CardSystem::CompareCards(void)
{
	//どちらかにカードが入っていなければ引ける
	if (putCardPow_[FIRST_ATK] == -1 || putCardPow_[SECOND_ATK] == -1)
	{
		canPut_ = true;
	}
	//どちらも入っていれば引けない
	else if(putCardPow_[FIRST_ATK] != -1 && putCardPow_[SECOND_ATK] != -1)
	{
		canPut_ = false;
	}
	//お互いに手札が0枚なら処理を抜ける
	if (putCardPow_[FIRST_ATK]==-1 && putCardPow_[SECOND_ATK] ==-1)
	{
		return;
	}

	//以下、2つのカードの強さを比較
	//どちらかのカードが出されていなければ先出し勝利
	if (putCardPow_[SECOND_ATK] == -1)
	{
		result_[FIRST_ATK] = BATTLE_RESULT::SUCCESS_USE;
	}
	//先出しと後出しカードが同じ強さの時はドロー
	else if (putCardPow_[FIRST_ATK] == putCardPow_[SECOND_ATK])
	{
		//引き分け
		result_[FIRST_ATK] = BATTLE_RESULT::BE_DRAW;
		result_[SECOND_ATK] = BATTLE_RESULT::GIVE_DRAW;


		//カードを両方リセット（Re:COM仕様）
		putCardPow_[FIRST_ATK] = -1;
		putCardPow_[SECOND_ATK] = -1;
	}
	//先出しの勝ち
	else if (putCardPow_[FIRST_ATK] > putCardPow_[SECOND_ATK])
	{
		result_[FIRST_ATK] = BATTLE_RESULT::SUCCESS_CARD_BREAK;
		result_[SECOND_ATK] = BATTLE_RESULT::FAILURE_USE_BE_REFLECTED;
	}
	//後出しの勝ち
	else if (putCardPow_[FIRST_ATK] < putCardPow_[SECOND_ATK])
	{
		result_[FIRST_ATK] = BATTLE_RESULT::FAILURE_USE_BE_REFLECTED;
		result_[SECOND_ATK] = BATTLE_RESULT::SUCCESS_CARD_BREAK;
	}

	//各プレイヤーの結果に判定結果を反映する
	for (int i = 0; i < ARRAY_NUM; i++)
	{
		if (isFirstAtk_[i])
		{
			playerResult_[i] = result_[FIRST_ATK];
		}
		else
		{
			playerResult_[i] = result_[SECOND_ATK];
		}
	}
}

void CardSystem::PutCard(const int _pow, const int _playerNum)
{
	//すでに強さが代入されていれば処理を抜ける
	if (putCardPow_[PLAYER_NO] != -1 && putCardPow_[CPU_NO] != -1)return;
	//カードの強さをセットする
	if (isFirstAtk_[_playerNum])
	{
		putCardPow_[FIRST_ATK] = _pow;
	}
	else
	{
		putCardPow_[SECOND_ATK] = _pow;
	}
}

