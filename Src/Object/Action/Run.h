#pragma once
#include "../Character/Base/ActionBase.h"
class Run :
    public ActionBase
{
public:
    Run(InputBase& _input);
    ~Run(void)override;
	void Init(void) override;
    void Update() override;
public:
    //移動スピード
    static constexpr float MOVE_SPEED = 6.0f;
};

