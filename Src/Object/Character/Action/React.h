#pragma once
#include "../Base/ActionBase.h"
class React :
    public ActionBase
{

public:
    /// @brief コンストラクタ
	/// @param _actCntl アクションコントローラー
    React(ActionController& _actCntl);

	/// @brief デストラクタ
    /// @param  
    ~React(void)override;

	/// @brief ロード
	/// @param  
	void Load(void) override;

    /// @brief 初期化
    /// @param  
    void Init(void) override;

	/// @brief 更新
	/// @param
    void Update(void) override;
private:

};

