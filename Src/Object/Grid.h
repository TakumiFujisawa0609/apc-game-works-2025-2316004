#pragma once

class Grid
{

public:

	/// @brief コンストラクタ
	/// @param  
	Grid(void);

	/// @brief コンストラクタ
	/// @param  
	~Grid(void);

	/// @brief 初期化処理
	/// @param  
	void Init(void);

	/// @brief 更新処理
	/// @param  
	void Update(void);
	
	/// @brief 描画処理
	/// @param  
	void Draw(void);

	/// @brief 解放処理
	/// @param  
	void Release(void);

};