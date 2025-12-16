#pragma once
#include <DxLib.h>
#include <functional>
#include <map>
#include "../../Common/Quaternion.h"
#include "../../Object/ObjectBase.h"
#include "../Template/Singleton.h"
#include "../Common/Easing.h"
class Transform;
class Easing;
class Camera:public ObjectBase
{
	friend class Singleton<Camera>;
public:

	// カメラスピード(度)
	static constexpr float SPEED = 1.0f;
	static constexpr float SPEED_PAD = 0.0015f;	//カメラのスピードパッド時
	// カメラクリップ：NEAR
	static constexpr float CAMERA_NEAR = 10.0f;

	// カメラクリップ：FAR
	static constexpr float CAMERA_FAR = 30000.0f;

	// カメラの初期座標
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// 追従位置からカメラ位置までの相対座標
	static constexpr VECTOR LOCAL_F2C_POS = { 0.0f, 100.0f, -800.0f };
	static constexpr VECTOR TARGET_CAM_LOCAL_F2C_POS = { 0.0f, 500.0f, -700.0f };

	// 追従位置から注視点までの相対座標
	static constexpr VECTOR LOCAL_F2T_POS = { 0.0f, 0.0f, 200.0f };

	// カメラのX回転上限度角
	static constexpr float LIMIT_X_UP_RAD = 60.0f * (DX_PI_F / 180.0f);
	//static constexpr float LIMIT_X_DW_RAD = 5.0f * (DX_PI_F / 180.0f);
	static constexpr float LIMIT_X_DW_RAD = -30.0f * (DX_PI_F / 180.0f);

	//ターゲットカメラ遷移時の補完時間
	static constexpr double CHANGE_TARGET_LERP_TIME = 0.7;

	//カメラ感度
	static constexpr float FOV_PER = 0.2f;

	//1シェイクにかかる時間
	static constexpr float SHAKE_PER = 0.1f;
	
	//当たり判定の半径
	static constexpr float HIT_RADIUS = 30.0f;

	//当たった時の押し出し回数
	static constexpr int COL_TRY_CNT_MAX = 10;

	//ヒットした法線方向へのオフセット
	static constexpr float HIT_NORMAL_OFFSET = 2.0f;

	// 衝突時の押し戻し試行回数
	static constexpr int CNT_TRY_COLLISION_CAMERA = 30;

	// 衝突判定用球体半径
	static constexpr float COL_CAPSULE_SPHERE = 50.0f;
	static constexpr float COL_SPHERE_SPHERE = 40.0f;

	// 衝突時の押し戻し量
	static constexpr float COLLISION_BACK_DIS = 2.0f;





	// カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,
		FOLLOW,
		SELF_SHOT,
		CHANGE_TARGET,
		TARGET_POINT
	};

	//イージングモード
	enum class SUB_MODE
	{
		NONE,		//何もない
		SHAKE,		//連続シェイク
		ONE_SHAKE,	//一回のみシェイク
	};

	Camera(void);
	~Camera(void);

	//当たり判定配列の格納
	void MakeColliderGeometry(void);

	void Init(void)override;
	void Update(void)override;
	void SetBeforeDraw(void);
	void Draw(void)override;

	// カメラ位置
	VECTOR GetPos(void) const;
	// カメラの操作角度
	VECTOR GetAngles(void) const;
	// カメラの注視点
	VECTOR GetTargetPos(void) const;

	// カメラ角度
	Quaternion GetQuaRot(void) const;
	// X回転を抜いたカメラ角度
	Quaternion GetQuaRotOutX(void) const;
	// カメラの前方方向
	VECTOR GetForward(void) const;

	// カメラモードの変更
	void ChangeMode(const MODE mode);
	
	/// @brief サブ処理の変更
	/// @param _submode サブ処理(イージングなど)
	void ChangeSub(const SUB_MODE _submode);

	// 追従対象の設定
	void SetFollow(const Transform* _follow,const VECTOR _localCenterPos);

	//ターゲットとする対象の設定
	void SetTarget(const Transform* _target);

	//ターゲットの座標セット
	void SetTargetPos(const VECTOR _targetPos);

	/// @brief シェイク時にセットするカウント(割合)
	/// @param t 現在の時間割合
	/// @param limit 範囲の加減
	void SetShakeStatus(const float t,const float limit=0.0f,const Easing::EASING_TYPE _easeType=Easing::EASING_TYPE::COS_BACK)
	{
		easePer_ = t;
		initLimit_ = limit;
		easeType_ = _easeType;
	}

	void SetStageTransform(const Transform* _stageTrans)
	{
		stageTransform_ = _stageTrans;
	}

