#pragma once
#include <unordered_map>
#include <string>
#include "../Template/Singleton.h"



class SoundManager : public Singleton<SoundManager>
{
	friend class Singleton<SoundManager>;
public:

	/// リソース種類
	enum class SRC
	{
		NONE,						//なし

		//BGM
		TITLE_BGM,					//タイトルBGM
		GAME_BGM,					//ゲームBGM
		GAME_CLEAR,
		GAME_OVER,
		//SE

		//足音
		PLAYER_FOOT_SE,				//プレイヤー足音
		ENEMY_FOOT_SE,				//エネミー足音
		ENEMY_STOMP_SE,				//エネミー爆発音
		ENEMY_CHARGE_SE,			//エネミーチャージ音
		ENEMY_JUMP_LAND_SE,			//エネミージャンプ着地音
		ENEMY_HIT_SE,			//エネミーヒット着地音
		PLAYER_ATTACK_SE,			//プレイヤー攻撃音
		PLAYER_DODGE_SE,			//プレイヤー回避音
		PLAYER_HIT_SE,				//プレイヤーヒット音


		//カード関連
		CARD_MOVE,					//カード移動
		CARD_BE_REFLECTED,			//カード弾かれ音
		CARD_BREAK,					//カード弾き音(カードに勝った)
		CARD_RELOAD,				//カードリロード音
		CARD_RELOAD_FINISH,			//カードリロード終了音

		//ボタン
		MOVE_BTN_SE	,				//移動ボタン
		DESIDE_BTN_SE,				//決定ボタン
		GAME_START_SE				//ゲームスタート音

	};

	/// 音源種類
	enum class TYPE
	{
		BGM,						//BGM
		SE,							//効果音
		MAX
	};

	/// 再生種類
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

	/// @brief デストラクタ
	/// @param  
	~SoundManager(void);

	/// @brief リソースの解放
	/// @param  
	void Release(void);

	/// @brief 初期化
	/// @param  
	void Init(void);

	/// @brief リソースの読み込み
	/// @param _src リソース種類
	/// @param _pitch ピッチ調整値(0.0fで通常のピッチ、正の値で高く、負の値で低くなる)
	/// @return true:読み込み成功　false:場合失敗
	const bool LoadResource(const SRC _src, const float _pitch = 0.0f);

	/// @brief 音源の再生
	/// @param _src リソース種類
	/// @param _playType 再生種類
	void Play(const SRC _src, const PLAYTYPE _playType);
	
	/// @brief 音源の停止
	/// @param _src リソース種類
	void Stop(const SRC _src);

	/// @brief 再生中かを返す
	/// @param _src リソース種類
	/// @return trueの:再生中　false;場合再生していない
	bool IsPlay(const SRC _src) const;

	/// @brief 読み込んだ音量を設定する
	/// @param  
	/// @return 
	const void SetLoadedSoundsVolume(void) { for (int i = 0; i < TYPE_MAX; i++) { SetSystemVolume(volume_[i], i); } };

	/// @brief 
	/// @param _src 
	/// @param _volumePercent 
	void SetSoundVolumeSRC(const SRC _src, const int _volumePercent);

	/// @brief 音量の設定
	/// @param _volumePercent 音量パーセント
	/// @param _type サウンド種類
	void SetSystemVolume(const int _volumePercent, const int _type);

	/// @brief 音量を返す
	/// @param _type サウンド種類
	/// @return 指定したサウンド種類の音量を返す
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
	bool _Load(const SRC _src, const float _pich = 0.0f);

	//再生種類を取得
	int GetPlayType(const PLAYTYPE _playType);

};