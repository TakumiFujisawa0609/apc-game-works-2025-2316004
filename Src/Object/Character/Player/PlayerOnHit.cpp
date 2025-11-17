//#include "....//Manager/System/SoundManager.h"
#include <ranges>
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Object/Common/Geometry/Sphere.h"
#include "../../../Object/Common/Geometry/Capsule.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Object/Common/Geometry/Line.h"
#include"../../../Object/Common/Geometry/Model.h"
#include "../Object/Character/Base/CharacterBase.h"
#include"../../../Utility/Utility3D.h"

#include"./Player.h"
#include"./ActionController.h"
#include "PlayerOnHit.h"
namespace
{
	using TAG_PRIORITY = ObjectBase::TAG_PRIORITY;
	using TAG = Collider::TAG;
}
PlayerOnHit::PlayerOnHit(CharacterBase& _chara, VECTOR& _movedPos, VECTOR& _moveDiff, 
	ActionController& _action, std::map<ObjectBase::TAG_PRIORITY,std::shared_ptr<Collider>>& _colParam, Transform& _trans, Collider::TAG _tag):
	charaObj_(_chara),
	movedPos_(_movedPos),
	moveDiff_(_moveDiff),
	colParam_(_colParam),
	action_(_action),
	trans_(_trans),
	tag_(_tag)
{
	//それぞれの当たった処理を格納する
	using TAG = Collider::TAG;
	colUpdates_={
		{ TAG::ENEMY1, [this](const std::weak_ptr<Collider> _hitCol) {CollChara(_hitCol); } },
		{ TAG::PLAYER1, [this](const std::weak_ptr<Collider> _hitCol) {CollChara(_hitCol); } },
		{ TAG::NML_ATK, [this](const std::weak_ptr<Collider> _hitCol) {CollNormalAttack(_hitCol); } },
		{ TAG::STAGE, [this](const std::weak_ptr<Collider>_hitCol) {CollStage(_hitCol); } },
		{ TAG::ROAR_ATK, [this](const std::weak_ptr<Collider>_hitCol) {CollRoarAttack(_hitCol); } },
	};

	//for (auto& col : _colParam)
	//{
	//	colParam_.emplace_back(col);
	//}


	hitPoint_ = {};

	moveDiff_ = Utility3D::VECTOR_ZERO;
	movedPos_ = Utility3D::VECTOR_ZERO;
	springJumpPow_ = 0.0f;
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
	movedPos_ = Utility3D::VECTOR_ZERO;
}


void PlayerOnHit::OnHitUpdate(const std::weak_ptr<Collider> _hitCol)
{
	for (const auto tag : _hitCol.lock()->GetTags())
	{
		colUpdates_[tag](_hitCol);
	}
}

inline void PlayerOnHit::CollNone(void)
{
	//何もしない
}

void PlayerOnHit::CollStage(const std::weak_ptr<Collider> _hitCol)
{
	HitModelCommon(_hitCol);
}

void PlayerOnHit::CollChara(const std::weak_ptr<Collider> _hitCol)
{
	//相手のタグをとる
	CharacterBase& parentChara = _hitCol.lock()->GetParentCharacter();
	std::set<Collider::TAG> tags = _hitCol.lock()->GetTags();
	
	//攻撃の場合は無視
	if (std::ranges::any_of(tags, [](Collider::TAG tag) { return tag == Collider::TAG::NML_ATK || tag == Collider::TAG::ROAR_ATK; })||charaObj_.GetCardAction()==ActionBase::CARD_ACT_TYPE::RUSH_ATK)
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
	movedPos_ = VAdd(moveDiff_, VScale(vec, pushPow));

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


void PlayerOnHit::HitModelCommon(const std::weak_ptr<Collider> _hitCol)
{

	Geometry& hitModel = _hitCol.lock()->GetGeometry();
	//当たったモデルの情報を取得
	//移動後座標と現在座標で早い移動速度でも対応させる
	VECTOR hitPos = hitModel.GetHitLineInfo().HitPosition;
	//移動後と移動前のコライダ
	auto& moveLineCol = colParam_[TAG_PRIORITY::MOVE_LINE];
	//上下を引いたラインのコライダ(接地)
	auto& upDownLine = colParam_[TAG_PRIORITY::UPDOWN_LINE];
	//球の当たり判定(プレイヤーの周囲)
	auto& bodyShere = colParam_[TAG_PRIORITY::BODY];



	if (moveLineCol->IsHit())
	{
		////Y座標のみ半径分上に移動させる
		movedPos_.y = hitPos.y + POSITION_OFFSET;

		hitPoint_.isDown = true;
		//現在座標の更新
		trans_.pos = movedPos_;
		charaObj_.JumpPowZero();
		//action_.JumpPowZero();
		return;
	}

	//プレイヤーの接地
	if (upDownLine->IsHit())
	{
		//ライン情報の取得
		Geometry& upDown =upDownLine->GetGeometry();
		VECTOR hitLinePos = upDown.GetHitLineInfo().HitPosition;

		//座標が当たっているライン座標より上のとき、地面と当たる
		if (movedPos_.y > hitLinePos.y)
		{
			movedPos_.y = hitLinePos.y + Player::RADIUS + POSITION_OFFSET;
			hitPoint_.isDown = true;
			action_.JumpPowZero();
		}

		//移動後座標が当たっているライン座標より下の場合、上に押し出す
		if(movedPos_.y < hitLinePos.y)
		{
			movedPos_.y = hitLinePos.y - POSITION_OFFSET;
			hitPoint_.isOverHead = true;

			action_.JumpPowZero();
			//if (action_.GetJumpDecel() > 0.0f)
			//{
			//	//オブジェクトの下に当たったら跳ね返るようにする
			//	action_.SetJumpDecel(DOWN_BOUNCE_DECELERATION);
			//}
		}
	}
	//移動後座標を一回格納し、移動前をとる
	Transform trans = Transform(trans_);
	trans.pos = movedPos_;
	trans.Update();
	//プレイヤーの体の球が当たったら
	if (bodyShere->IsHit())
	{
		auto& hitInfo = hitModel.GetHitModelInfo();
		for (int i = 0; i < hitInfo.HitNum; i++)
		{
			hitPoint_.isSide = true;
			//当たっている部分の情報を取得
			auto hit = hitInfo.Dim[i];
			VECTOR hitPos = hit.HitPosition;
			
			//一定回数の押し出し処理をする
			for (int tryCnt = 0; tryCnt < COL_TRY_CNT_MAX; tryCnt++)
			{
				int pHit = HitCheck_Sphere_Triangle(trans.pos, RADIUS
					, hit.Position[0], hit.Position[1], hit.Position[2]);
				if (pHit)
				{
					VECTOR normal = hit.Normal;
					//y座標を抜いて押しだす
					normal.y = 0.0f;
 					movedPos_ = VAdd(movedPos_, VScale(normal, HIT_NORMAL_OFFSET));
					
					//カプセルを移動させる
					trans.pos = movedPos_;
					trans.Update();
					continue;
				}
				break;
			}
		}
	}

	////移動前の座標を格納する
	moveDiff_ = trans_.pos;
	//移動
	trans_.pos = movedPos_;
}
