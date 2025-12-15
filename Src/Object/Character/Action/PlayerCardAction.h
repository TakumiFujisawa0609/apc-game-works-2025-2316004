#pragma once
#include<functional>
#include<queue>
//#include "../Base/ActionBase.h"
#include "../Base/CardActionBase.h"

class CardDeck;
class CharacterBase;

class PlayerCardAction :
    public CardActionBase
{

public:

    static constexpr float CARD_ACT_TIME_MAX = 5.0f; //カードアクション時間(デバッグ用)

    /// @brief コンストラクタ
    /// @param _actCntl アクションコントローラ
	/// @param _charaObj キャラクターオブジェクト
    /// @param _deck デッキ
    PlayerCardAction(ActionController& _actCntl, CharacterBase& _charaObj, CardDeck& _deck);

    /// @brief デストラクタ
    /// @param  
    ~PlayerCardAction(void)override;

	/// @brief ロード
	/// @param  
	void Load(void) override;

    /// @brief 初期化
    /// @param  
    void Init(void) override;

	/// @brief 更新
    /// @param  
    void Update(void) override;

	/// @brief 解放
	/// @param
    void Release(void)override;
private:

    //攻撃1段目判定
	static constexpr float ATTACK_ONE_COL_START_ANIM_CNT = 24.0f;   //攻撃当たり判定開始アニメーションカウント
	static constexpr float ATTACK_ONE_COL_END_ANIM_CNT = 36.0f;     //攻撃当たり判定終了アニメーションカウント
    //攻撃2段目判定
	static constexpr float ATTACK_TWO_COL_START_ANIM_CNT = 23.0f;   //攻撃当たり判定開始アニメーションカウント
	static constexpr float ATTACK_TWO_COL_END_ANIM_CNT = 35.0f;     //攻撃当たり判定終了アニメーションカウント
    //攻撃3段目判定
	static constexpr float ATTACK_THREE_COL_START_ANIM_CNT = 73.0f;   //攻撃当たり判定開始アニメーションカウント
	static constexpr float ATTACK_THREE_COL_END_ANIM_CNT = 83.0f;     //攻撃当たり判定終了アニメーションカウント
    static constexpr float ATK_SPHERE_RADIUS = 120.0f;				//カプセル球の半径
    //攻撃アニメーション3段階目の溜め時のアニメ速度
    static constexpr float ATTACK_THREE_ANIM_SPD = 10.0f;
    //攻撃3段階目アニメスピード補完時間
    static constexpr float ATTACK_THREE_ANIM_LERP_TIME = 0.4f;       
    
    //リロードアニメーション始め
    static constexpr float RELOAD_START_STEP = 17.0f;       //アニメーション始まりステップ
    static constexpr float RELOAD_END_STEP = 33.0f;         //アニメーション終わり
    static constexpr float RELOAD_LOOP_START = 26.0f;       //アニメーションループスタート
    static constexpr float RELOAD_LOOP_END = 30.0f;         //アニメーションループ終了


    //1段目攻撃のステータス
    static constexpr CardActionBase::ATK_STATUS NORMAL_ATK_ONE = { ATTACK_ONE_COL_START_ANIM_CNT,ATTACK_ONE_COL_END_ANIM_CNT,ATK_SPHERE_RADIUS };
    //2段目攻撃のステータス
    static constexpr CardActionBase::ATK_STATUS NORMAL_ATK_TWO = { ATTACK_TWO_COL_START_ANIM_CNT,ATTACK_TWO_COL_END_ANIM_CNT,ATK_SPHERE_RADIUS };
    //3段目攻撃のステータス
    static constexpr CardActionBase::ATK_STATUS NORMAL_ATK_THREE = { ATTACK_THREE_COL_START_ANIM_CNT,ATTACK_THREE_COL_END_ANIM_CNT,ATK_SPHERE_RADIUS };

    //リロードのボタン押す時間
    static constexpr float RELOAD_TIME = 3.0f;
    //攻撃ローカル座標
    static constexpr VECTOR ATK_ONE_LOCAL = { 0.0f,0.0f,50.0f };


    //攻撃の当たり判定始まりカウント
    float attackStartAnimcnt_;
    //攻撃の当たり判定終わりカウント
    float attackEndAnimCnt_;
    //攻撃段階
    int attackStageNum_;

    //リロードカウント
    float pushReloadCnt_;

    //サウンドマネージャ
	SoundManager& soundMng_;

    //3段階目攻撃判定終了カウント
    float atkThreeEndCnt_;
    float atkAnimLerpCnt_;
    //攻撃条件
    bool IsAttackable(void);
    //連続攻撃条件
    bool IsCanComboAttack(void);
    //攻撃モーション
    //void AttackMotion(const float _atkColStart, const float _atlColEnd);

    //攻撃状態遷移時のカード初期化
    void ChangeActionCardInit(void);



    //更新系
	void UpdateAttack(void);         //攻撃アクション
    void UpdateAttackThree(void);    //攻撃アクション３段階目(１，２段目とは違う演出を入れる)
    void UpdateReload(void);         //リロード
    void UpdateSonicRave(void);     //ソニックレイヴ
    void UpdateDuel(void)override;  //デュエルモード

    //遷移系
	void ChangeAttackOne(void);         //攻撃アクション1回目
	void ChangeAttackTwo(void);         //攻撃アクション2回目
	void ChangeAttackThree(void);       //攻撃アクション3回目
    void ChangeReload(void);            //リロード
    void ChangeSonicRave(void);         //ソニックレイヴ
    void ChangeDuel(void)override;      //デュエルモード
    //コンボアクション遷移(あれば実装する)
    void ChangeComboAction(void)override;
};

