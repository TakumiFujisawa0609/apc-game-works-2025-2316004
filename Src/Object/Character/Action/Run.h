#pragma once
#include "../Base/ActionBase.h"
class Run :
    public ActionBase
{
public:
    Run(ActionController& _actCntl);
    ~Run(void)override;
	void Init(void) override;
    void Update() override;
public:

};

