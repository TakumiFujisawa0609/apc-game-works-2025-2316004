#pragma once
#include <DxLib.h>
#include "../Common/Vector2.h"
#include "../Renderer/PixelMaterial.h"

class PixelRenderer
{

public:

	// ピクセルシェーダ用オリジナル定数バッファの使用開始スロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;

	// 頂点数
	static constexpr int NUM_VERTEX = 4;

	// 頂点インデックス数
	static constexpr int NUM_VERTEX_IDX = 6;

	// ポリゴン数
	static constexpr int NUM_POLYGON = 2;

	// コンストラクタ
	PixelRenderer(PixelMaterial& pixelMaterial);

	// デストラクタ
	~PixelRenderer(void);

	// 描画矩形の生成
	void MakeSquareVertex(Vector2F pos, Vector2F size);
	void MakeSquareVertex(void);

	// 座標設定
	void SetPos(Vector2F pos);

	// 画像サイズ設定
	void SetSize(Vector2F size);

	// 描画
	void Draw(void);
	void Draw(float x, float y);

private:

	// 座標
	Vector2F pos_;

	// 描画サイズ
	Vector2F size_;

	// 頂点
	VERTEX2DSHADER vertexs_[NUM_VERTEX];

	// 頂点インデックス
	WORD indexes_[NUM_VERTEX_IDX];

	// ピクセルマテリアル
	PixelMaterial& normalCardPSMaterial_;

};
