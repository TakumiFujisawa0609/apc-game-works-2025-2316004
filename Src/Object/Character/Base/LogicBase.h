#pragma once
#include<DxLib.h>
#include "../Object/Common/Transform.h"

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

    enum class ENEMY_ATTACK_TYPE {
        NONE,
        NORMAL,
        JUMP,
        ROAR,
        ROLE
    };

    //どの操作がされたか
    struct ACT_FLAG
    {
        //カード関連
        bool isCardMoveLeft = false;    //カードを左へ
        bool isCardMoveRight = false;   //カードを右へ
        bool isCardUse = false;         //カード使用
        bool isCardCharge = false;      //カードをチャージ配列へ  
        bool isCardPushKeep = false;    //カード使用キー長押し

        //アクション関連
		bool isRun = false;         //走る
		bool isJump = false;        //ジャンプ
		bool isDodge = false;       //回避
    };



	/// @brief コンストラクタ
    /// @param  
    LogicBase(void);
	
    /// @brief デストラクタ
    /// @param  
    virtual ~LogicBase(void);
    
    /// @brief 初期化
    /// @param  
    virtual void Init(void) = 0;
    
    /// @brief 更新
    /// @param  
    virtual void Update(void) = 0;
    
    /// @brief コントロール判定
	/// @param _actCntl ゲットしたい操作種類
    /// @return 
    const bool CheckAct(const ACT_CNTL& _actCntl)const { return actCntl_ == _actCntl ? true : false; }
    
    /// @brief 移動方向の取得
    /// @param  
    /// @return 方向
    const VECTOR& GetDir(void)const { return moveDir_; }
	
    /// @brief 移動角度の取得
    /// @param  
    /// @return 移動角度
    const float& GetMoveDeg(void)const { return moveDeg_; }
    
    /// @brief アクション入力の取得
    /// @param  
    /// @return アクション入力
    const ACT_FLAG GetIsAct(void)const { return isAct_; }

    /// @brief ランダムで決まった攻撃の種類を取得
    /// @param  
    /// @return 攻撃種類
    const ENEMY_ATTACK_TYPE& GetAttackType(void)const { return attackType_; }


	/// @brief 方向をモデルの前方向にセット
	/// @param _trans モデル情報 
	void SetMoveDirTransformFront(const Transform& _trans) { moveDir_ = _trans.GetForward(); }

	/// @brief ターゲットの方向を見る
    virtual const VECTOR& GetLookAtTargetDir(void)const;
    virtual const float& GetLookAtTargetDeg(void)const;

    void SetDegAndDir(const float& _deg, const VECTOR& _dir)
    {
        moveDeg_ = _deg;
        moveDir_ = _dir;
	}

	/// @brief アクション中フラグセット
	/// @param  
	void IsActioningSet(void) { isActioning_ = false; }


#ifdef _DEBUG
    virtual void DebugDraw(void) {};
#endif // _DEBUG

protected:

	//操作管理用
	ACT_CNTL actCntl_;
    //移動関連
	float moveDeg_;             //移動角度
	VECTOR moveDir_;            //移動方向ベクトル

    //攻撃種類
    ENEMY_ATTACK_TYPE attackType_;

    //アクション入力がされたかどうか
    ACT_FLAG isAct_;

    //アクション中
	bool isActioning_;

};

