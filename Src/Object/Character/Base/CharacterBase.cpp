#include "../Manager/Generic/SceneManager.h"
//スマートポインタをデストラクタで解放するためのインクルード
#include "../Object/Common/AnimationController.h"
#include "../Object/Character/Player/ActionController.h"
#include "../Object/Character/Base/LogicBase.h"
#include "../Object/Character/Base/CharacterOnHitBase.h"
#include "../Manager/Game/GravityManager.h"
#include"../Object/Card/CardDeck.h"
#include"../Object/Card/CardUIBase.h"
#include "../Object/Stage.h"
#include "../../../Utility/Utility3D.h"
#include "../../Common/Geometry/Capsule.h"
#include "../../Common/Geometry/Sphere.h"
#include "../../Common/Geometry/Line.h"

#include "../Object/ObjectBase.h"
#include "CharacterBase.h"

CharacterBase::CharacterBase(void) :
	movedPos_(Utility3D::VECTOR_ZERO),
	moveDiff_(Utility3D::VECTOR_ZERO),
	jumpPow_({Utility3D::VECTOR_ZERO})
{
}

CharacterBase::~CharacterBase(void)
{
}

void CharacterBase::MakeAttackCol(const Collider::TAG _charaTag,const Collider::TAG _attackTag, const VECTOR& _atkPos, const float& _radius)
{
	//当たり判定が存在したら削除する
	if (IsAliveCollider(_charaTag, _attackTag))return;
	std::unique_ptr<Sphere>sphere = std::make_unique<Sphere>(_atkPos, _radius);
	isDamage_ = false;
	MakeCollider(TAG_PRIORITY::ATK_SPHERE,{ _charaTag,_attackTag }, std::move(sphere),{Collider::TAG::STAGE});
}

void CharacterBase::UpdateAttackCol(const float _radius)
{
	//引数の半径を攻撃の球に設定する
	Sphere& sphere=dynamic_cast<Sphere&>(collider_[TAG_PRIORITY::ATK_SPHERE]->GetGeometry());
	sphere.SetRadius(_radius);
}

void CharacterBase::DeleteAttackCol(const Collider::TAG& _charaTag, const Collider::TAG& _attackCol)
{
	if (!IsAliveCollider(_charaTag, _attackCol))return;
	DeleteCollider(TAG_PRIORITY::ATK_SPHERE);
}
//
//void CharacterBase::CardInit(void)
//{
//	deck_ = std::make_shared<CardDeck>(cardCenterPos_, PLAYER_NUM);
//
//	cardUI_ = std::make_unique<PlayerCardUI>();
//	cardUI_->Load();
//
//	//デッキの先頭にリロードカード追加
//	deck_->AddDrawPile(RELOAD_CARD_STATUS);
//	cardUI_->AddCardUi(RELOAD_CARD_STATUS);
//	//デッキに山札追加
//	for (int i = 0; i < CARD_NUM_MAX; i++)
//	{
//		deck_->AddDrawPile(CARD_POWS[i]);
//		cardUI_->AddCardUi(CARD_POWS[i]);
//	}
//	deck_->Load();
//}

