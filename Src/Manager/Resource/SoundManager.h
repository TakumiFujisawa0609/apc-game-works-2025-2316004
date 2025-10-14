#pragma once
#include <unordered_map>
#include <string>

class SoundManager
{
public:

	/// <summary>
	/// リソース種類
	/// </summary>
	enum class SRC
	{
		NONE,						//なし

		//BGM
		TITLE_BGM,					//タイトルBGM
		//SE
		CARD_MOVE,
	};

	/// <summary>
	/// 音源種類
	/// </summary>
	enum class TYPE
	{
		BGM,						//BGM
		SE,							//効果音
		MAX
	};

	/// <summary>
	/// 再生種類
	/// </summary>
	enum class PLAYTYPE
	{
		NORMAL,	//ノーマル再生
		LOOP,	//ループ再生
		BACK	//バックグラウンド再生
	};

	//デフォルトの音量
	static constexpr int DEFAULT_VOLUME = 70;

	//音源種類最大数
	static constexpr int TYPE_MAX = static_cast<int>(TYPE::MAX);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SoundManager(void);

	/// <summary>
	/// 明示的にインスタンス生成する
	/// </summary>
	/// <param name=""></param>
	static void CreateInstance(void);

	/// <summary>
	/// 静的インスタンスの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>インスタンス
	static SoundManager& GetInstance(void);

	/// <summary>
	/// 解放
	/// </summary>
	void Destroy(void);

	/// <summary>
	/// リソースの解放
	/// </summary>
	void Release(void);

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(void);

	/// <summary>
	/// リソースの読み込み
	/// </summary>
	/// <param name="_src">リソース種類</param>
	/// <returns>trueの場合読み込み成功,falseの場合失敗</returns>
	const bool LoadResource(const SRC _src);

	/// <summary>
	/// 音源の再生
	/// </summary>
	/// <param name="_src">リソース種類</param>
	/// <param name="_playType">再生種類</param>
	void Play(const SRC _src, const PLAYTYPE _playType);

	/// <summary>
	/// 音源の停止
	/// </summary>
	/// <param name="_src">リソース種類</param>
	void Stop(const SRC _src);

	/// <summary>
	/// 再生中かを返す
	/// </summary>
	/// <param name="_src">リソース種類</param>
	/// <returns>trueの場合再生中,falseの場合再生していない</returns>
	bool IsPlay(const SRC _src) const;

	/// <summary>
	/// 読み込んだ音量を設定する
	/// </summary>
	/// <returns></returns>
	const void SetLoadedSoundsVolume(void) { for (int i = 0; i < TYPE_MAX; i++) { SetSystemVolume(volume_[i], i); } };

	/// <summary>
	/// 音量の設定
	/// </summary>
	/// <param name="_volumePercent">音量パーセント</param>
	/// <param name="_type">サウンド種類</param>
	void SetSystemVolume(const int _volumePercent, const int _type);

	/// <summary>
	/// 音量を返す
	/// </summary>
	/// <param name="_type">サウンド種類</param>
	/// <returns>指定したサウンド種類の音量を返す</returns>
	const int GetSoundTypeVolume(const int _type) const { return volume_[_type]; }

private:

	struct SoundResource
	{
		int handleId = -1;		//音源ハンドルID
		TYPE type = TYPE::MAX;	//音源の種類
		std::wstring path = L"";	//音源のパス
	};		
	
	//静的インスタンス
	static SoundManager* instance_;
	
	//ボリューム
	int volume_[TYPE_MAX];

	//管理対象
	std::unordered_map<SRC, SoundResource> resourcesMap_;

	//読み込み済み
	std::unordered_map<SRC, SoundResource> loadedMap_;

	// コンストラクタ
	SoundManager(void);

	// デフォルトコンストラクタは使用不可
	SoundManager(const SoundManager&) = delete;

	// コピー代入演算子は使用不可
	SoundManager& operator=(const SoundManager&) = delete;

	//内部読み込み処理
	bool _Load(const SRC _src);

	//再生種類を取得
	int GetPlayType(const PLAYTYPE _playType);

};