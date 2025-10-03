#pragma once
#include"../Object/ObjectBase.h"

class AnimationController;
class ActionController;
class CardDeck;
class InputBase;
class Capsule;

class CharacterBase :public ObjectBase
{
public:
	//アニメーション速度
	static constexpr float ANIM_SPEED = 40.0f;
	// アニメーション種別
	enum class ANIM_TYPE
	{
		NONE,
		IDLE,
		RUN,
		REACT,
		ATTACK_1,
		ATTACK_2,
		ATTACK_3,
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	CharacterBase(void);


	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	virtual ~CharacterBase(void)override = 0;

	/// <summary>
/// 読み込み
/// </summary>
/// <param name=""></param>
	virtual void Load(void)override = 0;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	virtual void Init(void)override = 0;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name=""></param>
	virtual void Update(void)override = 0;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name=""></param>
	virtual void Draw(void)override = 0;
protected:
	//カプセル関連
	static constexpr VECTOR CAP_LOCAL_TOP = { 0.0f, 150.0f, 0.0f };	//トップ座標
	static constexpr VECTOR CAP_LOCAL_DOWN = { 0.0f,0.0f,0.0f };	//ダウン座標
	static constexpr float CAP_RADIUS = 25.0f;

	//入力
	std::unique_ptr<LogicBase>logic_;
	//行動系
	std::unique_ptr<ActionController>action_;
	// アニメーション
	std::unique_ptr<AnimationController>animationController_;
	//デッキ
	std::shared_ptr<CardDeck>deck_;
	//当たり判定用のカプセル
	std::unique_ptr<Capsule>cap_;
private:

};

