#pragma once
#include "../Base/LogicBase.h"
#include"../Object/Common/Transform.h"
class CharacterBase;

class EnemyLogic :
    public LogicBase
{
public:
	EnemyLogic(Transform& _myTrans);
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

	//攻撃種類によって硬直時間をセット
	void SetFreezeCntByAttackType(void)override;
#ifdef _DEBUG
	void DebugUpdate(void);
	void DebugDraw(void)override;
#endif // _DEBUG
private:
	static constexpr float NORMAL_FREEZE_TIME = 3.0f;	//通常攻撃硬直時間
	static constexpr float JUMP_FREEZE_TIME = 0.7f;		//ジャンプ攻撃硬直時間
	static constexpr float ROAR_FREEZE_TIME = 1.0f;		//咆哮攻撃硬直時間
	static constexpr float ROLE_FREEZE_TIME = 1.5f;		//咆哮攻撃硬直時間
	//カードクールタイム制限
	static constexpr float CARD_COOL_TIME = 1.0f;	
	//攻撃の射程
	static constexpr float ATK_RANGE = 200.0f;
	//プレイヤーまでの距離詰めのクールタイム
	static constexpr float MOVE_COOL_TIME = 0.5f;

	//プレイヤーに攻撃を仕掛けるプレイヤーとの距離
	static constexpr float ATK_DISTANCE = 250.0f;

	//シーンマネージャ
	SceneManager& scnMng_;

	//カード使用クールタイム
	float cardCoolCnt_;

	//移動時間
	float moveCnt_;



	//攻撃の重み付け
	AttackWeight weight_;

	//硬直時間
	float freezeCnt_;
	//攻撃の届く範囲
	float atkRange_;
	
	//前回の攻撃種類
	ENEMY_ATTACK_TYPE prevAttackType_;
	//攻撃を決める
	void DecideAction(void);

};

