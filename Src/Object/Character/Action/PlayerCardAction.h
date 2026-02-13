#pragma once
#include<functional>
#include<queue>
//#include "../Base/ActionBase.h"
#include "../Base/CardActionBase.h"

class Easing;
class CardDeck;
class CharacterBase;

class PlayerCardAction :
    public CardActionBase
{

public:

    /// @brief コンストラクタ
    /// @param _actCntl アクションコントローラ
	/// @param _charaObj キャラクターオブジェクト
    /// @param _deck デッキ
    PlayerCardAction(ActionController& _actCntl, CharacterBase& _charaObj, CardPresenter& _deck);

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

    //攻撃1段目判定(近距離)
	static constexpr float ATTACK_ONE_COL_START_ANIM_CNT = 24.0f;   //攻撃当たり判定開始アニメーションカウント
	static constexpr float ATTACK_ONE_COL_END_ANIM_CNT = 36.0f;     //攻撃当たり判定終了アニメーションカウント
    //攻撃1段目判定(中距離)
	static constexpr float ATTACK_ONE_MID_COL_START_ANIM_CNT = 16.0f;   //攻撃当たり判定開始アニメーションカウント
	static constexpr float ATTACK_ONE_MID_COL_END_ANIM_CNT = 35.0f;     //攻撃当たり判定終了アニメーションカウント
    //アニメーションスタート位置
    static constexpr float ATTACK_ONE_MID_ANIM_START = 13.0f;
    static constexpr float ATTACK_ONE_MID_ANIM_END = 37.0f;
    //コンボ受付時間
    static constexpr float ATTACK_ONE_MID_COMBO_TIME = 0.2f;
    //中距離移動時間
    static constexpr float ATTACK_ONE_MID_TIME = 1.0f;
    //中距離初期速度
    static constexpr float ATTACK_ONE_MID_SPD = 30.0f;
    //攻撃2段目判定
	static constexpr float ATTACK_TWO_COL_START_ANIM_CNT = 23.0f;   //攻撃当たり判定開始アニメーションカウント
	static constexpr float ATTACK_TWO_COL_END_ANIM_CNT = 35.0f;     //攻撃当たり判定終了アニメーションカウント
    //攻撃3段目判定
	static constexpr float ATTACK_THREE_COL_START_ANIM_CNT = 73.0f;   //攻撃当たり判定開始アニメーションカウント
	static constexpr float ATTACK_THREE_COL_END_ANIM_CNT = 83.0f;     //攻撃当たり判定終了アニメーションカウント
    static constexpr float ATK_SPHERE_RADIUS = 0.0f;				//カプセル球の半径
    //攻撃３段階目アニメーションスタート
    static constexpr float ATTACK_THREE_ANIM_START = 60.0f;
    //攻撃３段階目アニメーションゴール
    static constexpr float ATTACK_THREE_ANIM_GOAL = 86.0f;
    //攻撃アニメーション3段階目の溜め時のアニメ速度
    static constexpr float ATTACK_THREE_ANIM_SPD = 10.0f;
    //攻撃3段階目アニメスピード補完時間
    static constexpr float ATTACK_THREE_ANIM_LERP_TIME = 0.4f;       
    
    //リロードアニメーション始め
    static constexpr float RELOAD_START_STEP = 17.0f;       //アニメーション始まりステップ
    static constexpr float RELOAD_END_STEP = 33.0f;         //アニメーション終わり
    static constexpr float RELOAD_LOOP_START = 26.0f;       //アニメーションループスタート
    static constexpr float RELOAD_LOOP_END = 30.0f;         //アニメーションループ終了

    //攻撃ごとの攻撃力
    static constexpr float ATTACK_ONE_POINT = 10.0f;
    static constexpr float ATTACK_TWO_POINT = 15.0f;
    static constexpr float ATTACK_THREE_POINT =25.0f;

    //攻撃段階
    static constexpr int ATTACK_ONE = 0;
    static constexpr int ATTACK_TWO = 1;
    static constexpr int ATTACK_THREE = 2;

    //1段目攻撃のステータス(近距離)
    static constexpr CardActionBase::ATK_STATUS NORMAL_ATK_ONE_SHORT = 
    { ATTACK_ONE_COL_START_ANIM_CNT,ATTACK_ONE_COL_END_ANIM_CNT,ATK_SPHERE_RADIUS,ATTACK_ONE_POINT,{},false };
    //1段目攻撃のステータス(中距離)
    static constexpr CardActionBase::ATK_STATUS NORMAL_ATK_ONE_MIDDLE = 
    { ATTACK_ONE_MID_COL_START_ANIM_CNT,ATTACK_ONE_MID_COL_END_ANIM_CNT,ATK_SPHERE_RADIUS,ATTACK_ONE_POINT,{},false };
    //2段目攻撃のステータス
    static constexpr CardActionBase::ATK_STATUS NORMAL_ATK_TWO = 
    { ATTACK_TWO_COL_START_ANIM_CNT,ATTACK_TWO_COL_END_ANIM_CNT,ATK_SPHERE_RADIUS,ATTACK_TWO_POINT,{},false };
    //3段目攻撃のステータス
    static constexpr CardActionBase::ATK_STATUS NORMAL_ATK_THREE = 
    { ATTACK_THREE_COL_START_ANIM_CNT,ATTACK_THREE_COL_END_ANIM_CNT,ATK_SPHERE_RADIUS ,ATTACK_THREE_POINT,{},false };

    //リロードのボタン押す時間
    static constexpr float RELOAD_TIME = 3.0f;

    //攻撃ローカル座標
    static constexpr VECTOR ATK_ONE_LOCAL = { 0.0f,0.0f,50.0f };
    //カードリロード中の音量
    static constexpr int CARD_RELOAD_VOL = 60;

    //攻撃の当たり判定始まりカウント
    float attackStartAnimcnt_;
    //攻撃の当たり判定終わりカウント
    float attackEndAnimCnt_;
    //攻撃段階
    int attackStageNum_;

    //リロードカウント
    float pushReloadCnt_;

    //中距離攻撃突きカウント
    float midAtkCnt_;
    float midAtkOverCnt_;

    //サウンドマネージャ
	SoundManager& soundMng_;

    //3段階目攻撃判定終了カウント
    float atkThreeEndCnt_;
    float atkAnimLerpCnt_;

    //イージング
    std::unique_ptr<Easing>easing_;
    //攻撃条件
    bool IsAttackable(void);
    //連続攻撃条件
    bool IsCanComboAttack(void);
    //攻撃モーション
    //void AttackMotion(const float _atkColStart, const float _atlColEnd);

    //１段目を中距離攻撃アクションか近距離攻撃アクションかを決める
    void DesideAttackOne(void);

    //攻撃状態遷移時のカード初期化
    void ChangeActionCardInit(void);



    //更新系
	void UpdateAttack(void);            //攻撃アクション
    void UpdateMiddleAttack(void);      //中距離攻撃アクション１回目(中距離)
    void UpdateAttackThree(void);       //攻撃アクション３段階目(１，２段目とは違う演出を入れる)
    void UpdateReload(void);            //リロード
    void UpdateSonicRave(void);         //ソニックレイヴ
    void UpdateDuel(void)override;      //デュエルモード

    //遷移系
	void ChangeShortAttackOne(void);        //攻撃アクション1回目(近距離)
	void ChangeMiddleAttackOne(void);        //攻撃アクション1回目(中距離)
	void ChangeAttackTwo(void);             //攻撃アクション2回目
	void ChangeAttackThree(void);           //攻撃アクション3回目
    void ChangeReload(void);                //リロード
    void ChangeSonicRave(void);             //ソニックレイヴ
    void ChangeDuel(void)override;          //デュエルモード
    //コンボアクション遷移(あれば実装する)
    void ChangeComboAction(void);
};

