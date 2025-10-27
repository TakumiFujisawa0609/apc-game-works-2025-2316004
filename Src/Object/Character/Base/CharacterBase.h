#pragma once
#include<map>
#include"../../Card/CardBase.h"
#include"../Object/ObjectBase.h"
#include"../Player/ActionController.h"
class AnimationController;
class ActionController;
class CardDeck;
class CardUI;
class InputBase;
class Capsule;
class LogicBase;
class PlayerOnHit;



class CharacterBase :public ObjectBase
{
public:

	//アニメーション速度
	static constexpr float ANIM_SPEED = 40.0f;

	// 回転完了までの時間
	static constexpr float TIME_ROT = 0.1f;

	//最大ＨＰ
	static constexpr int HP_MAX = 200;

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
		//敵
		SWIP_ATK,
		ROAR_ATK,
		JUMP_ATK

	};

	struct ROTATION
	{
		//回転
		Quaternion playerRotY_;		//プレイヤーY角度
		Quaternion goalQuaRot_;		//目的の回転
		float stepRotTime_;			//補完時間
		VECTOR dir_;			//方向
	};

	struct STATUS
	{
		int maxHp_= HP_MAX;			//最大体力
		int hp_ = maxHp_;	//体力
		int atk_;			//攻撃力
		//int def_;			//防御力
	};



	enum class ACTION_TYPE
	{
		IDLE,		//何もしてない
		MOVE,		//移動
		DASHMOVE,	//ダッシュ
		REACT,	//パンチされた状態
		JUMP,		//ジャンプ
		CARD_ACTION	//カードアクション
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

	/// <summary>
	/// 当たった時の処理
	/// </summary>
	/// <param name="_hitCol"></param>
	virtual void OnHit(const std::weak_ptr<Collider> _hitCol)override = 0;
	
	/// <summary>
	/// 攻撃の当たり判定生成
	/// </summary>
	/// <param name="_charaTag">どのキャラクターか</param>
	void MakeAttackCol(const Collider::TAG _charaTag,const VECTOR& _atkPos,const float& _radius);

	/// <summary>
	/// 攻撃の当たり判定削除
	/// </summary>
	/// <param name="_charaTag"></param>
	/// <param name="_atkPos"></param>
	void DeleteAttackCol(const Collider::TAG _charaTag);

	/// <summary>
	/// 移動後座標などの更新
	/// </summary>
	/// <param name=""></param>
	void UpdatePost(void);

	/// <summary>
	/// キャラタグの取得
	/// </summary>
	const Collider::TAG GetCharaTag(void)const { return tag_; }

	/// <summary>
	/// 入力方向に応じて方向を決める
	/// </summary>
	/// <param name=""></param>
	virtual void MoveDirFronInput(void);
	/// <summary>
	/// ゴール角度をセット
	/// </summary>
	/// <param name="_deg"></param>
	virtual void SetGoalRotate(const double _deg);

	/// <summary>
	/// 方向関連の変数の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline const ROTATION GetRotation(void)const { return charaRot_; }

	/// <summary>
	/// 角度更新
	/// </summary>
	/// <param name=""></param>
	void Rotate(void);

	/// @brief ダメージ処理
	/// @param _dam ダメージ数
	void Damage(const int _dam);
	/// @brief ステータス取得
	/// @param  
	/// @return 
	inline const STATUS GetStatus(void) { return status_; }

	/// @brief カードUI
	/// @param  
	/// @return 
	inline CardUI& GetCardUI(void) { return *cardUI_; }
	
	/// @brief　使用済みカードについての処理
	/// @param  
	void DeleteCard(void);
protected:
	//カプセル関連
	static constexpr VECTOR CAP_LOCAL_TOP = { 0.0f, 150.0f, 0.0f };	//トップ座標
	static constexpr VECTOR CAP_LOCAL_DOWN = { 0.0f,0.0f,0.0f };	//ダウン座標
	static constexpr float CAP_RADIUS = 20.0f;						//カプセル球の半径

	//当たり判定
	static constexpr int CUPSULE_COL_NO = 0;
	static constexpr int MOVE_LINE_COL_NO = 1;
	static constexpr int ATK_COL_NO = 2;
	//移動量ラインオフセット
	static constexpr float MOVE_LINE_Y_OFFSET = - 1.0f;
	//移動量更新条件の移動ラインの長さ
	static constexpr float MOVE_LINE_Y_CHECK_VALUE =  1.5f;
	//リロードカードステータス
	static constexpr CardBase::CARD_STATUS RELOAD_CARD_STATUS = { -1,CardBase::CARD_TYPE::RELOAD };

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
	//当たった時の処理
	std::unique_ptr<PlayerOnHit>onHit_;

	//当たり判定の要素
	VECTOR movedPos_;		//移動後座標
	VECTOR moveDiff_;		//移動前座標
	
	//角度関連
	ROTATION charaRot_;
	//ステータス
	STATUS status_;
	//カードUI(とりあえず)
	std::unique_ptr<CardUI>cardUI_;

private:

};

