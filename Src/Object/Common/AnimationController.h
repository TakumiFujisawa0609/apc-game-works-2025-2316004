#pragma once
#include<DxLib.h>
#include <string>
#include <map>
#include "../Common/Easing.h"

class Easing;
class SceneManager;

class AnimationController
{
	
public :

	// アニメーションデータ
	struct Animation
	{
		int model = -1;
		int attachNo = -1;
		int animIndex = 0;
		float speed = 0.0f;
		float totalTime = 0.0f;
		float step = 0.0f;
		VECTOR firstPos = {};
		VECTOR movePow = {};
	};

	// コンストラクタ
	AnimationController(const int _modelId,const int _hipNum);
	// デストラクタ
	~AnimationController(void);

	// アニメーション追加
	void Add(int type, const float speed, int modelId);

	// アニメーション再生
	void Play(int type, bool isLoop = true, 
		float startStep = 0.0f, float endStep = -1.0f, bool isStop = false, bool isForce = false);

	//アニメーション更新 
	void Update(const float _spdScl=1.0f);

	// アニメーション終了後に繰り返すループステップ
	void SetEndLoop(float startStep, float endStep, float speed);

	//アニメーションの途中からのループ再生
	void SetMidLoop(const float startStep,const float endStep,float _spd);

	//アニメーション途中ループ終了
	void SetEndMidLoop(const float _spd);

	// 再生中のアニメーション
	int GetPlayType(void) const;

	//アニメーションステップゲッタ
	const float GetAnimStep(void)const;

	/// @brief アニメーションスピードセッタ(イージングで、だんだん増やしていくのも可)
	/// @param _spd セットしたいスピード
	/// @param _isEase イージングを使用するか
	/// @param _startSpd イージング使用時の始まりのスピード
	/// @param _t 時間
	/// @param _easeType 使用したいイージングタイプ
	void SetAnimSpeed(const float _spd, const bool _isEase = false, const float _startSpd=0.0f, const float _t = 1.0f, Easing::EASING_TYPE _easeType = Easing::EASING_TYPE::LERP);

	// 再生終了
	bool IsEnd(void) const;

	/// @brief フレームの行列ローカル座標のセット
	/// @param _modelId モデルID
	/// @param frameIdx フレーム番号
	/// @param pos フレーム座標
	void SetFrameLocalMatrixPos(const int _modelId, const int frameIdx, VECTOR& pos);

	/// @brief アニメーションのフレーム位置をセット
	/// @param modelId モデルID
	/// @param attachNo アタッチ番号
	/// @param frameIdx フレーム番号
	/// @param pos 座標
	void SetFrameAnimAttachLocalMatrixPos(int modelId, int attachNo, int frameIdx, VECTOR& pos);

	/// @brief フレーム行列情報の取得
	/// @param _modelId モデルID
	/// @param _frameIdx フレーム番号
	/// @param _scl スケール
	/// @param _matRot 行列角度
	/// @param _pos 座標
	void GetFrameLocalMatrix(const int _modelId, int _frameIdx, VECTOR& _scl, MATRIX& _matRot, VECTOR& _pos);

	/// @brief // 指定アニメーションのフレーム(ボーン)のローカル行列を取得し、
			   // 拡大縮小・回転・移動成分に分解して出力する
	/// @param modelId モデルID
	/// @param attachNo アタッチ番号
	/// @param frameIdx フレーム番号
	/// @param scl スケール
	/// @param matRot 行列角度
	/// @param pos 座標
	void GetFrameAnimAttachLocalMatrix(int modelId, int attachNo, int frameIdx, VECTOR& scl, MATRIX& matRot, VECTOR& pos);

private :
	//ヒップフレームの番号
	static constexpr int HIP_FRAME_NO = 0;

	//イージング
	std::unique_ptr<Easing>easing_;
	// モデルのハンドルID
	int modelId_;

	// 種類別のアニメーションデータ
	std::map<int, Animation> animations_;

	int playType_;
	Animation playAnim_;

	// アニメーションをループするかしないか
	bool isLoop_;

	// アニメーションを止めたままにする
	bool isStop_;

	//途中ループフラグ
	bool isMidLoop_;

	//イージング使用中フラグ
	bool isEase_;

	// アニメーション終了後に繰り返すループステップ
	float stepEndLoopStart_;
	float stepEndLoopEnd_;
	float endLoopSpeed_;

	// 逆再生
	float switchLoopReverse_;

	//モデルのヒップ番号
	int hipNum_;

};

