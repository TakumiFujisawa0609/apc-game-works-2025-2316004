#pragma once
#include"./CardBase.h"

class PixelMaterial;
class PixelRenderer;
class CardUIController;

class CardUIDraw :public ObjectBase
{

public:

	CardUIDraw(int& _typeImg,Vector2F& _centerPos, float& _scl);
	~CardUIDraw(void);
	void Load(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;

	/// @brief カード３Dモデル描画
	/// @param  
	void DrawModel(void);

private:

	//カード強さ番号の倍率
	static constexpr float CARD_SCL = 0.5f;

	//カード初期座標
	static constexpr VECTOR CARD_INIT_POS = { 50.0f,200.0f,0.0f };

	//シェーダの定数バッファスロット
	static constexpr int CONST_BUF_SLOT_NUM = 3;
	//ピクセルマテリアル
	std::unique_ptr<PixelMaterial> pixelMaterial_;
	//ピクセルレンダラー
	std::unique_ptr<PixelRenderer> pixelRenderer_;

	int& typeImg_;		//カードの種類画像

	Vector2F halfSize_;		//カードの半分大きさ
	Vector2F size_;			//カードの大きさ
	Vector2F& centerPos_;	//中心座標
	Vector2F rightTopPos_;	//左上座標
	Vector2F leftDownPos_;	//右下座標
	float& scl_;			//サイズ


};

