#pragma once
#include "../Template/Singleton.h"

class UIBase2D;

class UI2DManager:public Singleton<UI2DManager>
{
	//シングルトンにだけ共有する
	friend class Singleton<UI2DManager>;
public:

	/// @brief ロード
	/// @param  
	void Load(void);

	/// @brief 初期化
	/// @param  
	void Init(void);

	/// @brief 更新
	/// @param  
	void Update(void);

	/// @brief 描画
	/// @param  
	void Draw(void);

	/// @brief 2DUIの追加
	/// @param _ui2D 
	void AddUi2D(std::unique_ptr<UIBase2D>_ui2D);

private:
	UI2DManager(void);
	~UI2DManager(void)override;
	std::vector<std::unique_ptr<UIBase2D>>ui2D_;
};

