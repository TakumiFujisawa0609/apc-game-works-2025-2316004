#pragma once
#include<DxLib.h>
class InputBase
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


    struct ACT_FLAG
    {
        bool isCardMoveLeft = false;
        bool isCardMoveRight = false;
        bool isCardUse = false;
        bool isCardCharge = false;
    };

    InputBase(void);
	virtual ~InputBase(void);
    //変更時の初期化
    virtual void Init(void) = 0;

    virtual void Update(void) = 0;

    //コントロール判定
    bool CheckAct(ACT_CNTL _actCntl) { return actCntl_ == _actCntl ? true : false; }
	//移動角度の取得
    const VECTOR GetDir(void) { return moveDir_; }
	//移動角度の取得
	const float GetMoveDeg(void) { return moveDeg_; }
    //アクション入力の取得
    const ACT_FLAG GetIsAct(void) { return isAct_; }

protected:
	//メンバ変数
	//-----------------------------------------------------------------------
	//操作管理用
	ACT_CNTL actCntl_;

	float moveDeg_;             //移動角度
	VECTOR moveDir_;            //移動方向ベクトル

    //アクション入力がされたかどうか
    ACT_FLAG isAct_;
};

