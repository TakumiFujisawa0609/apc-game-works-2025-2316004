#pragma once
#include <DxLib.h>
#include"../Common/Quaternion.h"
class SceneManager;
class ActionController;
class AnimationController;
class PlayerLogic;

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
	/// 移動量の取得(ベクトル)
	/// </summary>
	/// <param name=""></param>
	/// <returns>移動量</returns>
	inline VECTOR GetMovePow(void) { return movePow_; }

	/// <summary>
	/// 1方向に動く移動量
	/// </summary>
	/// <param name=""></param>
	/// <returns>1方向に動く移動量</returns>
	const float GetSpeed(void) { return speed_; }

	/// <summary>
	/// 方向転換可能かどうかの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>方向転換可能かどうか</returns>
	const bool GetIsTurnable(void) { return isTurnable_; }

protected:
	//移動
//------------------------
	float speed_;			// 移動スピード
	VECTOR moveDir_;		// 移動方向
	VECTOR movePow_;		// 移動量
	VECTOR dir_;			//方向

	//ジャンプ
	//-----------------------
	bool isJump_;			// ジャンプ判定
	float stepJump_;		// ジャンプの入力受付時間
	VECTOR jumpPow_;		// ジャンプ量
	float jumpDeceralation_;	//ジャンプ減衰量

	//シングルトンクラス
	SceneManager& scnMng_;
	//アクションコントローラー
	ActionController& actionCntl_;
	//アニメーション
	AnimationController& anim_;

	//方向転換可能かどうか
	bool isTurnable_;

private:
	// 回転完了までの時間
	static constexpr float TIME_ROT = 0.1f;
};

