#pragma once
#include "../Common/Vector2.h"
#include "../Common/Vector2F.h"
class Geometry2D
{
public:
	struct RectObb
	{
		Vector2F center;    // 中心座標
		Vector2F halfSize;  // 幅と高さの半分（半径）
		float angle;      // 回転角度（ラジアン）

		// 各軸ベクトル（角度から算出）
		Vector2F GetAxisX() const
		{
			return { cosf(angle), sinf(angle) };
		}
		Vector2F GetAxisY() const
		{
			return { -sinf(angle), cosf(angle) };
		}
	};
	//デストラクタ
	virtual~Geometry2D(void) = 0;
	//描画
	virtual void Draw(void) = 0;
	//各種当たり判定
	virtual const bool IsHit(Geometry2D& _geometry) = 0;
	virtual const bool IsHit(class Rect2D& _rect) = 0;
	virtual const bool IsHit(class Circle2D& _circle) = 0;
	//ヒット後の処理
	virtual void HitAfter(void);
	//親情報を返す
	inline const Vector2F& GetColPos(void)const { return pos_; }
	//inline const float GetColRot(void)const { return rot_; }
protected:
	const Vector2F& pos_;			//親の座標

};

