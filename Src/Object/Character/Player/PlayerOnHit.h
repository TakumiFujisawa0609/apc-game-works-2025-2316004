#pragma once
class ActionController;
class CharacterBase;
#include"../Object/ObjectBase.h"

class PlayerOnHit
{

public:

	struct HIT_POINT
	{
		bool isDown = false;
		bool isOverHead = false;
		bool isSide = false;
	};

	/// <summary>
	/// 再生させたいSE以外すべて止める
	/// </summary>
	/// <param name="_action">アクションクラス</param>
	/// <param name="_colParam">当たり判定情報</param>
	/// <param name="_trans">モデル情報</param>
	/// <param name="_tag">当たり判定タグ</param>
	PlayerOnHit(CharacterBase& _chara,VECTOR& _movedPos,VECTOR& _moveDiff, ActionController& _action, std::vector<ObjectBase::ColParam>&_colParam,Transform& _trans,Collider::TAG _tag);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerOnHit(void);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	void Init(void);

	/// <summary>
	/// 当たった時のSEなどの読み込み
	/// </summary>
	/// <param name=""></param>
	void Load(void);

	/// <summary>
	/// 当たった処理の更新
	/// </summary>
	void OnHitUpdate(const std::weak_ptr<Collider> _hitCol);

	/// <summary>
	/// デバッグ表示
	/// </summary>
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
	static constexpr float POSITION_OFFSET = 1.0f;
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
	std::vector<ObjectBase::ColParam>& colParam_;
	Collider::TAG tag_;	//プレイヤーの当たり判定タグ
	//キャラクターの情報
	CharacterBase& charaObj_;
	//死亡判定
	bool isDeath_;
	//地面と当たっているか
	bool isLandHit_;
	//プレイヤーの頭上が当たっているか
	bool isHitOverHead_;
	//スライム床と当たっているか
	bool isHitSlimeFloor_;
	//プレイヤーの横がわが当たっている
	bool isSide_;

	//当たった箇所
	HIT_POINT hitPoint_;
	//コインの枚数
	int coinNum_;
	//バネジャンプ力
	float springJumpPow_;

	//モデルの当たった時の共通処理
	void HitModelCommon(const std::weak_ptr<Collider> _hitCol);			

	//オブジェクト毎の当たった時にされる処理
	void CollNone(void);												//ステージ
	void CollStage(void);												//当たっても何もしない(プレイヤー側で何も起きない)
	void CollChara(const std::weak_ptr<Collider> _hitCol);				//キャラクター同士
	void CollSword(const std::weak_ptr<Collider> _hitCol);				//敵の剣
};

