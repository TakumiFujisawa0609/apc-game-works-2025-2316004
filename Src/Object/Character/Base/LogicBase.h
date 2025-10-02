#pragma once
#include<DxLib.h>
class LogicBase
{
public:
    //アクションボタンの種類
    enum class ACT_CNTL
    {
        NONE            //何もしていないとき
        , MOVE          //移動
        , DASHMOVE      //ダッシュ
        , PUNCH         //パンチ
        , JUMP          //ジャンプ
        , CARD_CHARGE   //カードチャージ
        , CARD_USE      //カード使用
        , CARD_MOVE_LEFT //カード左
        , CARD_MOVE_RIGHT//カード右
    };

    //どの操作がされたか
    struct ACT_FLAG
    {
        bool isCardMoveLeft = false;    //カードを左へ
        bool isCardMoveRight = false;   //カードを右へ
        bool isCardUse = false;         //カード使用
        bool isCardCharge = false;      //カードをチャージ配列へ  
        bool isCardPushKeep = false;    //カード使用キー長押し
    };

    LogicBase(void);
	virtual ~LogicBase(void);
    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name=""></param>
    virtual void Init(void) = 0;

    /// <summary>
    /// 更新
    /// </summary>
    /// <param name=""></param>
    virtual void Update(void) = 0;

    //コントロール判定
    bool CheckAct(ACT_CNTL _actCntl) { return actCntl_ == _actCntl ? true : false; }
    /// <summary>
    /// 移動方向の取得
    /// </summary>
    /// <param name=""></param>
    /// <returns>方向</returns>
    const VECTOR GetDir(void) { return moveDir_; }
	/// <summary>
	/// 移動角度の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const float GetMoveDeg(void) { return moveDeg_; }
    /// <summary>
    /// アクション入力の取得
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    const ACT_FLAG GetIsAct(void) { return isAct_; }

protected:
	//操作管理用
	ACT_CNTL actCntl_;
    //移動関連
	float moveDeg_;             //移動角度
	VECTOR moveDir_;            //移動方向ベクトル

    //アクション入力がされたかどうか
    ACT_FLAG isAct_;
};

