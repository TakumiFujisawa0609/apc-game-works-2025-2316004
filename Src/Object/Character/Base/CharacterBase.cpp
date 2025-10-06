//スマートポインタをデストラクタで解放するためのインクルード
#include"../Object/Common/AnimationController.h"
#include"../Object/Character/Player/ActionController.h"
#include"../Object/Character/Base/LogicBase.h"
#include"../Object/Character/Player/PlayerOnHit.h"
#include"../../../Utility/Utility3D.h"
#include"../../Common/Geometry/Capsule.h"
#include"../../Common/Geometry/Sphere.h"
#include"../../Common/Geometry/Line.h"

#include "CharacterBase.h"

CharacterBase::CharacterBase(void) :
	movedPos_(Utility3D::VECTOR_ZERO),
	moveDiff_(Utility3D::VECTOR_ZERO)
{
}

CharacterBase::~CharacterBase(void)
{
}

void CharacterBase::MakeAttackCol(const Collider::TAG _charaTag)
{
	VECTOR localPos = {0.0f,30.0f,0.0f};
	VECTOR atkPos = Utility3D::AddPosRotate(trans_.pos, trans_.quaRot, localPos);
	std::unique_ptr<Sphere>sphere = std::make_unique<Sphere>(atkPos, CAP_RADIUS);
	MakeCollider({ _charaTag,Collider::TAG::SWORD }, std::move(sphere));
}

void CharacterBase::UpdatePost(void)
{
	//移動後座標の更新
	movedPos_ = VAdd(trans_.pos, action_->GetMovePow());
	//movedPos_ = VAdd(movedPos_, action_->GetJumpPow());

	//移動量ラインの更新
	VECTOR moveVec = VSub(movedPos_, trans_.pos);
	moveVec.y -= MOVE_LINE_Y_OFFSET;
	if (moveVec.x != 0.0f || moveVec.y != MOVE_LINE_Y_CHECK_VALUE || moveVec.z != 0.0f)
	{
		Line& moveLine = dynamic_cast<Line&>(colParam_[MOVE_LINE_COL_NO].collider_->GetGeometry());
		moveLine.SetLocalPosPoint1(Utility3D::VECTOR_ZERO);
		moveLine.SetLocalPosPoint2(moveVec);
	}

	//移動前の座標を格納する
	moveDiff_ = trans_.pos;
	//移動
	trans_.pos = movedPos_;
}
