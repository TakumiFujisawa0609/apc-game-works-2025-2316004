//#include "....//Manager/System/SoundManager.h"
#include <ranges>
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Object/Common/Geometry/Sphere.h"
#include "../../../Object/Common/Geometry/Capsule.h"
#include "../../../Object/Common/Geometry/Line.h"
#include"../../../Object/Common/Geometry/Model.h"
#include "../Object/Character/Base/CharacterBase.h"
#include"../../../Utility/Utility3D.h"
#include"./ActionController.h"

#include"./Player.h"
#include "../Base/CharacterOnHitBase.h"
#include "PlayerOnHit.h"
namespace
{
	using TAG_PRIORITY = ObjectBase::TAG_PRIORITY;
	using TAG = Collider::TAG;
}
PlayerOnHit::PlayerOnHit(CharacterBase& _chara, VECTOR& _movedPos,VECTOR& _moveDiff,
	ActionController& _action, std::map<ObjectBase::TAG_PRIORITY, std::shared_ptr<Collider>>& _colParam, Transform& _trans):
	CharacterOnHitBase(_chara,_movedPos,_moveDiff,_action,_colParam,_trans)
{
	//それぞれの当たった処理を格納する
	using TAG = Collider::TAG;
	colUpdates_={
		{ TAG::ENEMY1, [this](const std::weak_ptr<Collider> _hitCol) {CollChara(_hitCol); } },
		{ TAG::NML_ATK, [this](const std::weak_ptr<Collider> _hitCol) {CollNormalAttack(_hitCol); } },
		{ TAG::STAGE, [this](const std::weak_ptr<Collider>_hitCol) {CollStage(_hitCol); } },
		{ TAG::ROAR_ATK, [this](const std::weak_ptr<Collider>_hitCol) {CollRoarAttack(_hitCol); } },
		{ TAG::ROCK, [this](const std::weak_ptr<Collider>_hitCol) {CollRoarAttack(_hitCol); } },
	};

}

PlayerOnHit::~PlayerOnHit(void)
{
	colUpdates_.clear();
	colParam_.clear();
}

void PlayerOnHit::Load(void)
{
	
}

void PlayerOnHit::Init(void)
{
	hitPoint_ = {};
	bodyRadius_ = Player::RADIUS;
	movedPos_ = Utility3D::VECTOR_ZERO;
}


void PlayerOnHit::CollChara(const std::weak_ptr<Collider> _hitCol)
{
	//相手のタグをとる
	CharacterBase& parentChara = _hitCol.lock()->GetParentCharacter();
	std::set<Collider::TAG> tags = _hitCol.lock()->GetTags();
	
	//攻撃の場合は無視
	if (std::ranges::any_of(tags, [](Collider::TAG tag) { return tag == Collider::TAG::NML_ATK || tag == Collider::TAG::ROAR_ATK; })
		||charaObj_.GetCardAction()==ActionBase::CARD_ACT_TYPE::RUSH_ATK)
	{
		return;
	}
	Geometry& myCap = colParam_[TAG_PRIORITY::BODY]->GetGeometry();
	Geometry& hitCap = _hitCol.lock()->GetGeometry();
	//自分の座標
	VECTOR myPos = charaObj_.GetTransform().pos;
	VECTOR hitCharaPos = parentChara.GetTransform().pos;

	//お互いの距離をとる
	float dis = static_cast<float>(Utility3D::Distance(myPos, hitCharaPos));
	float minDist = myCap.GetRadius() + hitCap.GetRadius();

	//お互いの距離が離れていたら押し出さない
	if (dis > minDist)return;

	//押し出し量の計算
	float pushPow = abs(minDist - dis);

	//押し出す方向ベクトルの計算
	VECTOR vec = Utility3D::GetMoveVec(parentChara.GetTransform().pos,charaObj_.GetTransform().pos );
	//Y成分はいらない
	vec.y = 0.0f;

	//押し出し
	trans_.pos = VAdd(moveDiff_, VScale(vec, pushPow));
}

void PlayerOnHit::CollNormalAttack(const std::weak_ptr<Collider> _hitCol)
{

	auto& parentChara = _hitCol.lock()->GetParentCharacter();
	if (parentChara.GetIsDamage())return;
	
	auto tag = _hitCol.lock()->GetParentCharacter().GetCharaTag();
	//ダメージを与えたことを知らせる
	parentChara.SetIsDamage();
	charaObj_.Damage(20);
	action_.ChangeAction(ActionController::ACTION_TYPE::REACT);
	
}


void PlayerOnHit::CollRoarAttack(const std::weak_ptr<Collider> _hitCol)
{
	auto& parentChara = _hitCol.lock()->GetParentCharacter();
	if (parentChara.GetIsDamage())return;

	auto tag = _hitCol.lock()->GetParentCharacter().GetCharaTag();
	//ダメージを与えたことを知らせる
	parentChara.SetIsDamage();
	charaObj_.Damage(20);

	//のけぞり時間セット
	charaObj_.SetFlinchCnt(ROAR_FLICTION_TIME);
	action_.ChangeAction(ActionController::ACTION_TYPE::REACT);
}

#ifdef DEBUG_ON
void PlayerOnHit::DrawDebug(void)
{
	//colParam_[BODY_SPHERE_COL_NO].geometry_->DrawPlayerUI();
	//colParam_[MOVE_LINE_COL_NO].geometry_->DrawPlayerUI();
	//colParam_[UP_AND_DOWN_LINE_COL_NO].geometry_->DrawPlayerUI();
	//colParam_[EYE_LINE_NO].geometry_->DrawPlayerUI();
}
#endif // DEBUG_ON