private:

	//追従対象のフレームナンバー
	static constexpr int FOLLOW_FRAME_NUM = 1;

	// カメラが追従対象とするTransform
	const Transform* followTransform_;
	VECTOR followLocalCenterPos_;

	//カメラの注視点とするターゲットTransform
	const Transform* targetTransform_;

	//ターゲットの座標(フレーム座標)
	VECTOR targetPoses_;

	//イージング
	std::unique_ptr<Easing>easing_;

	//ステージのTransform
	const Transform* stageTransform_;

	// カメラモード
	MODE mode_;
	//サブモード
	SUB_MODE subMode_;
	//カメラ更新
	std::function<void(void)>modeUpdate_;
	//イージングなど、、同時に動かしたい更新
	std::function<void(void)>subUpdate_;

	//遷移
	std::map<MODE, std::function<void(void)>>changeMode_;
	std::map<SUB_MODE, std::function<void(void)>>changeSub_;

	// カメラの位置
	VECTOR pos_;

	//移動後座標
	VECTOR goalPos_;

	// カメラ角度(rad)
	VECTOR angles_;

	// X軸回転が無い角度
	Quaternion rotOutX_;

	// カメラ角度
	Quaternion rot_;

	// 注視点
	VECTOR targetPos_;

	// カメラの上方向
	VECTOR cameraUp_;
	
	//カメラシェイクのステータス
	Easing::EASING_TYPE easeType_;	//イージングの種類
	float shekePerCnt_;//1シェイクのカウント
	float initLimit_;	//初めに設定する範囲
	float limit_;		//動かす範囲
	float easePer_;		//シェイク全体時間
	float initPosY_;		//動かす前のカメラ座標Y
	//ターゲットカメラ遷移カウント
	double changeTargetLerpCnt_;

	//ターゲットカメラ遷移中フラグ
	bool isChangingCamera_;

	//追従対象フレーム座標
	VECTOR followFramePos_;
	//追従対象の中心座標
	VECTOR followCenterPos_;

	/// @brief 当たったときの処理
	/// @param _hitCol ヒットしたコライダ
	virtual void OnHit(const std::weak_ptr<Collider> _hitCol)override;

	//カメラ当たり判定の線分更新
	void UpdateCameraColliderLine(void);

	// カメラを初期位置に戻す
	void SetDefault(void);

	// 追従対象との位置同期を取る
	void SyncFollow(void);

	//ターゲットカメラの追従
	void SyncTargetFollow(void);

	// カメラ操作
	void ProcessRot(void);

	//スムーズにターゲットカメラに変わる
	void SmoothChangeCamera(void);

	/// @brief あたり判定
	/// @param  
	void Collision(void);

	// モード別更新ステップ
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFollow(void);
	void SetBeforeDrawSelfShot(void);
	void SetBeforeDrawLerpCamera(void);
	void SetBeforeDrawTargetPoint(void);

	//遷移
	void ChangeFixedPoint(void);
	void ChangeFollow(void);
	void ChangeSelfShot(void);
	void ChangeTargetLerp(void);
	void ChangeTargetCamera(void);

	//サブモード別更新
	void UpdateNone(void);
	void UpdateShake(void);
	void UpdateShakeOne(void);

	//遷移
	void ChangeNone(void);
	void ChangeShake(void);
	void ChanegShakeOne(void);
};

