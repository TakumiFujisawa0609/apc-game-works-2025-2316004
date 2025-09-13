#pragma once
#include "../ActionBase.h"
class Run :
    public ActionBase
{
public:
    Run(void);
    ~Run(void)override;
    void Update() override;
    void Draw() override;
    void Start() override;
	void End() override;
};

