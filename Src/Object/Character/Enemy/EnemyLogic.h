#pragma once
#include "../Base/LogicBase.h"
class EnemyLogic :
    public LogicBase
{
public:
	EnemyLogic(void);
	~EnemyLogic(void) override;

	void Init(void) override;
	void Update(void) override;
private:
};

