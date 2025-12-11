#pragma once

#include "../Object/ObjectBase.h"
#include "../Base/CharacterOnHitBase.h"
class ActionController;
class CharacterBase;

class PlayerOnHit 
	:public CharacterOnHitBase
{

public:

	/// @brief コンストラクタ
	/// @param _chara アクションクラス
	/// @param _movedPos 移動後座標
	/// @param _moveDiff 移動前座標
	/// @param _action アクションクラス
	/// @param _colParam 当たり判定情報
	/// @param _trans モデル情報
	PlayerOnHit(CharacterBase& _chara, VECTOR& _movedPos, VECTOR& _moveDiff,
		ActionController& _action, std::map<ObjectBase::TAG_PRIORITY, std::shared_ptr<Collider>>& _colParam, Transform& _trans);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerOnHit(void)override;
	
	/// @brief  初期化
	/// @param  
	void Init(void)override;
	
	/// @brief 当たった時のSEなどの読み込み
	/// @param  
	void Load(void)override;

#ifdef _DEBUG

	/// @brief デバッグ表示
	/// @param  
	void DrawDebug(void)override;
#endif // _DEBUG


private:

	//プレイヤーの大きさ
	static constexpr float RADIUS = 25.0f;
	//接地しているときのラインのコライダ
	//static constexpr int UP_AND_DOWN_LINE_COL_NO = 1;
	//プレイヤーの体の球
	static constexpr int BODY_SPHERE_COL_NO = 0;
	//移動後移動前のコライダ
	static constexpr int MOVE_LINE_COL_NO = 1;
	//上下ライン
	static constexpr int UP_AND_DOWN_LINE_COL_NO = 2;
	//雄たけび硬直時間
	static constexpr float ROAR_FLICTION_TIME = 0.1f;









	////プレイヤー
	//ActionController& action_;
	////移動量
	//VECTOR& movedPos_;
	////移動前
	//VECTOR& moveDiff_;
	////プレイヤーの情報
	//Transform& trans_;

	////当たり判定ごとの更新
	//std::map<Collider::TAG, std::function<void(const std::weak_ptr<Collider> _hitCol)>>colUpdates_;
	////当たり判定関係
	//std::map<ObjectBase::TAG_PRIORITY,std::shared_ptr<Collider>>& colParam_;
	////キャラクターの情報
	//CharacterBase& charaObj_;
	////当たった箇所
	//HIT_POINT hitPoint_;

	void CollChara(const std::weak_ptr<Collider> _hitCol)override;		//キャラ同士
	void CollNormalAttack(const std::weak_ptr<Collider> _hitCol);		//通常攻撃
	void CollRoarAttack(const std::weak_ptr<Collider>_hitCol);			//おたけび攻撃
};

