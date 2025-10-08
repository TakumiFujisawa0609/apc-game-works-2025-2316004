#pragma once
#include "../Base/LogicBase.h"
#include"../Object/Common/Transform.h"
class CharacterBase;

class EnemyLogic :
    public LogicBase
{
public:
	EnemyLogic(CharacterBase& _playerChara, Transform& _myTrans);
	~EnemyLogic(void) override;

	void Init(void) override;
	void Update(void) override;
private:
	//カードクールタイム制限
	static constexpr float CARD_COOL_TIME = 0.2f;
	
	//シーンマネージャ
	SceneManager& scnMng_;
	//プレイヤー参照
	CharacterBase& playerChara_;
	//モデル情報
	Transform& myTrans_;

	//カード使用クールタイム
	float cardCoolCnt_;
};

