#pragma once
#include"./CardBase.h"

class Easing;
class PixelMaterial;
class PixelRenderer;
class CardUIController;

class CardUIDraw :public ObjectBase
{

public:

	/// @brief コンストラクタ
	/// @param _typeImg カード画像
	/// @param _centerPos 中心座標
	/// @param _scl 大きさ
	CardUIDraw(int& _typeImg,Vector2F& _centerPos, float& _scl);

	/// @brief デストラクタ
	/// @param  
	~CardUIDraw(void);

	/// @brief ロード
	/// @param  
	void Load(void)override;

	/// @brief 初期化
	/// @param  
	void Init(void)override;

	/// @brief 更新
	/// @param  
	void Update(void)override;

	/// @brief 描画
	/// @param  
	void Draw(void)override;

	/// @brief 選択カード枠描画
	/// @param  
	void DrawSelectedFrame(void);

	/// @brief 選択カード描画
	/// @param  
	void DrawSelectCard(void);

	/// @brief リロードゲージ描画
	/// @param  
	void DrawReloadGauge(const int& _reloadFrameImg,const float& _reloadPer);

private:

	//カード強さ番号の倍率
	static constexpr float CARD_SCL = 0.5f;
	//カード初期座標
	static constexpr VECTOR CARD_INIT_POS = { 50.0f,200.0f,0.0f };
	//選択中のフォグの強さ
	static constexpr float SELECT_FOG_STRENGTH = 0.3f;
	//選択カード枠の大きさイージング時間
	static constexpr float SELECT_CARD_FRAME_EASING_TIME = 1.0f;
	//選択カード枠移動量
	static constexpr float SELECT_CARD_FRAME_MOVE_AMOUNT = 10.0f;


	//フレーム固定座標
	static constexpr Vector2F LEFT_UP_FRAME_POS = { 19.5f,343.0f };
	static constexpr Vector2F RIGHT_DOWN_FRAME_POS = { 140.0f,509.0f };

	//シェーダの定数バッファスロット
	static constexpr int CONST_BUF_SLOT_NUM = 3;
	//選択カードの定数バッファスロット
	static constexpr int CARD_NUM_CONST_BUF_SIZE = 3;
	//選択カードアウトラインサイズ
	static constexpr float SELECT_CARD_OUTLINE_SIZE = 100.0f;	

	//通常カードピクセルマテリアル
	std::unique_ptr<PixelMaterial> normalCardPSMaterial_;
	//通常カードピクセルレンダラー
	std::unique_ptr<PixelRenderer> normalCardPSRenderer_;
	//リロードカード
	std::unique_ptr<PixelMaterial> reloadCardPSMaterial_;
	std::unique_ptr<PixelRenderer> reloadCardPSRenderer_;

	//選択カード枠マテリアル
	std::unique_ptr<PixelMaterial> selectCardPSMaterial_;
	std::unique_ptr<PixelRenderer> selectCardPSRenderer_;

	//イージング
	std::unique_ptr<Easing>easing_;

	int& typeImg_;		//カードの種類画像

	Vector2F halfSize_;		//カードの半分大きさ
	Vector2F size_;			//カードの大きさ
	Vector2F& centerPos_;	//中心座標
	Vector2F rightTopPos_;	//左上座標
	Vector2F leftDownPos_;	//右下座標
	float& scl_;			//サイズ
	float selectEaseCnt_;	//選択枠イージングカウント
	//カードの描画
	void DrawCard(void);

	//選択枠イージング
	void SelectFrameEasing(void);
};

