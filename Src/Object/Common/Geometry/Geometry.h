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

	//バウンディングボックス
	struct OBB
	{
		VECTOR vMin;
		VECTOR vMax;
		VECTOR axis[3];
	};

	//通常色
	static constexpr int NORMAL_COLOR = 0x000000;

	//デストラクタ
	virtual~Geometry(void) = 0;

	//描画
	virtual void Draw(void) = 0;

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


	//1つ目の点の座標を取得
	inline const VECTOR GetLocalPosPoint1(void)const { return localPosPoint1_; }
	//2つ目の点の座標を取得
	inline const VECTOR GetLocalPosPoint2(void)const { return localPosPoint2_; }

	// 回転済みの1つ目の点の座標を取得
	inline const VECTOR GetPosPoint1(void) const { return GetRotPos(localPosPoint1_); }
	// 回転済みの2つ目の点の座標を取得
	inline const VECTOR GetPosPoint2(void) const { return GetRotPos(localPosPoint2_); }

	//中心座標の取得
	const VECTOR GetCenter(void)const;

	//1つ目の点の座標の設定
	inline void SetLocalPosPoint1(const VECTOR _pos) { localPosPoint1_ = _pos; }
	//2つ目の点の座標の設定
	inline void SetLocalPosPoint2(const VECTOR _pos) { localPosPoint2_ = _pos; }

	//半径の取得
	inline const float GetRadius(void)const { return radius_; }
	//半径の設定
	inline void SetRadius(const float _radius) { radius_ = _radius; }

	//当たった時の情報取得
	inline const MV1_COLL_RESULT_POLY& GetHitLineInfo(void)const { return hitLineInfo_; }
	//当たった時の情報取得
	inline const MV1_COLL_RESULT_POLY_DIM& GetHitModelInfo(void)const { return hitInfo_; }
	//親モデルIDの取得
	inline const int GetParentModel(void)const { return parentModelId_; }

	//当たった時の情報設定
	inline void SetHitLineInfo(const MV1_COLL_RESULT_POLY _hitLineInfo) { hitLineInfo_ = _hitLineInfo; }
	//当たった時の情報設定
	inline void SetHitInfo(MV1_COLL_RESULT_POLY_DIM _hitInfo) { std::swap(hitInfo_, _hitInfo); }

	//MV1_COLL_RESULT_POLY

	//回転バウンティボックスの取得
	inline const OBB& GetObb(void)const { return obb_; }
	//箱の最小地点の取得
	inline const VECTOR GetVecMin(void)const { return obb_.vMin; }
	//箱の最大地点の取得
	inline const VECTOR GetVecMax(void)const { return obb_.vMax; }
	//回転バウンティボックスの設定
	inline void SetObb(const OBB& _obb) { obb_ = _obb; }
	//箱の最小地点の設定
	inline void SetVecMin(const VECTOR& _min) { obb_.vMin = _min; }
	//箱の最大地点の設定
	inline void SetVecMax(const VECTOR& _max) { obb_.vMax = _max; }
	//サイズの半分の設定
	inline void SetHalfSize(const VECTOR& _halfSize);




protected:

	/// <summary>
	/// コンストラクタ(外部で作る必要のない基底なのでprotected)
	/// </summary>
	/// <param name="_pos">追従する親の座標</param>
	/// <param name="_rot">追従する親の回転</param>
	Geometry(const VECTOR& _pos, const Quaternion& _rot, const float& _radius,
		const VECTOR& _localPosPoint1, const VECTOR& _localPosPoint2,
		const OBB& _obb, const int& _parentModelId);

	// 相対座標を回転させてワールド座標で取得する
	const VECTOR GetRotPos(const VECTOR& _localPos) const;

	const VECTOR& pos_;			//親の座標
	const Quaternion& quaRot_;	//親の回転

	float radius_;			// 半径
	MV1_COLL_RESULT_POLY hitLineInfo_;	//当たった時の情報(線)
	MV1_COLL_RESULT_POLY_DIM hitInfo_;	//当たった時の情報(球、カプセル)

	//Line
	VECTOR localPosPoint1_;	//1つ目の点の座標
	VECTOR localPosPoint2_;	//2つ目の点の座標


	//Cube
	OBB obb_;			//回転バウンディングボックス

	//Model
	int parentModelId_;					//親のモデルID
};