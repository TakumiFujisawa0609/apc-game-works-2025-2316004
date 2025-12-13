#pragma once
#include "../Base/ActionBase.h"
class Jump :
    public ActionBase
{

public:

    Jump(ActionController& _actCntl,CharacterBase& _charaObj,VECTOR& _jumpPow);
    ~Jump(void)override;
    void Init(void) override;
    void Update(void) override;

public:

    //ジャンプ力
    static constexpr float POW_JUMP = 20.0f;
    //重力
    static constexpr float GRAVITY = 1.3f;
    //ジャンプ高さ
	static constexpr float JUMP_HEIGHT = 400.0f;

    //移動量
    VECTOR velocity_;

    VECTOR& jumpPow_;		// ジャンプ量
    //オブジェクト
    CharacterBase& charaObj_;
};

