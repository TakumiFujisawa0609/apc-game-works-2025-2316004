#pragma once
#include<memory>
#include<map>
#include "Common/Transform.h"
#include "../Common/IntVector3.h"
#include "./Common/Collider.h"

class ResourceManager;
class SceneManager;
class Geometry;

class ObjectBase
{

public:

	//タグ優先順位
	enum class  TAG_PRIORITY
	{
		BODY,
		MOVE_LINE,
		UPDOWN_LINE,
		ATK_SPHERE
	};

	// コンストラクタ
	ObjectBase(void);

	// デストラクタ
	virtual ~ObjectBase(void);

	virtual void Load(void) = 0;
	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	
	/// @brief 基本情報の取得
	/// @param  
	/// @return 
	inline const Transform& GetTransform(void) const { return trans_; }
	
	/// @brief 当たったときの処理
	/// @param _hitCol ヒットしたコライダ
	virtual void OnHit(const std::weak_ptr<Collider> _hitCol);

protected:

	// シングルトン参照
	ResourceManager& resMng_;
	SceneManager& scnMng_;

	// モデル制御の基本情報
	Transform trans_;

	//当たり判定関係
	//std::vector<ColParam> colParam_;
	std::map<TAG_PRIORITY, std::shared_ptr<Collider>> collider_;	//全体の当たり判定情報

	//タグ
	Collider::TAG tag_;

	//当たり判定をしないタグ設定
	std::set<Collider::TAG> noneHitTag_;

	//初めのタグ順格納
	std::vector<TAG_PRIORITY>tagPrioritys_;
	
	/// @brief 当たり判定作成(形状情報作成後)
	/// @param _tag 自身の当たり判定タグ
	/// @param _geometry 自身の形状情報
	/// @param _notHitTags 衝突させないタグ
	void MakeCollider(const TAG_PRIORITY _tagPriority,const std::set<Collider::TAG> _tag, std::unique_ptr<Geometry> _geometry, const std::set<Collider::TAG> _notHitTags = {});

	/// @brief 特定のオブジェクトがあるかどうかを判定する
	/// @param _chataTag 自身のタグ
	/// @param _tag 作りたいタグ
	/// @return turue:特定のオブジェクトが存在する	false存在しない
	const bool IsAliveCollider(const Collider::TAG _chataTag, const Collider::TAG _tag);


	//特定の配列番号の当たり判定の削除
	void DeleteCollider(const TAG_PRIORITY _priority);

	//全当たり判定の削除
	void DeleteAllCollider(void);

};

