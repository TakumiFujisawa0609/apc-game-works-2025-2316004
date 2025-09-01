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

    InputBase(void);
	virtual ~InputBase(void);
    //変更時の初期化
    virtual void Init(void) = 0;

    virtual void Update(void) = 0;

    //コントロール判定
    bool CheckAct(ACT_CNTL _actCntl) { return actCntl_ == _actCntl ? true : false; }
protected:
	//メンバ変数
	//-----------------------------------------------------------------------
	//操作管理用
	ACT_CNTL actCntl_;

	float moveDeg_;             //移動角度
	VECTOR moveDir_;            //移動方向ベクトル
};