void CharacterBase::UpdatePost(void)
{
	//移動後座標の更新
	movedPos_ = VAdd(trans_.pos, action_->GetMovePow());
	//movedPos_ = VAdd(trans_.pos, movePow_);
	//ジャンプ力の追加
	//movedPos_ = VAdd(movedPos_, action_->GetJumpPow());
	movedPos_ = VAdd(movedPos_, jumpPow_);

	//移動量ラインの更新
	VECTOR moveVec = VSub(movedPos_, trans_.pos);
	moveVec.y -= MOVE_LINE_Y_OFFSET;
	if (moveVec.x != 0.0f || moveVec.y != MOVE_LINE_Y_CHECK_VALUE || moveVec.z != 0.0f)
	{
		Line& moveLine = dynamic_cast<Line&>(collider_[TAG_PRIORITY::MOVE_LINE]->GetGeometry());
		moveLine.SetLocalPosPoint1(Utility3D::VECTOR_ZERO);
		moveLine.SetLocalPosPoint2(moveVec);
	}

	//当たり判定をする前に初期化する
	onHit_->InitHitPoint();

	MoveLimit(Stage::STAGE_POS, { Stage::STAGE_SIZE,0.0f, Stage::STAGE_SIZE });
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

void CharacterBase::SetStatus(const float& _spd, const float& _hp, const float& _atk, const float& _def)
{
	//最大値をセット
	maxStatus_ = { _spd,_hp,_atk,_def };

	//現在ステータスを最大値にセット
	status_ = maxStatus_;
}

void CharacterBase::MoveLimit(const VECTOR& _stagePos,const VECTOR& _stageSize)
{
	VECTOR subRadiusSize={_stageSize.x- capRadius_,0.0f,_stageSize.z- capRadius_};
	VECTOR sizeHalf = VScale(subRadiusSize, 0.5f);
	VECTOR limit = VAdd(_stagePos, sizeHalf);
	VECTOR moveVec = Utility3D::GetMoveVec(trans_.pos, movedPos_);
	VECTOR addPos = VScale(moveVec, capRadius_);
	VECTOR limitPos = VAdd(trans_.pos, addPos);

	VECTOR pushVec = Utility3D::GetMoveVec(movedPos_, moveDiff_);
	pushVec.y = 0.0f;
	VECTOR pushPow = VScale(pushVec, action_->GetSpd());
	if (limitPos.x >= limit.x)
	{
		movedPos_.x = limit.x + pushPow.x;
	}
	if (limitPos.x <= -limit.x)
	{
		movedPos_.x = -limit.x + pushPow.x;
	}
	if (limitPos.z >= limit.z)
	{
		movedPos_.z = limit.z + pushPow.z;
	}
	if (limitPos.z <= -limit.z)
	{
		movedPos_.z = -limit.z + pushPow.z;
	}

}

void CharacterBase::MoveDirFromInput(void)
{
}

void CharacterBase::SetGoalRotate(const double _deg)
{
}

void CharacterBase::Rotate(void)
{
	if (charaRot_.stepRotTime_ <= 0.0f)return;
	charaRot_.stepRotTime_ -= scnMng_.GetDeltaTime();
	// 回転の球面補間
	charaRot_.playerRotY_ = Quaternion::Slerp(
		charaRot_.playerRotY_, charaRot_.goalQuaRot_, (TIME_ROT - charaRot_.stepRotTime_) / TIME_ROT);
	//charaRot_.playerRotY_ = charaRot_.goalQuaRot_;
}

void CharacterBase::Damage(const int _dam)
{
	status_.hp -= _dam;
}

const VECTOR& CharacterBase::GetCharaCenterPos(void) const
{
	return collider_.at(ObjectBase::TAG_PRIORITY::BODY)->GetGeometry().GetCenter();
}

void CharacterBase::SetFlinchCnt(const float _flichCnt)
{
	action_->SetFlinchCnt(_flichCnt);
}

const CardActionBase::CARD_ACT_TYPE& CharacterBase::GetCardAction(void) const
{
	return action_->GetCardAction();
}


const ActionBase& CharacterBase::GetMainAction(void) const
{
	return action_->GetMainAction();
}

void CharacterBase::DeleteCard(void)
{
	//現在使っているカードを捨てる
	deck_->EraseHandCard();
	//手札に移動
	deck_->MoveUsingCardToDrawPile();
	cardUI_->ChangeUsedActionCard();
	//charaObj_.GetCardUI().ChangeSelectState(CardUI::CARD_SELECT::DISITION);
}

const CharacterOnHitBase::HIT_POINT& CharacterBase::GetHitPoint(void) const
{
	return onHit_->GetHitPoint();
}

void CharacterBase::MovedPosMove(const VECTOR& _vec, const float& _movePow)
{
	movedPos_ = VAdd(movedPos_, VScale(_vec, _movePow));
}

void CharacterBase::LariatMove(const float& _deg)
{
	charaRot_.playerRotY_ = Quaternion::AngleAxis(UtilityCommon::Deg2RadF(_deg), Utility3D::AXIS_Y);
}

void CharacterBase::SetLogicTargetCharacter(std::shared_ptr<CharacterBase> _targetChara)
{
	logic_->SetTargetCharacter(_targetChara);
}
