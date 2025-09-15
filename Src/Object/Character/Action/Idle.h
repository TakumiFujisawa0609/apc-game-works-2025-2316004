#pragma once
#include "../Base/ActionBase.h"
class Idle :
    public ActionBase
{
public:
    Idle(InputBase& _input);
    ~Idle(void)override;
    void Init(void) override;
    void Update() override;
public:
};

