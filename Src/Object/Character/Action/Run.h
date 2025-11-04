#pragma once
#include "../Base/ActionBase.h"
class Run :
    public ActionBase
{
public:
    Run(ActionController& _actCntl,float& _speed);
    ~Run(void)override;
	void Init(void) override;
    void Update() override;
public:
    float& moveSpd_;
};

