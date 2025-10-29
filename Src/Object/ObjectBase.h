#pragma once
#include<memory>
#include<vector>
#include "Common/Transform.h"
#include "../Common/IntVector3.h"
#include "./Common/Collider.h"

class ResourceManager;
class SceneManager;
class Geometry;

class ObjectBase
{
	//仮

public:

	//当たり判定情報
	struct ColParam
	{
		std::unique_ptr<Geometry> geometry_;	//形状情報
		std::shared_ptr<Collider> collider_;	//全体の当たり判定情報
	};

	// コンストラクタ
	ObjectBase(void);

	// デストラクタ
	virtual ~ObjectBase(void);

	virtual void Load(void) = 0;
	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	/// <summary>
	/// 基本情報の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline const Transform& GetTransform(void) const { return trans_; }

	/// <summary>
	/// 当たったときの処理
	/// </summary>
	/// <param name="_hitCol"></param>
	virtual void OnHit(const std::weak_ptr<Collider> _hitCol);

protected:

	// シングルトン参照
	ResourceManager& resMng_;
	SceneManager& scnMng_;

	// モデル制御の基本情報
	Transform trans_;

	//当たり判定関係
	std::vector<ColParam> colParam_;

	//タグ
	Collider::TAG tag_;

	//当たり判定をしないタグ設定
	std::set<Collider::TAG> noneHitTag_;

	/// <summary>
	/// 当たり判定作成(形状情報作成後)
	/// </summary>
	/// <param name="_tag">自身の当たり判定タグ</param>
	/// <param name="_Geometry">自身の形状情報</param>
	/// <param name="_notHitTags">衝突させないタグ</param>
	void MakeCollider(const std::set<Collider::TAG> _tag, std::unique_ptr<Geometry> _geometry, const std::set<Collider::TAG> _notHitTags = {});

	/// <summary>
	/// 特定のオブジェクトがあるかどうかを判定する
	/// </summary>
	/// <param name="_chataTag">自身のタグ</param>
	/// <param name="_tag">作りたいタグ</param>
	/// <param name="_geo">作りたい形状</param>
	const bool IsAliveCollider(const Collider::TAG _chataTag, const Collider::TAG _tag);


	//特定の配列番号の当たり判定の削除
	void DeleteCollider(const int _arrayNum);

	//全当たり判定の削除
	void DeleteAllCollider(void);

};

