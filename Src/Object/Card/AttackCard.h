#pragma once
#include "CardBase.h"
class AttackCard :
    public CardBase
{
public:
	//コンストラクタ
	AttackCard(const int _pow);
	//デストラクタ
	~AttackCard(void)override;

	void Execute(void)override;
private:

};

