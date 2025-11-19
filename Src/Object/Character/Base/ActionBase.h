#pragma once
#include <DxLib.h>

#include "../Object/ObjectBase.h"

#include"../Common/Quaternion.h"
class SceneManager;
class ActionController;
class AnimationController;
class PlayerLogic;

class ActionBase:
	public ObjectBase
{
public:
	ActionBase(ActionController& _actCntl);
	virtual ~ActionBase();

	/// @brief ロード
	/// @param  
	virtual void Load(void) override {};

	/// @brief 初期化
	/// @param  
	virtual void Init(void) override = 0;

	/// @brief 更新
	/// @param  
	virtual void Update(void) override = 0;

	/// @brief 描画
	/// @param
	virtual void Draw(void) override {};
	
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

	//カードアクション種類
	enum class CARD_ACT_TYPE
	{
		NONE = -1,
		//プレイヤーのアクション
		ATTACK_ONE, //攻撃アクション1回目
		ATTACK_TWO, //攻撃アクション2回目
		ATTACK_THREE, //攻撃アクション3回目
		HEAL_CARD,       //ヒールカード使用
		MAGIC_CARD,        //マジックカード使用

		//敵のアクション
		SWIP_ATK, //ひっかき攻撃
		ROAR_ATK, //咆哮
		JUMP_ATK, //ジャンプ攻撃
		RUSH_ATK,  //ロール攻撃

		//共通
		RELOAD, //リロード
		NUM,
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
	const float&GetSpeed(void) { return speed_; }

	
	/// @brief 方向転換可能かどうかの取得
	/// @param  
	/// @return 方向転換可能かどうか
	const bool& GetIsTurnable(void) { return isTurnable_; }
	
	/// @brief 攻撃当たり判定中かの取得
	/// @param  
	/// @return 
	const bool& GetIsAliveAtkCol(void) { return isAliveAtkCol_; }

	/// @brief のけぞりカウントのセット
	/// @param _flinchTime のけぞりカウント
	void SetFlinchCnt(const float _flinchTime) { flinchCnt_ = _flinchTime; }

	//カードアクションの取得
	const CARD_ACT_TYPE& GetCardAction(void)const;

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
	//VECTOR jumpPow_;		// ジャンプ量
	float jumpDeceralation_;	//ジャンプ減衰量

	//怯み時間
	float flinchCnt_;

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
	//状態
	CARD_ACT_TYPE actType_;

private:
	// 回転完了までの時間
	static constexpr float TIME_ROT = 0.1f;
};

