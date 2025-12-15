#pragma once
#include "../Manager/Resource/SoundManager.h"
#include "../Base/ActionBase.h"
class Run :
    public ActionBase
{
public:
    Run(ActionController& _actCntl,float& _speed,SoundManager::SRC _src,const float _footSeDis);
    ~Run(void)override;

	/// @brief ロード
	/// @param  
	void Load(void) override;

	/// @brief 初期化
	/// @param  
	void Init(void) override;

    /// @brief 更新
    /// @param  
    void Update(void) override;
public:
    //移動速度
    float& moveSpd_;

    //足音間隔
	float footSECnt_;

	//足音音源
	SoundManager::SRC footSESrc_;

    //足音再生間隔
	float  footSeDis_;


};

