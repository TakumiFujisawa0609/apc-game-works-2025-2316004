#pragma once
#include<memory>
#include<set>
#include"Geometry/Geometry.h"

class ObjectBase;
class CharacterBase;
class Collider
{

public :

	// 衝突用タグ
	enum class TAG
	{
		STAGE,			//ステージ
		PLAYER1,		//プレイヤー1
		ENEMY1,			//敵1
		NML_ATK,			//剣
		ROAR_ATK,
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_tags">自身の衝突用タグ</param>
	/// <param name="_geometry">当たり判定の形状</param>
	/// <param name="_notHitTags">衝突させないタグ</param>
	Collider(ObjectBase& _parent, const std::set<TAG> _tags, std::unique_ptr<Geometry>_geometry, const std::set<TAG> _notHitTags);

	// デストラクタ
	~Collider(void);

	//衝突用タグの取得
	inline const std::set<TAG>& GetTags(void)const { return tags_; }

	//当たり判定の形状を取得
	Geometry& GetGeometry(void)const;

	//衝突させないタグの取得
	inline const std::set<TAG>& GetNotHitTags(void)const { return notHitTags_; }

	//親を取得
	inline const ObjectBase& GetParent(void)const { return parent_; }

	//子クラスがキャラクターである場合、キャラクターベースを取得する
	CharacterBase& GetParentCharacter(void);

	//当たったかの判定の取得
	inline const bool& IsHit(void)const { return isHit_; }

	//当たっていない
	inline void NotHit(void) { isHit_ = false; }

	//終了判定の取得
	inline const bool& IsDead(void)const { return isDead_; }

	//終了処理(所持者の解放時に置く)
	inline void Kill(void) { isDead_ = true; }

	/// <summary>
	/// 当たった時の処理
	/// </summary>
	/// <param name="_collider">相手のコライダ</param>
	void OnHit(const std::weak_ptr<Collider> _collider);

private:

	//親
	ObjectBase& parent_;

	// 衝突用タグ
	std::set<TAG> tags_;

	// 衝突しないタグ
	std::set<TAG> notHitTags_;

	//当たり判定の形状
	std::unique_ptr<Geometry> geometry_;

	//当たったかの判定
	bool isHit_;

	//終了判定
	bool isDead_;
};
