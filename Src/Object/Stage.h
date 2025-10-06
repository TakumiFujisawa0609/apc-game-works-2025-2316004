#pragma once
#include "ObjectBase.h"
class Stage :
    public ObjectBase
{
public:
    void Load(void)override;
    void Init(void)override;
    void Update(void)override;
    void Draw(void)override;
private:
    static constexpr float STAGE_SCL = 50.0f;
};

