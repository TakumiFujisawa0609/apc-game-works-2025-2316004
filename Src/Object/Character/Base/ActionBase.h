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

	/// @brief 初期化
	/// @param  
	virtual void Init(void) = 0;

	/// @brief 更新
	/// @param  
	virtual void Update(void) = 0;
	
	/// @brief 解放
	/// @param  
	virtual void Release(void);

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

	struct ATK_STATUS
	{
		float colRadius;
		float colStartFrame;
		float colEndFrame;
	};

	struct ACTION_STATUS
	{

	};
	
	/// @brief 移動量の取得(ベクトル)
	/// @param 
	/// @return 移動量
	inline VECTOR& GetMovePow(void) { return movePow_; }
	
	/// @brief 1方向に動く移動量
	/// @param  
	/// @return 1方向に動く移動量
	const float& GetSpeed(void) { return speed_; }

	
	/// @brief 方向転換可能かどうかの取得
	/// @param  
	/// @return 方向転換可能かどうか
	const bool& GetIsTurnable(void) { return isTurnable_; }
	
	/// @brief 攻撃当たり判定中かの取得
	/// @param  
	/// @return 
	const bool& GetIsAliveAtkCol(void) { return isAliveAtkCol_; }

	/// @brief ジャンプ力の取得
	/// @param  
	/// @return ジャンプ力
	const VECTOR& GetJumpPow(void)const { return jumpPow_; }

protected:

	//移動スピード
	static constexpr float MOVE_SPEED = 6.0f;

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
	//攻撃の当たり判定中か
	bool isAliveAtkCol_;

private:
	// 回転完了までの時間
	static constexpr float TIME_ROT = 0.1f;
};

