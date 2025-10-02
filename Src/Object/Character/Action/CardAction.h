#pragma once
#include<functional>
#include<queue>
#include "../Base/ActionBase.h"
class CardDeck;
class CardAction :
    public ActionBase
{

public:

    static constexpr float CARD_ACT_TIME_MAX = 5.0f; //カードアクション時間(デバッグ用)

    enum class ACT_STATE
    {
        NONE = -1,
		ATTACK_ONE, //攻撃アクション1回目
        ATTACK_TWO, //攻撃アクション2回目
        ATTACK_THREE, //攻撃アクション3回目
        RELOAD, //リロード
		HEAL_CARD,       //ヒールカード使用
		MAGIC_CARD,        //マジックカード使用
        NUM,
	};

    CardAction(ActionController& _actCntl,CardDeck& _deck);
    ~CardAction(void)override;
    void Init(void) override;
    void Update() override;

public:
    //攻撃判定
	static constexpr float ATTACK_COL_START_ANIM_CNT = 14.0f;   //攻撃当たり判定開始アニメーションカウント
	static constexpr float ATTACK_COL_END_ANIM_CNT = 25.0f;     //攻撃当たり判定終了アニメーションカウント

    //リロードのボタン押す時間
    static constexpr float RELOAD_TIME = 3.0f;

    //カードデッキ
    CardDeck& deck_;
    //カードアクション中かどうか
    bool isCardAct_;
    //カードアクション時間(デバッグ用)
    float cardActTime_; 
	//カードアクション関数ポインタ
	std::function<void(void)> cardActFunc_;
	//カードアクション遷移
    std::map<ACT_STATE, std::function<void(void)>>changeAction_;
    //アタックのQueue配列
    std::queue<std::function<void(void)>>cardFuncs_;

    //攻撃の当たり判定始まりカウント
    float attackStartAnimcnt_;
    //攻撃の当たり判定終わりカウント
    float attackEndAnimCnt_;
    //攻撃段階
    int attackStageNum_;

    //リロード
    int pushReloadCnt_;

    //攻撃条件
    bool IsAttackable(void);
    //カードの勝敗を常に監視する
    bool IsCardFailure(void);

    //更新系
	void UpdateAttack(void);         //攻撃アクション
    void UpdateReload(void);         //リロード

    //遷移系
	void ChangeAttackOne(void);         //攻撃アクション1回目
	void ChangeAttackTwo(void);         //攻撃アクション2回目
	void ChangeAttackThree(void);       //攻撃アクション3回目
    void ChangeReload(void);            //リロード
};

