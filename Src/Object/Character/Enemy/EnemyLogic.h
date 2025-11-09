#pragma once
#include "../Base/LogicBase.h"
#include"../Object/Common/Transform.h"
class CharacterBase;

class EnemyLogic :
    public LogicBase
{
public:
	EnemyLogic(Transform& _myTrans, CharacterBase& _playerChara);
	~EnemyLogic(void) override;

	struct AttackWeight {
		int normal = 60;
		int jump = 30;
		int Roar = 20;
		int role = 10;
	};


	/// @brief 初期化
	/// @param  
	void Init(void) override;

	/// @brief 更新
	/// @param  
	void Update(void) override;

	/// @brief 相手の方向を見る
	/// @param  
	const VECTOR GetLookAtTargetDir(void)const override;
	const float GetLookAtTargetDeg(void)const override;

#ifdef _DEBUG
	void DebugUpdate(void);
	void DebugDraw(void)override;
#endif // _DEBUG
private:
	static constexpr float NORMAL_FREEZE_TIME = 0.3f;	//通常攻撃硬直時間
	static constexpr float JUMP_FREEZE_TIME = 0.7f;		//ジャンプ攻撃硬直時間
	static constexpr float ROAR_FREEZE_TIME = 1.0f;		//咆哮攻撃硬直時間
	static constexpr float ROLE_FREEZE_TIME = 1.5f;		//咆哮攻撃硬直時間
	//カードクールタイム制限
	static constexpr float CARD_COOL_TIME = 1.0f;	
	//攻撃の射程
	static constexpr float ATK_RANGE = 200.0f;
	//プレイヤーまでの距離詰めのクールタイム
	static constexpr float MOVE_COOL_TIME = 0.5f;

	//角度関連
	static constexpr float FLONT_DEG = 0.0f;
	static constexpr float LEFT_DEG = 270.0f;
	static constexpr float BACK_DEG = 180.0f;
	static constexpr float RIGHT_DEG = 90.0f;

	//自身のモデル情報
	Transform& myTrans_;

	//シーンマネージャ
	SceneManager& scnMng_;
	//プレイヤー参照
	CharacterBase& playerChara_;
	//カード使用クールタイム
	float cardCoolCnt_;

	//移動時間
	float moveCnt_;

	//プレイヤーと敵の距離
	float distance_;

	//攻撃の重み付け
	AttackWeight weight_;

	//硬直時間
	float freezeCnt_;
	//攻撃の届く範囲
	float atkRange_;

	//攻撃を決める
	void DesideAction(void);
	//攻撃種類によって硬直時間をセット
	void SetFreezeCntByAttackType(const ENEMY_ATTACK_TYPE& _type);
};

