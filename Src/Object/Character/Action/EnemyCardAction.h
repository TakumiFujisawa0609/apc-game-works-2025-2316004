#pragma once
#include<functional>
#include<queue>
#include "../Base/ActionBase.h"
#include "../Base/CardActionBase.h"

class CardDeck;
class CharacterBase;

class EnemyCardAction :
    public CardActionBase
{

public:

    enum class ACT_TYPE
    {
        NONE = -1,
        SWIP_ATK, //ひっかき攻撃
        ROAR,     //咆哮
        JUMP_ATK, //攻撃アクション3回目
		RUSH_ATK, //ロール攻撃
		CALL_PROJECTILE, //射撃物呼び出し
		DOUBLE_SPHERE_ATK, //ダブルスフィア攻撃
        SHOT_CUTTER,     //斬撃飛ばし
        RELOAD, //リロード
    };
    EnemyCardAction(ActionController& _actCntl, CharacterBase& _charaObj, CardDeck& _deck);
    ~EnemyCardAction(void)override;

    /// @brief 初期化
    /// @param  
    void Init(void) override;

    /// @brief 更新
    /// @param  
    void Update(void) override;

    /// @brief 解放
    /// @param  
    void Release(void)override;

    /// @brief ジャンプ攻撃チャージ中取得
    /// @param  
    /// @return 
    const bool IsJumpAtkCharge(void)const override;

    /// @brief 敵のジャンプチャージ中のカード勝負数
    /// @param  
    /// @return 
    virtual const int GetJumpCardNum(void)const override { return jampCardNum_; }
    
private:

    //攻撃1段目判定
    static constexpr float ATTACK_ONE_COL_START_ANIM_CNT = 35.0f;   //攻撃当たり判定開始アニメーションカウント
    static constexpr float ATTACK_ONE_COL_END_ANIM_CNT = 46.0f;     //攻撃当たり判定終了アニメーションカウント

    //重力
    static constexpr float GRAVITY = 0.1f;
    //ジャンプ高さ
    static constexpr float JUMP_HEIGHT = 100.0f;
    //ジャンプ攻撃アニメーション
    static constexpr float JUMP_ANIM_START = 16.4f;
    static constexpr float JUMP_ANIM_END = 52.0f;
    ////攻撃2段目判定
    static constexpr float ATTACK_TWO_COL_START_ANIM_CNT = 63.0f;   //攻撃当たり判定開始アニメーションカウント
    static constexpr float ATTACK_TWO_COL_END_ANIM_CNT = 65.0f;     //攻撃当たり判定終了アニメーションカウント
    //攻撃半径広がるスピード
    static constexpr float JUMP_ATK_COL_SPD = 5.0f;
    //広がる時間
    static constexpr float JUMP_ATK_CNT_MAX = 2.5f;
    //ジャンプチャージ時間
    static constexpr float JUMP_CHARGE_TIME = 6.0f;
    //咆哮判定
    static constexpr float ROAR_COL_START_ANIM_CNT = 52.0f;   //攻撃当たり判定開始アニメーションカウント
    static constexpr float ROAR_COL_END_ANIM_CNT = 122.0f;     //攻撃当たり判定終了アニメーションカウント
    //転がる攻撃
    static constexpr float ROLE_COL_START_ANIM_CNT = 52.0f;   //攻撃当たり判定開始アニメーションカウント
    static constexpr float ROLE_COL_END_ANIM_CNT = 122.0f;     //攻撃当たり判定終了アニメーションカウント

    static constexpr float ROLE_PRE_TIME = 5.0F;     //攻撃前隙時間
    static constexpr float ROLE_TIME = 0.5f;    //転がる時間
    static constexpr float ROLE_SPEED = 20.0f; //転がるスピード

    //攻撃ローカル座標
    static constexpr VECTOR ATK_ONE_LOCAL = { 0.0f,100.0f,50.0f };
    static constexpr VECTOR JUMP_ATK_LOCAL = { 0.0f,100.0f,0.0f };

    static constexpr float ATK_SPHERE_RADIUS = 50.0f;					//通常攻撃の球体の半径
    static constexpr float JUMP_ATK_RADIUS = 30.0f;						//ジャンプ攻撃の始まりの半径
	static constexpr float ROAR_ATK_RADIUS = 300.0f;                    //咆哮攻撃の球体の半径
	static constexpr float ROLE_ATK_RADIUS = 150.0f;                     //転がる攻撃の球体の半径
    //ジャンプ攻撃範囲
    static constexpr float JUMP_ATK_GOAL_RADIUS = 2500.0f;

    //ジャンプアニメーションループ
    static constexpr float JUMP_ATK_ANIM_LOOP_START = 12.0f;
    static constexpr float JUMP_ATK_ANIM_LOOP_END = 13.0f;
    static constexpr float JUMP_ATK_ANIM_LOOP_SPEED = 5.0f;

    //ひっかき攻撃のステータス
    static constexpr CardActionBase::ATK_STATUS SWIP_ATK = { ATTACK_ONE_COL_START_ANIM_CNT,ATTACK_ONE_COL_END_ANIM_CNT,ATK_SPHERE_RADIUS };
    //ジャンプ攻撃のステータス
    static constexpr CardActionBase::ATK_STATUS JUMP_ATK = { ATTACK_TWO_COL_START_ANIM_CNT,ATTACK_TWO_COL_END_ANIM_CNT,JUMP_ATK_RADIUS };
    //咆哮のステータス
    static constexpr CardActionBase::ATK_STATUS ROAR_ATK = { ROAR_COL_START_ANIM_CNT,ROAR_COL_END_ANIM_CNT,ROAR_ATK_RADIUS };
	//転がるのステータス
	static constexpr CardActionBase::ATK_STATUS RUSH_ATK = { ROLE_COL_START_ANIM_CNT,ROLE_COL_END_ANIM_CNT,ROLE_ATK_RADIUS };


    //ジャンプ攻撃カウント
    float jumpAtkCnt_;
    //ジャンプチャージカウント
    float jumpChargeCnt_;
    //ジャンプチャージ中のカード勝敗数カウント
    int jampCardNum_;

    //イージング
    std::unique_ptr<Easing>easing_;

    //遷移
    void ChangeSwip(void);
    void ChangeRoar(void);
    void ChangeJumpAtk(void);
	void ChangeRoleAtk(void);
    void ChangeReload(void);
    void ChangeDuel(void)override;
    //更新
    void UpdateSwip(void);
    void UpdateRoar(void);
    void UpdateJumpAtk(void);
	void UpdateRoleAtk(void);
    void UpdateReload(void);
    void UpdateDuel(void)override;

    //ジャンプチャージ中、カード負けしても何枚かカードを出す
    bool IsCardFailureJumpCharge(void);

	//アクションによって処理を分岐
	void DesideCardAction(void);

    //デュエルデッキからカードを出す
    void PutCardToDuelDeck(void);

    //攻撃別の当たり判定情報
    std::map<ACT_TYPE, ATK_STATUS>atkTable_;

    //転がる攻撃関連
    float preRoleAtkCnt_;//前隙カウント
    float roleAtkCnt_; //後隙カウント
    VECTOR preRolePos_;//転がる攻撃する前の座標
	float roleDeg_; //ラリアット回転角
	float roleMoveDeg_; //ラリアット移動方向
	VECTOR roleMoveDir_; //ラリアット移動方向ベクトル
};

