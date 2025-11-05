//#include "....//Manager/System/SoundManager.h"
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

PlayerOnHit::PlayerOnHit(CharacterBase& _chara, VECTOR& _movedPos, VECTOR& _moveDiff, ActionController& _action, std::vector<std::shared_ptr<Collider>> _colParam, Transform& _trans, Collider::TAG _tag):
	charaObj_(_chara),
	movedPos_(_movedPos),
	moveDiff_(_moveDiff),
	action_(_action),
	trans_(_trans),
	tag_(_tag)
{
	//それぞれの当たった処理を格納する
	using TAG = Collider::TAG;
	colUpdates_.emplace(TAG::ENEMY1, [this](const std::weak_ptr<Collider> _hitCol) {CollChara(_hitCol); });
	colUpdates_.emplace(TAG::PLAYER1, [this](const std::weak_ptr<Collider> _hitCol) {CollChara(_hitCol); });
	colUpdates_.emplace(TAG::NML_ATK, [this](const std::weak_ptr<Collider> _hitCol) {CollNormalAttack(_hitCol); });
	colUpdates_.emplace(TAG::STAGE, [this](const std::weak_ptr<Collider>_hitCol) {CollStage(_hitCol); });
	colUpdates_.emplace(TAG::ROAR_ATK, [this](const std::weak_ptr<Collider>_hitCol) {CollRoarAttack(_hitCol); });

	for (auto& col : _colParam)
	{
		colParam_.emplace_back(col);
	}

	////プレイヤー同士の当たり判定のタグをNoneに設定
	//int playerNum = DateBank::GetInstance().GetPlayerNum();
	//for (int i = static_cast<int>(TAG::PLAYER1); i < playerNum; i++)
	//{
	//	//同じタグだったら設定しない
	//	if (static_cast<int>(tag_)== i)continue;
	//	colUpdates_[static_cast<TAG>(i)] = [this](const std::weak_ptr<Collider> _hitCol) {CollNone(); };
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
	//自分のタグと相手のタグをとる
	auto& parentChara = _hitCol.lock()->GetParentCharacter();
	auto& tags = _hitCol.lock()->GetTags();
	
	auto it = std::find(tags.begin(), tags.end(), Collider::TAG::NML_ATK);
	if (it != tags.end())return;

	Capsule* myCap = dynamic_cast<Capsule*>(&colParam_[BODY_SPHERE_COL_NO].lock()->GetGeometry());
	Capsule* hitCap = dynamic_cast<Capsule*>(&_hitCol.lock()->GetGeometry());

	if (myCap == nullptr || hitCap == nullptr)
	{
		return;
	}
	//自分の座標
	VECTOR myPos = charaObj_.GetTransform().pos;
	VECTOR hitCharaPos = parentChara.GetTransform().pos;

	float dis = Utility3D::Distance(myPos, hitCharaPos);
	float minDist = myCap->GetRadius() + hitCap->GetRadius();
	float pushPow = abs(minDist - dis);

	VECTOR vec = Utility3D::GetMoveVec(parentChara.GetTransform().pos,charaObj_.GetTransform().pos );
	vec.y = 0;
	//charaObj_.MovedPosMove(vec, pushPow);
	movedPos_ = VAdd(moveDiff_, VScale(vec, pushPow));
	//parentChara.MovedPosMove(vec, pushPow);
	//movedPos_ = VAdd(movedPos_,VScale(vec, pushPow));

}

void PlayerOnHit::CollNormalAttack(const std::weak_ptr<Collider> _hitCol)
{
	auto& parentChara = _hitCol.lock()->GetParentCharacter();
	if (parentChara.GetIsDamage())return;
	
	auto tag = _hitCol.lock()->GetParentCharacter().GetCharaTag();
	//ダメージを与えたことを知らせる
	parentChara.SetIsDamage();
	charaObj_.Damage(1);
	action_.ChangeAction(ActionController::ACTION_TYPE::REACT);
	
}

void PlayerOnHit::CollRoarAttack(const std::weak_ptr<Collider> _hitCol)
{
	auto& parentChara = _hitCol.lock()->GetParentCharacter();
	if (parentChara.GetIsDamage())return;

	auto tag = _hitCol.lock()->GetParentCharacter().GetCharaTag();
	//ダメージを与えたことを知らせる
	parentChara.SetIsDamage();
	charaObj_.Damage(1);

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
	Model& hitModel = dynamic_cast<Model&>(const_cast<Geometry&>(_hitCol.lock()->GetGeometry()));
	//当たったモデルの情報を取得
	//移動後座標と現在座標で早い移動速度でも対応させる
	VECTOR hitPos = hitModel.GetHitLineInfo().HitPosition;
	//移動後と移動前のコライダ
	auto& moveLineCol = colParam_[MOVE_LINE_COL_NO];
	//上下を引いたラインのコライダ(接地)
	auto& upDownLine = colParam_[UP_AND_DOWN_LINE_COL_NO];
	//球の当たり判定(プレイヤーの周囲)
	auto& bodyShere = colParam_[BODY_SPHERE_COL_NO];



	if (moveLineCol.lock()->IsHit())
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
	if (upDownLine.lock()->IsHit())
	{
		//ライン情報の取得
		Line& upDown = dynamic_cast<Line&>(upDownLine.lock()->GetGeometry());
		VECTOR hitLinePos = upDown.GetHitInfo().HitPosition;

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
	if (bodyShere.lock()->IsHit())
	{
		auto& hitInfo = hitModel.GetHitInfo();
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
