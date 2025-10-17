#pragma once
#include <string>
#include <vector>

class ResourceData
{

public:
	
	// リソースタイプ
	enum class TYPE
	{
		NONE,
		IMG,
		IMGS,
		MODEL,
		SOUND,
		FONT,
		EFFEKSEER,
		VERTEX_SHADER,
		PIXEL_SHADER,
	};

	// コンストラクタ
	ResourceData(void);
	// コンストラクタ
	ResourceData(TYPE type, const std::wstring& path);
	// コンストラクタ(IMGS用)
	ResourceData(TYPE type, const std::wstring& path, int numX, int numY, int sizeX, int sizeY);

	// デストラクタ
	~ResourceData(void);

	/// @brief 読み込み
	/// @param  
	void Load(void);

	/// @brief 解放
	/// @param  
	void Release(void);

	/// @brief 複数画像ハンドルを別配列にコピー
	/// @param imgs 
	void CopyHandle(int* imgs);

	// リソースタイプ
	TYPE type_;

	// リソースの読み込み先
	std::wstring path_;

	// 画像とモデルのハンドルID
	int handleId_;

	// IMGS::LoadDivGraph用
	int* handleIds_;
	int numX_;
	int numY_;
	int sizeX_;
	int sizeY_;

	// モデル複製用
	std::vector<int> duplicateModelIds_;

};
