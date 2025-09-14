#pragma once
#include "../Base/InputBase.h"
class EnemyInput :
    public InputBase
{
public:
	EnemyInput(void);
	~EnemyInput(void) override;

	void Init(void) override;
	void Update(void) override;
private:
};

