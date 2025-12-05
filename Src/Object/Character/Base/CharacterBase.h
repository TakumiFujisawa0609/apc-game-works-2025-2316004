#pragma once
#include<map>
#include "../../Card/CardBase.h"
#include "../Player/PlayerOnHit.h"
#include "../Player/ActionController.h"
#include "../Base/CardActionBase.h"
#include "../Base/ActionBase.h"
#include "../Object/ObjectBase.h"

class AnimationController;
class ActionController;
class CardDeck;
class CardUIBase;
class InputBase;
class Capsule;
class LogicBase;
class PlayerOnHit;



class CharacterBase :public ObjectBase
{
public:

	//アニメーション速度
	static constexpr float ANIM_SPEED = 40.0f;

	// 回転完了までの時間
	static constexpr float TIME_ROT = 0.1f;
	//最大ＨＰ
	static constexpr float HP_MAX = 200.0f;
	//転がるアニメーション速度
	static constexpr float ROLL_ANIM_SPEED = 100.0f;

	//当たり判定
	static constexpr int CUPSULE_COL_NO = 0;
	static constexpr int MOVE_LINE_COL_NO = 1;
	static constexpr int UP_DOWN_LINE = 2;
	static constexpr int ATK_COL_NO = 3;

	// アニメーション種別
	enum class ANIM_TYPE
	{
		NONE,
		IDLE,
		RUN,
		REACT,
		ATTACK_1,
		ATTACK_2,
		ATTACK_3,
		JUMP,
		DODGE,
		//敵
		SWIP_ATK,
		ROAR_ATK,
		JUMP_ATK,
		RUSH_ATK

	};

	struct ROTATION
	{
		//回転
		Quaternion playerRotY_;		//プレイヤーY角度
		Quaternion goalQuaRot_;		//目的の回転
		float stepRotTime_;			//補完時間
		VECTOR dir_;			//方向
	};

	//各ステータス
	struct STATUS
	{
		float speed;		//移動速度
		float hp = HP_MAX;	//体力
		float atk;			//攻撃力
		float def;			//防御力
	};



	enum class ACTION_TYPE
	{
		IDLE,		//何もしてない
		MOVE,		//移動
		DASHMOVE,	//ダッシュ
		REACT,	//パンチされた状態
		JUMP,		//ジャンプ
		CARD_ACTION	//カードアクション
	};

	/// @brief コンストラクタ
	/// @param  
	CharacterBase(void);



	/// @brief デストラクタ
	/// @param  
	virtual ~CharacterBase(void)override = 0;

	/// @brief 読み込み
	/// @param  
	virtual void Load(void)override = 0;

	/// @brief 初期化
	/// @param  
	virtual void Init(void)override = 0;

	/// @brief 更新
	/// @param  
	virtual void Update(void)override = 0;

	/// @brief 描画
	/// @param  
	virtual void Draw(void)override = 0;

	/// @brief 当たった時の処理
	/// @param _hitCol 
	virtual void OnHit(const std::weak_ptr<Collider> _hitCol)override = 0;
	
	/// @brief 攻撃の当たり判定生成
	/// @param _charaTag 自身のタグ
	/// @param _atkPos 作りたい攻撃の座標
	/// @param _radius 当たり判定の半径
	void MakeAttackCol(const Collider::TAG _charaTag, const Collider::TAG _attackTag,const VECTOR& _atkPos,const float& _radius);

	/// @brief 攻撃の当たり判定削除
	/// @param _charaTag 自身のタグ
	void DeleteAttackCol(const Collider::TAG& _charaTag, const Collider::TAG& _attackCol);
	
	/// @brief キャラタグの取得
	/// @param  
	/// @return 
	const Collider::TAG& GetCharaTag(void)const { return tag_; }
	
	/// @brief 入力方向に応じて方向を決める
	/// @param  
	virtual void MoveDirFromInput(void);

	/// @brief ゴール角度をセット
	/// @param _deg 
	virtual void SetGoalRotate(const double _deg);
	
	/// @brief 方向関連の変数の取得
	/// @param  
	/// @return 方向関連の変数
	inline const ROTATION& GetRotation(void)const { return charaRot_; }

	/// @brief 当たり判定の大きさの更新
	/// @param _radius 半径
	void UpdateAttackCol(const float _radius);

