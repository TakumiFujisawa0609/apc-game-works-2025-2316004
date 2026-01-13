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

	virtual void Load(void) = 0;

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

	//カードアクション種類
	enum class CARD_ACT_TYPE
	{
		NONE = -1,
		//プレイヤーのアクション
		ATTACK_ONE_SHORT,	//攻撃アクション1回目(近距離)
		ATTACK_ONE_MIDDLE,	//攻撃アクション1回目(中距離)
		ATTACK_TWO,			//攻撃アクション2回目
		ATTACK_THREE,		//攻撃アクション3回目
		DUEL_FAZE,			//敵のジャンプチャージフェーズの時、カード勝負行動に出る
		DUEL_WAIT_FAZE,		//デュエルのカード待機
		HEAL_CARD,			//ヒールカード使用
		MAGIC_CARD,			//マジックカード使用
		SONIC_RAVE,			//ソニックレイヴ		

		//敵のアクション
		STOMP_ATK,			//ひっかき攻撃
		ROAR_ATK,			//咆哮
		JUMP_ATK,			//ジャンプ攻撃
		RUSH_ATK,			//ロール攻撃

		//共通
		RELOAD, //リロード
		NUM,
	};

	//攻撃のステータス
	struct ATK_STATUS
	{
		float colStartCnt=0.0f;
		float colEndCnt=0.0f;
		float atkRadius=0.0f;
		float atkPoint = 0.0f;		//攻撃力
		VECTOR pos = {};
		bool isDamage = true;
	};
	
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

	/// @brief のけぞりカウントのセット
	/// @param _flinchTime のけぞりカウント
	void SetFlinchCnt(const float _flinchTime) { flinchCnt_ = _flinchTime; }

	/// @brief 攻撃ステータスの取得
	/// @param  
	/// @return 攻撃ステータス
	const ATK_STATUS& GetAtkStatus(void)const { return atk_; }

	//カードアクションの取得
	const CARD_ACT_TYPE& GetCardAction(void)const;

	/// @brief ジャンプ攻撃チャージ
	/// @param  
	/// @return 
	virtual const bool IsJumpAtkCharge(void)const { return false; }

	/// @brief ジャンプチャージのカード勝利数
	/// @param  
	/// @return 
	virtual const int GetJumpCardNum(void)const { return -1; }

	/// @brief 攻撃がダメージを与えたかを返す
	/// @param  
	const bool GetIsDamage(void)const { return atk_.isDamage; }

	/// @brief ダメージを与えたことをセット
	/// @param  
	void SetIsDamage(void) { atk_.isDamage = true; }
protected:

	//移動スピード
	static constexpr float MOVE_SPEED = 6.0f;

	//移動
	//------------------------
	float speed_;			// 移動スピード

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
	//敵への方向転換
	bool isTargetTurn_;
	//攻撃の当たり判定中か
	bool isAliveAtkCol_;
	//状態
	CARD_ACT_TYPE actType_;

	//現在の攻撃
	ATK_STATUS atk_;

private:
	// 回転完了までの時間
	static constexpr float TIME_ROT = 0.1f;
};

