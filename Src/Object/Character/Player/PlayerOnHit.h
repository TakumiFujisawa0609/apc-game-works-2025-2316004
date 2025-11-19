#pragma once
class ActionController;
class CharacterBase;
#include"../Object/ObjectBase.h"

class PlayerOnHit :public ObjectBase
{

public:

	struct HIT_POINT
	{
		bool isDown = false;
		bool isOverHead = false;
		bool isSide = false;
	};

	
	/// @brief コンストラクタ
	/// @param _chara アクションクラス
	/// @param _movedPos 移動後座標
	/// @param _moveDiff 移動前座標
	/// @param _action アクションクラス
	/// @param _colParam 当たり判定情報
	/// @param _trans モデル情報
	/// @param _tag 当たり判定タグ
	PlayerOnHit(CharacterBase& _chara, VECTOR& _movedPos, VECTOR& _moveDiff,
		ActionController& _action, std::map<ObjectBase::TAG_PRIORITY, std::shared_ptr<Collider>>& _colParam, Transform& _trans, Collider::TAG _tag);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerOnHit(void) override;
	
	/// @brief 当たった時のSEなどの読み込み
	/// @param  
	void Load(void) override;

	/// @brief  初期化
	/// @param  
	void Init(void) override;
	
	/// @brief 更新(つかわない)
	/// @param  
	void Update(void) override {};

	/// @brief 描画
	/// @param  
	void Draw(void) override {}
	

	/// @brief 当たった処理の更新
	/// @param _hitCol 
	void OnHitUpdate(const std::weak_ptr<Collider> _hitCol);

	/// @brief 当たった箇所の取得
	/// @param  
	/// @return 
	const HIT_POINT& GetHitPoint(void)const { return hitPoint_; }

	const bool& GetIsHitAtk(void)const { return isHitAtk_; }

	/// @brief 攻撃生成時用の当たった判定初期化
	/// @param  
	void InitIsHitAtk(void) { isHitAtk_ = false; }

	/// @brief 当たり判定をする前の初期化
	/// @param  
	void InitHitPoint(void) { hitPoint_ = {}; }

	/// @brief デバッグ表示
	/// @param  
	void DrawDebug(void);

private:

	//プレイヤーの大きさ
	static constexpr float RADIUS = 25.0f;
	//当たり判定の押し出し回数
	static constexpr int COL_TRY_CNT_MAX = 10;
	//ヒットした法線方向へのオフセット
	static constexpr float HIT_NORMAL_OFFSET = 3.0f;
	//接地しているときのラインのコライダ
	//static constexpr int UP_AND_DOWN_LINE_COL_NO = 1;
	//プレイヤーの体の球
	static constexpr int BODY_SPHERE_COL_NO = 0;
	//移動後移動前のコライダ
	static constexpr int MOVE_LINE_COL_NO = 1;
	//上下ライン
	static constexpr int UP_AND_DOWN_LINE_COL_NO = 2;
	//押し出す移動量
	static constexpr float POSITION_OFFSET = 0.1f;
	//雄たけび硬直時間
	static constexpr float ROAR_FLICTION_TIME = 0.1f;
	//プレイヤー
	ActionController& action_;
	//移動量
	VECTOR& movedPos_;
	//移動前
	VECTOR& moveDiff_;
	//プレイヤーの情報
	Transform& trans_;

	//当たり判定ごとの更新
	std::map<Collider::TAG, std::function<void(const std::weak_ptr<Collider> _hitCol)>>colUpdates_;
	//当たり判定関係
	std::map<ObjectBase::TAG_PRIORITY,std::shared_ptr<Collider>>& colParam_;
	Collider::TAG tag_;	//プレイヤーの当たり判定タグ
	//キャラクターの情報
	CharacterBase& charaObj_;

	//攻撃が当たったことを伝える
	bool isHitAtk_;

	//当たった箇所
	HIT_POINT hitPoint_;
	//コインの枚数
	int coinNum_;
	//バネジャンプ力
	float springJumpPow_;

	//モデルの当たった時の共通処理
	void HitModelCommon(const std::weak_ptr<Collider> _hitCol);			

	//オブジェクト毎の当たった時にされる処理
	void CollNone(void);												//当たっても何もしない(プレイヤー側で何も起きない)
	void CollStage(const std::weak_ptr<Collider> _hitCol);				//ステージ
	void CollChara(const std::weak_ptr<Collider> _hitCol);				//キャラクター同士
	void CollNormalAttack(const std::weak_ptr<Collider> _hitCol);		//通常攻撃
	void CollRoarAttack(const std::weak_ptr<Collider>_hitCol);			//おたけび攻撃
};

