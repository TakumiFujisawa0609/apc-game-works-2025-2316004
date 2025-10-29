#include "../Manager/Generic/SceneManager.h"
//スマートポインタをデストラクタで解放するためのインクルード
#include "../Object/Common/AnimationController.h"
#include "../Object/Character/Player/ActionController.h"
#include "../Object/Character/Base/LogicBase.h"
#include "../Object/Character/Player/PlayerOnHit.h"
#include"../Object/Card/CardDeck.h"
#include"../Object/Card/CardUI.h"
#include "../../../Utility/Utility3D.h"
#include "../../Common/Geometry/Capsule.h"
#include "../../Common/Geometry/Sphere.h"
#include "../../Common/Geometry/Line.h"

#include "../Object/ObjectBase.h"
#include "CharacterBase.h"

CharacterBase::CharacterBase(void) :
	movedPos_(Utility3D::VECTOR_ZERO),
	moveDiff_(Utility3D::VECTOR_ZERO)
{
}

CharacterBase::~CharacterBase(void)
{
}

void CharacterBase::MakeAttackCol(const Collider::TAG _charaTag, const VECTOR& _atkPos, const float& _radius)
{
	//当たり判定が存在したら削除する
	if (IsAliveCollider(_charaTag, Collider::TAG::SWORD))return;
	std::unique_ptr<Sphere>sphere = std::make_unique<Sphere>(_atkPos, _radius);

	isDamage_ = false;
	MakeCollider({ _charaTag,Collider::TAG::SWORD }, std::move(sphere),{Collider::TAG::STAGE});
}

void CharacterBase::UpdateAttackCol(const float _radius)
{
	//引数の半径を攻撃の球に設定する
	Sphere* sphere=dynamic_cast<Sphere*>(colParam_[ATK_COL_NO].geometry_.get());
	sphere->SetRadius(_radius);
}

void CharacterBase::DeleteAttackCol(const Collider::TAG _charaTag)
{
	if (!IsAliveCollider(_charaTag, Collider::TAG::SWORD))return;
	DeleteCollider(ATK_COL_NO);
}

void CharacterBase::UpdatePost(void)
{
	//移動後座標の更新
	movedPos_ = VAdd(trans_.pos, action_->GetMovePow());
	//ジャンプ力の追加
	movedPos_ = VAdd(movedPos_, action_->GetJumpPow());

	//移動量ラインの更新
	VECTOR moveVec = VSub(movedPos_, trans_.pos);
	moveVec.y -= MOVE_LINE_Y_OFFSET;
	if (moveVec.x != 0.0f || moveVec.y != MOVE_LINE_Y_CHECK_VALUE || moveVec.z != 0.0f)
	{
		Line& moveLine = dynamic_cast<Line&>(colParam_[MOVE_LINE_COL_NO].collider_->GetGeometry());
		moveLine.SetLocalPosPoint1(Utility3D::VECTOR_ZERO);
		moveLine.SetLocalPosPoint2(moveVec);
	}

	//当たり判定をする前に初期化する
	onHit_->InitHitPoint();

	//
	////地面接地ライン
	//if (movedPos_.y < 0.0f)
	//{
	//	movedPos_.y = 0.0f;
	//}

	//移動前の座標を格納する
	moveDiff_ = trans_.pos;
	//移動
	trans_.pos = movedPos_;
}

void CharacterBase::MoveDirFronInput(void)
{
}

void CharacterBase::SetGoalRotate(const double _deg)
{
}

void CharacterBase::Rotate(void)
{
	charaRot_.stepRotTime_ -= scnMng_.GetDeltaTime();
	// 回転の球面補間
	charaRot_.playerRotY_ = Quaternion::Slerp(
		charaRot_.playerRotY_, charaRot_.goalQuaRot_, (TIME_ROT - charaRot_.stepRotTime_) / TIME_ROT);
	//charaRot_.playerRotY_ = charaRot_.goalQuaRot_;
}

void CharacterBase::Damage(const int _dam)
{
	status_.hp_ -= _dam;
}

void CharacterBase::DeleteCard(void)
{
	//現在使っているカードを捨てる
	deck_->EraseHandCard();
	//手札に移動
	deck_->MoveHandToCharge();
	cardUI_->ChangeUsedActionCard();
	//charaObj_.GetCardUI().ChangeSelectState(CardUI::CARD_SELECT::DISITION);
}

const PlayerOnHit::HIT_POINT& CharacterBase::GetHitPoint(void) const
{
	return onHit_->GetHitPoint();
}
