#pragma once
#include <DxLib.h>
#include"../Common/Quaternion.h"
class SceneManager;
class ActionController;
class InputController;

class ActionBase
{
public:
	ActionBase(ActionController& _actCntl);
	virtual ~ActionBase();

	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	

	enum TYPE
	{
		NONE = -1,
		IDLE,
		WALK,
		RUN,
		JUMP,
		ATTACK,
		DAMAGE,
		DEAD,
		MAX
	};

	struct ACTION_STATUS
	{

	};
	/// <summary>
	/// 移動量の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>移動量</returns>
	inline VECTOR GetMovePow(void) { return movePow_; }

	//角度Yの取得
	const Quaternion GetPlayerRotY(void) { return playerRotY_; }

	const float GetSpeed(void) { return speed_; }

protected:
	//移動
//------------------------
	float speed_;			// 移動スピード
	VECTOR moveDir_;		// 移動方向
	VECTOR movePow_;		// 移動量
	VECTOR dir_;			//方向

	//回転
	Quaternion playerRotY_;		//プレイヤーY角度
	Quaternion goalQuaRot_;		//目的の回転
	float stepRotTime_;			//補完時間

	//ジャンプ
	//-----------------------
	bool isJump_;			// ジャンプ判定
	float stepJump_;		// ジャンプの入力受付時間
	VECTOR jumpPow_;		// ジャンプ量
	float jumpDeceralation_;	//ジャンプ減衰量


	//メンバ関数
	//入力方向に応じて方向を決める
	void MoveDirFronInput(void);
	
	//シングルトンクラス
	SceneManager& scnMng_;
	//アクションコントローラー
	ActionController& actionCntl_;

	//ゴール角度をセット
	void SetGoalRotate(const double _deg);
	//回転処理
	void Rotate(void);
	//方向と移動量の更新
	void DirAndMovePowUpdate(void);

private:
	// 回転完了までの時間
	static constexpr float TIME_ROT = 0.1f;
};

