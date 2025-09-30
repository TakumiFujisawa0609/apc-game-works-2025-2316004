#pragma once
#include "../Base/ActionBase.h"
class React :
    public ActionBase
{

public:
    React(ActionController& _actCntl);
    ~React(void)override;
    void Init(void) override;
    void Update(void) override;
private:

};

