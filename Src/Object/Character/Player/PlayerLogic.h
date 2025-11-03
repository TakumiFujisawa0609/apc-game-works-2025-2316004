#pragma once
#include<DxLib.h>
#include<memory>
#include"../Manager/Generic/SceneManager.h"
#include"../Manager/Generic/InputManager.h"
#include"../Manager/Generic/InputManagerS.h"
#include"../Utility/Utility3D.h"
#include"../Base/LogicBase.h"
//#define DEBUG_ON
class PlayerBase;
class Command;

class PlayerLogic:public LogicBase
{
public:
    //各アクション操作ボタン
    using JOYPAD_BTN = InputManager::JOYPAD_BTN;
    //移動
    static constexpr int MOVE_FRONT_KEY = KEY_INPUT_W;      //前
    static constexpr int MOVE_LEFT_KEY = KEY_INPUT_A;       //左
    static constexpr int MOVE_BACK_KEY = KEY_INPUT_S;       //後ろ
    static constexpr int MOVE_RIGHT_KEY = KEY_INPUT_D;      //右

    //カード選択
    //右
    static constexpr int CARD_MOVE_RIGHT_KEY = KEY_INPUT_E;
    static constexpr JOYPAD_BTN CARD_MOVE_RIGHT_BTN = JOYPAD_BTN::R_BUTTON;
    static constexpr int CARD_MOVE_LEFT_KEY = KEY_INPUT_Q;
    static constexpr JOYPAD_BTN CARD_MOVE_LEFT_BTN = JOYPAD_BTN::L_BUTTON;
    //カード使用キー
    static constexpr int CARD_USE_KEY = KEY_INPUT_F;
    static constexpr JOYPAD_BTN CARD_USE_BTN = JOYPAD_BTN::RIGHTBUTTON_RIGHT;

    //カードチャージ
    static constexpr int CARD_CHARGE_KEY = KEY_INPUT_R;
    static constexpr JOYPAD_BTN CARD_CHARGE_BTN = JOYPAD_BTN::LEFTBUTTON_TOP;

    //移動判定になるスティックの大きさ
    static constexpr int STICK_MOVE_SIZE_MIN = 200;

    //ダッシュ
    //static constexpr int DASH_KEY = KEY_INPUT_L;

    ////パンチ
    //static constexpr int PUNCH_KEY = KEY_INPUT_RETURN;
    //static constexpr InputManager::JOYPAD_BTN PUNCH_BTN = InputManager::JOYPAD_BTN::RIGHTBUTTON_LEFT;

    //ジャンプ
    static constexpr int JUMP_KEY = KEY_INPUT_SPACE;
    static constexpr InputManager::JOYPAD_BTN JUMP_BTN = InputManager::JOYPAD_BTN::RIGHTBUTTON_DOWN;

    static constexpr float STICK_MARGIN_DEG = 90.0f;

    //角度関連
    static constexpr float FLONT_DEG = 0.0f;
    static constexpr float LEFT_DEG = 270.0f;
    static constexpr float BACK_DEG = 180.0f;
    static constexpr float RIGHT_DEG = 90.0f;

    //デルタタイム
    static constexpr float DELTA_TIME = 1.0f / 60.0f;

    ////アクションボタンの種類
    //enum class ACT_CNTL
    //{
    //    NONE            //何もしていないとき
    //    , MOVE          //移動
    //    , DASHMOVE      //ダッシュ
    //    , PUNCH         //パンチ
    //    , JUMP          //ジャンプ
    //    , CARD_CHARGE   //カードチャージ
    //    ,CARD_ACTION      //カード使用
    //    ,CARD_MOVE_LEFT //カード左
    //    ,CARD_MOVE_RIGHT//カード右
    //};

    enum class CNTL
    {
        NONE,
        KEYBOARD,
        PAD
    };

    /// @brief コンストラクタ
    /// @param _padNum パッド番号
    /// @param _cntl 操作タイプ
    PlayerLogic(InputManager::JOYPAD_NO _padNum, InputManager::CONTROLL_TYPE _cntl);
    ~PlayerLogic(void)override;

	/// @brief 変更時の初期化
	/// @param  
	void Init(void)override;
    
    /// @brief 更新
    /// @param  
    void Update(void)override;

    ////コントロール判定
    //bool CheckAct(ACT_CNTL _actCntl) { return actCntl_ == _actCntl ? true : false; }
#ifdef _DEBUG
#endif // _DEBUG_ON

 

    //ゲッタ
    //----------------------------------------------------------------
    ACT_CNTL GetAct(void) { return actCntl_; }
    float GetStickDeg(void){ return stickDeg_; }
private:
    //入力デバイス別の更新
    void InputKeyBoard(void);   //キーボード
    void InputAll(void);        //両対応
    void InputPad(void);        //パッド

    void KeyBoard(void);
    void Pad(void);

    /// @brief 一定の大きさ以上スティックが動いているか
    /// @param  
    /// @return 
    bool IsStickMoved(void);

    //スティック
    float leftStickX_;          //スティックの角度X
    float leftStickY_;          //スティックの角度Y

    //スティックの角度(0～1000を返す)
    Vector2 LStickAngleSize_;
    float stickDeg_;            //パッドのスティックの角度

    InputManager::CONTROLL_TYPE cntl_;              //入力デバイス
    InputManager::JOYPAD_NO padNum_;                //パッド番号

    std::map<InputManager::CONTROLL_TYPE, std::function<void(void)>>inputUpdates_;

};