	/// @brief 角度更新
	/// @param  
	void Rotate(void);

	/// @brief ダメージ処理
	/// @param _dam ダメージ数
	void Damage(const int _dam);

	/// @brief ダメージを食らったか
	/// @param  
	/// @return 
	const bool& GetIsDamage(void)const { return isDamage_; }

	/// @brief 着地時のジャンプ力初期化
	/// @param  
	void JumpPowZero(void) { jumpPow_ = Utility3D::VECTOR_ZERO; }

	/// @brief キャラクターの中心座標取得
	/// @param  
	/// @return キャラクターの中心座標
	const VECTOR& GetCharaCenterPos(void)const;

	/// @brief ダメージを与えたことを知らせる
	/// @param  
	void SetIsDamage(void) { isDamage_ = true; }

	/// @brief のけぞり時間のセット
	/// @param _flichCnt のけぞり時間
	void SetFlinchCnt(const float _flichCnt);

	/// @brief ステータス取得
	/// @param  
	/// @return 
	inline const STATUS& GetStatus(void)const { return status_; }

	/// @brief カードUI
	/// @param  
	/// @return 
	inline CardUIBase& GetCardUI(void)const { return *cardUI_; }

	/// @brief カードアクションの取得
	/// @param  
	/// @return 
	const CardActionBase::CARD_ACT_TYPE& GetCardAction(void)const;

	/// @brief 敵のジャンプチャージ中取得
	/// @param  
	/// @return 
	const ActionBase& GetMainAction(void)const;
	
	/// @brief　使用済みカードについての処理
	/// @param  
	void DeleteCard(void);

	/// @brief 移動後座標の取得
	/// @param  
	/// @return 移動後座標
	const VECTOR& GetMovedPos(void)const { return movedPos_; }

	/// @brief ヒットポイントを当たり判定から取得
	/// @param  
	/// @return 
	const PlayerOnHit::HIT_POINT& GetHitPoint(void)const;

	/// @brief 移動量を設定して動かす
	/// @param _movePow 移動量
	void MovedPosMove(const VECTOR& _vec,const float& _movePow);

	/// @brief 
	/// @param _dir 
	/// @param _movePow 
	void LariatMove(const float& _deg);

	/// @brief ロジッククラスにターゲットをセット
	/// @param _targetChara ターゲット
	void SetLogicTargetCharacter(std::shared_ptr<CharacterBase>_targetChara);
protected:



	//移動量ラインオフセット
	static constexpr float MOVE_LINE_Y_OFFSET = - 1.0f;
	//移動量更新条件の移動ラインの長さ
	static constexpr float MOVE_LINE_Y_CHECK_VALUE =  1.5f;
	//リロードカードステータス
	static constexpr CardBase::CARD_STATUS RELOAD_CARD_STATUS = { -1,CardBase::CARD_TYPE::RELOAD };



	//入力
	std::unique_ptr<LogicBase>logic_;
	//行動系
	std::unique_ptr<ActionController>action_;
	// アニメーション
	std::unique_ptr<AnimationController>animationController_;
	//デッキ
	std::shared_ptr<CardDeck>deck_;
	//当たり判定用のカプセル
	std::unique_ptr<Capsule>cap_;
	//当たった時の処理
	std::unique_ptr<PlayerOnHit>onHit_;

	//当たり判定の要素
	VECTOR movedPos_;		//移動後座標
	VECTOR moveDiff_;		//移動前座標

	VECTOR jumpPow_;		// ジャンプ量
	VECTOR movePow_;		// 移動量
	
	//角度関連
	ROTATION charaRot_;
	//ステータス
	STATUS status_;
	
	//ステータス
	STATUS maxStatus_;
	//カードUI(とりあえず)
	std::unique_ptr<CardUIBase>cardUI_;
	//攻撃によってダメージを与えたか(与えたら判定を抜ける)
	bool isDamage_;

	////カード関連初期化
	//void CardInit(void);

	//カプセル半径
	float capRadius_;

	//移動後座標などの更新
	void UpdatePost(void);
	//ステータスの設定
	void SetStatus(const float& _spd, const float& _hp, const float& _atk, const float& _def);

	//移動制限
	void MoveLimit(const VECTOR& _stagePos, const VECTOR& _stageSize);
private:

};

