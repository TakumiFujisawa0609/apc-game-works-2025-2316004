#pragma once
#include "../Base/CharacterOnHitBase.h"
class EnemyOnHit :
    public CharacterOnHitBase
{
public:

	/// @brief コンストラクタ
	/// @param _chara アクションクラス
	/// @param _movedPos 移動後座標
	/// @param _moveDiff 移動前座標
	/// @param _action アクションクラス
	/// @param _colParam 当たり判定情報
	/// @param _trans モデル情報
	EnemyOnHit(CharacterBase& _chara, VECTOR& _movedPos, VECTOR& _movedDiff,
		ActionController& _action, std::map<ObjectBase::TAG_PRIORITY, std::shared_ptr<Collider>>& _colParam, Transform& _trans);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyOnHit(void)override;

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
	void CollNormalAttack(const std::weak_ptr<Collider> _hitCol);		//通常攻撃
	void CollDuelSpecialAttack(const std::weak_ptr<Collider> _hitCol);	//デュエルで負けたときの反応
};

