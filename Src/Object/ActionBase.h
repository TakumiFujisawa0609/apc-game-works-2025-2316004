#pragma once
#include <DxLib.h>
#include"../Common/Quaternion.h"
class ActionBase
{
public:
	ActionBase(void);
	virtual ~ActionBase();

	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	
	void ChangeAction(ActionBase* action);

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
private:

};

