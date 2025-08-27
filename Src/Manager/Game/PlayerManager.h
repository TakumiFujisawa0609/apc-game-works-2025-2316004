#pragma once
#include<memory>
#include<vector>
#include"../../Object/Common/Transform.h"
#include"../../Object/Player/Player.h"
#include"../../Object/Player/Process/PlayerInput.h"
//#include"../../Object/Common/Capsule.h"
class Player;
class PlayerManager
{
public:
	//**************************************
	//定数
	//**************************************
	static constexpr int PLAYER_NUM_MAX = 4;

	//プレイヤー1人
	static constexpr int PLAYER_SINGLE = 1;

	//プレイヤーカラー
	static constexpr COLOR_F PLAYER_COLOR[PLAYER_NUM_MAX] =
	{		
		{0.6f,0.8f,1.0f,1.0f},
		{1.0f,0.6f,0.9f,1.0f},
		{0.7f,1.0f,0.8f,1.0f},
		{1.0f,1.0f,0.7f,1.0f},
	};

	
	enum class PLAYER
	{
		PLAYER_ONE,
		PLAYER_TWO,
		PLAYER_THREE,
		PLAYER_FOUR
	};


	//**************************************
	

	/// <summary>
	/// 静的にインスタンス生成
	/// </summary>
	/// <param name="_playerNum">プレイヤー人数</param>
	static void CreateInstance(void);

	//解放
	void Destroy(void);

	//静的にインスタンスを取得する
	static PlayerManager& GetInstance(void);
	void Load(void);
	void Init(void);
	void Update(void);
	void Draw(void);

	//*****************************************
	//ゲッタ
	//*****************************************
	//モデル情報ゲッタ
	const Transform& GetPlayerTransform(const int _num) { return players_[_num]->GetTransform(); }

	//プレイヤー情報
	std::vector<std::unique_ptr<Player>>&GetPlayers(void) { return players_; }


	//ゴールタイムのゲッタ
	const std::vector<float> GetGoalTime(void);

	//****************************************
	//セッタ
	//****************************************
	//初期座標に戻す
	void SetInitPos(VECTOR _worldPos);
private:
	//プレイヤーの大きさ
	static constexpr VECTOR MODEL_SCL = { 1.0f,1.0f,1.0f };



	//初期座標
	static constexpr float START_POS = 50.0f;

	//静的インスタンス
	static PlayerManager* instance_;

	//始まってからの総タイム
	//float time_;

	//ゴール時間
	std::vector<float>goalTime_;

	//*****************************************
	//メンバ変数
	//*****************************************
	//プレイヤー
	std::vector<std::unique_ptr<Player>> players_;

	//プレイヤー人数
	int playerNum_;

	//*****************************************
	//メンバ関数
	//*****************************************

	//プレイヤー番号ごとでモデル情報を決定する
	Transform FixTrans(int _playerNum);

	//プレイヤーカラーを設定
	void InitPlayerColor();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="playerNum">データバンクから人数を持ってくる</param>
	PlayerManager(void);
	PlayerManager(const PlayerManager& instance_) = default;
	~PlayerManager(void);
	

};

