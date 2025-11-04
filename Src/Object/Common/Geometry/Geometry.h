#pragma once

#include<DxLib.h>
#include"../../../Common/Quaternion.h"

class Model;
class Cube;
class Sphere;
class Capsule;
class Line;

class Geometry
{
public:

	//通常色
	static constexpr int NORMAL_COLOR = 0x000000;

	//デストラクタ
	virtual~Geometry(void) = 0;

	//描画
	virtual void Draw(void) = 0;

	struct CAPSULE_INFO
	{
		VECTOR localPosTop_;	// 親Transformからの相対位置(上側)
		VECTOR localPosDown_;	// 親Transformからの相対位置(下側)
		float radius_;			// 半径

		MV1_COLL_RESULT_POLY_DIM hitInfo_;	//当たった時の情報(球、カプセル)
	};

	struct LINE_INFO
	{
		VECTOR localPosPoint1_;	//1つ目の点の座標
		VECTOR localPosPoint2_;	//2つ目の点の座標

		MV1_COLL_RESULT_POLY hitInfo_;	//当たった時の情報(モデル)
	};

	struct SPHERE_INFO
	{
		float radius_;						//半径
		MV1_COLL_RESULT_POLY_DIM hitInfo_;	//当たった時の情報(モデル)
	};

	struct CUBE_INFO
	{
		//バウンディングボックス
		struct OBB
		{
			VECTOR vMin;
			VECTOR vMax;
			VECTOR axis[3];
		};
		OBB obb_;			//回転バウンディングボックス
	};

	struct MODEL_INFO
	{
		int parentModelId_;					//親のモデルID
		MV1_COLL_RESULT_POLY hitLineInfo_;	//当たった時の情報(線)
		MV1_COLL_RESULT_POLY_DIM hitInfo_;	//当たった時の情報(球、カプセル)
	};

	//各種当たり判定
	virtual const bool IsHit(Geometry& _geometry) = 0;
	virtual const bool IsHit(Model& _model) = 0;
	virtual const bool IsHit(Cube& _cube) = 0;
	virtual const bool IsHit(Sphere& _sphere) = 0;
	virtual const bool IsHit(Capsule& _capsule) = 0;
	virtual const bool IsHit(Line& _line) = 0;
	
	//ヒット後の処理
	virtual void HitAfter(void);

	//親情報を返す
	//inline const VECTOR& GetColPos(void)const { return pos_; }
	inline const VECTOR GetColPos(void)const { return pos_; }
	inline const Quaternion& GetColRot(void)const { return quaRot_; }

protected:

	/// <summary>
	/// コンストラクタ(外部で作る必要のない基底なのでprotected)
	/// </summary>
	/// <param name="_pos">追従する親の座標</param>
	/// <param name="_rot">追従する親の回転</param>
	Geometry(const VECTOR& _pos, const Quaternion& _rot);

	// 相対座標を回転させてワールド座標で取得する
	const VECTOR GetRotPos(const VECTOR& _localPos) const;

	const VECTOR& pos_;			//親の座標
	const Quaternion& quaRot_;	//親の回転
};