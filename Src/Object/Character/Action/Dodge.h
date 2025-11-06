#pragma once
#include "../Base/ActionBase.h"
#include "../Object/Common/Transform.h"

class Dodge :
    public ActionBase
{

public:

    Dodge(ActionController& _actCntl,Transform& _trans,float& _spd);
    ~Dodge(void)override;
    void Init(void) override;
    void Update() override;
private:

    //通常移動スピードにプラスする
    static constexpr float ADD_DODGE_SPEED = 16.0f;

    //回避を終えるアニメーションステップ
    static constexpr float END_DODGE_ANIM_STEP = 30.0f;
    float& dodgeSpd_;

    Transform& trans_;
};

