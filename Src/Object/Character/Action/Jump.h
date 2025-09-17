#pragma once
#include "../Base/ActionBase.h"
class Jump :
    public ActionBase
{
public:
    Jump(ActionController& _actCntl);
    ~Jump(void)override;
    void Init(void) override;
    void Update(void) override;
public:
    //ジャンプ力
    static constexpr float POW_JUMP = 20.0f;
    //ジャンプ加速の倍率
    static constexpr float TIME_JUMP_SCALE = 1.0f;
};

