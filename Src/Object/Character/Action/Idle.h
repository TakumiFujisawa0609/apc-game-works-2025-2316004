#pragma once
#include "../Base/ActionBase.h"
class Idle :
    public ActionBase
{

public:

    Idle(ActionController& _actCntl);
    ~Idle(void)override;
    void Init(void) override;
    void Update() override;

public:


};

