#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

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
	enum class SOUND_TYPE
	{
		NONE,
		BGM,
		SE,
	};
	enum class SET_SOUND_STATUS
	{
		NONE,			//なし
		PITCH,			//ピッチ調整
		TIME_STRETCH,	//タイムストレッチ
		LOOP_START,		//ループ開始
		LOOP_END,		//ループ終了
	};

	//struct SET_SOUND_FUNC
	//{
	//	std::function<void(void)> createFunc;
	//	std::function<void(void)> playFunc;
	//};

	/// @brief コンストラクタ
	/// @param  
	ResourceData(void);
	ResourceData(TYPE type, const std::wstring& path);
	ResourceData(TYPE type, const std::wstring& path, int numX, int numY, int sizeX, int sizeY);
	ResourceData(TYPE type, const std::wstring& path, SOUND_TYPE soundType
		, const float pitch = 0.0f
		, const float timeStretch = 1.0f
		, const float volume = 1.0f
		, const float loopStartTime = 0.0f, const float loopEndTime = 0.0f);

	/// @brief デストラクタ
	/// @param  
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

	//サウンド用
	SOUND_TYPE soundType_;
	float pitch_;
	float timeStretch_;
	float volume_;
	float loopStartTime_;
	float loopEndTime_;


	// モデル複製用
	std::vector<int> duplicateModelIds_;

	//ロード関数の表
	std::unordered_map<TYPE, std::function<void(void)>>loadFunc_;

	//解放関数の表
	std::unordered_map<TYPE, std::function<void(void)>>releaseFunc_;

	//サウンドの状態設定関数の表
	std::unordered_map < SET_SOUND_STATUS, std::function<void(void)>>setCreateFunc_;
	std::unordered_map < SET_SOUND_STATUS, std::function<void(void)>>setPlayFunc_;

	//関数ポインタの追加
	void AddFunc(void);

	//ロード
	void LoadImg(void);
	void LoadImgs(void);
	void LoadModel(void);
	void LoadSound(void);
	void LoadFont(void);
	void LoadEffekseer(void);
	void LoadVS(void);
	void LoadPS(void);

	//解放
	void ReleaseImg(void);
	void ReleaseImgs(void);
	void ReleaseModel(void);
	void ReleaseSound(void);
	void ReleaseFont(void);
	void ReleaseEffekseer(void);
	void ReleaseVS(void);
	void ReleasePS(void);

};
