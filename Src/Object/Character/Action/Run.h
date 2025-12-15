#pragma once
#include "../Manager/Resource/SoundManager.h"
#include "../Base/ActionBase.h"
class Run :
    public ActionBase
{
public:
    Run(ActionController& _actCntl,float& _speed,SoundManager::SRC _src,const float _footSeDis);
    ~Run(void)override;
	void Init(void) override;
    void Update() override;
public:
    //ˆÚ“®‘¬“x
    float& moveSpd_;

    //‘«‰¹ŠÔŠu
	float footSECnt_;

	//‘«‰¹‰¹Œ¹
	SoundManager::SRC footSESrc_;

    //‘«‰¹Ä¶ŠÔŠu
	float  footSeDis_;


};

