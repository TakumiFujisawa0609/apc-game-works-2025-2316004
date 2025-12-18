#pragma once
#include "../Object/ObjectBase.h"
class Weapon :
    public ObjectBase
{
public:
    Weapon(void);
	~Weapon(void) override;

	/// @brief ロード
	/// @param  
	void Load(void) override;

	/// @brief 初期化
	/// @param  
	void Init(void) override;

	/// @brief 更新
	/// @param  
	void Update(void) override;

	/// @brief 描画
	/// @param  
	void Draw(void) override;

	/// @brief 追従対象セット
	/// @param _targetTrans 
	void SetTargetAndFrameNo(Transform* _targetTrans,int _frameNo);
private:
	//武器の大きさ
	static constexpr float WEAPON_SCL = 0.3f;

	//武器の追従対象
	Transform* targetTrans_;

	//追従するフレームの番号
	int followFrameNo_;

	//武器の相対位置
	VECTOR localPos_;

	//武器の相対回転
	VECTOR localRot_;



};

