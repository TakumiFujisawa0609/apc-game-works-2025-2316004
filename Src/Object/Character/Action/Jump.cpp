#include "../Base/ActionBase.h"
#include"../../../Utility/Utility3D.h"
#include"../../../Manager/Generic/SceneManager.h"
#include "Jump.h"

Jump::Jump(InputBase& _input):
	ActionBase(_input)
{
}

Jump::~Jump(void)
{
}

void Jump::Init(void)
{
}

void Jump::Update(void)
{
	//ステップジャンプを基準にジャンプ減衰量を決める
	float deltaTime = scnMng_.GetDeltaTime();
	stepJump_ += deltaTime;

	////空中アニメーションステップのループ設定
	//animationController_.SetEndLoop(JUMP_ANIM_LOOP_START_FRAME
	//	, JUMP_ANIM_LOOP_END_FRAME, JUMP_ANIM_ATTACK_BLEND_TIME);

	//ジャンプ中も移動できるようにする
	//MoveDirFronInput();

	//ジャンプカウントが0以上なら
	if (stepJump_ > 0.0f)
	{
		stepJump_ += deltaTime;
		//プレイヤーが落下していたら
		if (jumpDeceralation_ < 0.0f)
		{
			//animationController_.Play(static_cast<int>(Player::ANIM_TYPE::LAND));
		}
		//減衰量の計算
		float deceralation = stepJump_ * TIME_JUMP_SCALE;
		jumpDeceralation_ -= deceralation;

		//ジャンプ量に掛ける
		//jumpPow_ = VScale(player_.GetTransform().GetUp(), jumpDeceralation_);
	}

	//地面に着いたらジャンプ関係の変数リセット
	if (!isJump_)
	{
		jumpDeceralation_ = POW_JUMP;
		jumpPow_ = Utility3D::VECTOR_ZERO;
		stepJump_ = 0.0f;

		//着地エフェクト
		//Transform trans = player_.GetTransform();
		const float EFF_SCL = 30.0f;

		//動いていた場合の移動量リセット
		speed_ = 0.0f;
		//ChangeAction(ATK_ACT::INPUT);
		return;
	}
}