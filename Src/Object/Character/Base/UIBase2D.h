#pragma once
#include<memory>
#include "../Common/Vector2F.h"
class Easing;
class UIBase2D
{
public:

	UIBase2D(void);
	virtual ~UIBase2D(void);

	/// @brief ロード
	/// @param  
	virtual void Load(void) = 0;

	/// @brief 初期化
	/// @param  
	virtual void Init(void) = 0;

	/// @brief 更新
	/// @param  
	virtual void Update(void) = 0;

	/// @brief 描画
	/// @param  
	virtual void Draw(void) = 0;
protected:
	std::unique_ptr<Easing>easing_;
private:
};

