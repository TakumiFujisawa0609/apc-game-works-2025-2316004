//#include "....//Manager/System/SoundManager.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Object/Common/Geometry/Sphere.h"
#include "../../../Manager/Resource/ResourceManager.h"
#include "../../../Object/Common/Geometry/Line.h"
#include"../../../Object/Common/Geometry/Model.h"
#include "../Object/Character/Base/CharacterBase.h"
#include"../../../Utility/Utility3D.h"

#include"./Player.h"
#include"./ActionController.h"
#include "PlayerOnHit.h"

PlayerOnHit::PlayerOnHit(CharacterBase& _chara, VECTOR& _movedPos, VECTOR& _moveDiff, ActionController& _action, std::vector<ObjectBase::ColParam>& _colParam, Transform& _trans, Collider::TAG _tag):
	charaObj_(_chara),
	movedPos_(_movedPos),
	moveDiff_(_moveDiff),
	action_(_action),
	colParam_(_colParam),
	trans_(_trans),
	tag_(_tag)
{
	//それぞれの当たった処理を格納する
	using TAG = Collider::TAG;
	colUpdates_.emplace(TAG::ENEMY1, [this](const std::weak_ptr<Collider> _hitCol) {CollChara(_hitCol); });
	colUpdates_.emplace(TAG::PLAYER1, [this](const std::weak_ptr<Collider> _hitCol) {CollChara(_hitCol); });
	colUpdates_.emplace(TAG::SWORD, [this](const std::weak_ptr<Collider> _hitCol) {CollSword(_hitCol); });
	colUpdates_.emplace(TAG::STAGE, [this](const std::weak_ptr<Collider>hitCol) {CollStage(); });

	////プレイヤー同士の当たり判定のタグをNoneに設定
	//int playerNum = DateBank::GetInstance().GetPlayerNum();
	//for (int i = static_cast<int>(TAG::PLAYER1); i < playerNum; i++)
	//{
	//	//同じタグだったら設定しない
	//	if (static_cast<int>(tag_)== i)continue;
	//	colUpdates_[static_cast<TAG>(i)] = [this](const std::weak_ptr<Collider> _hitCol) {CollNone(); };
	//}

	isDeath_ = false;
	isLandHit_ = false;
	isHitOverHead_ = false;
	hitPoint_ = {};
	isHitSlimeFloor_ = false;
	moveDiff_ = Utility3D::VECTOR_ZERO;
	movedPos_ = Utility3D::VECTOR_ZERO;
	isSide_ = false;
	coinNum_ = 0;
	springJumpPow_ = 0.0f;
}

PlayerOnHit::~PlayerOnHit(void)
{

}

void PlayerOnHit::Load(void)
{
	
}

void PlayerOnHit::Init(void)
{
	isDeath_ = false;
	coinNum_ = 0;
	isLandHit_ = false;
	isHitOverHead_ = false;
	hitPoint_ = {};
	isHitSlimeFloor_ = false;
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

void PlayerOnHit::CollStage(void)
{
}

void PlayerOnHit::CollChara(const std::weak_ptr<Collider> _hitCol)
{
	
}

void PlayerOnHit::CollSword(const std::weak_ptr<Collider> _hitCol)
{
	if (action_.GetAct() == ActionController::ACTION_TYPE::REACT)return;
	charaObj_.Damage(1);
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
	auto& moveLineCol = colParam_[MOVE_LINE_COL_NO].collider_;
	//上下を引いたラインのコライダ(接地)
	auto& upDownLine = colParam_[UP_AND_DOWN_LINE_COL_NO].collider_;
	//球の当たり判定(プレイヤーの周囲)
	auto& bodyShere = colParam_[BODY_SPHERE_COL_NO].collider_;

	isHitSlimeFloor_ = false;

	if (moveLineCol->IsHit())
	{
		////Y座標のみ半径分上に移動させる
		movedPos_.y = hitPos.y + Player::RADIUS + POSITION_OFFSET;

		hitPoint_.isDown = true;
		//現在座標の更新
		trans_.pos = movedPos_;
		return;
	}
	//プレイヤーの接地
	if (upDownLine->IsHit())
	{
		//ライン情報の取得
		Line& upDown = dynamic_cast<Line&>(upDownLine->GetGeometry());
		VECTOR hitLinePos = upDown.GetHitInfo().HitPosition;

		//座標が当たっているライン座標より上のとき、地面と当たる
		if (movedPos_.y > hitLinePos.y)
		{
			movedPos_.y = hitLinePos.y + Player::RADIUS + POSITION_OFFSET;
			hitPoint_.isDown = true;
			//action_.SetJumpPow(Utility3D::VECTOR_ZERO);
		}

		//移動後座標が当たっているライン座標より下の場合、上に押し出す
		if(movedPos_.y < hitLinePos.y)
		{
			movedPos_.y = hitLinePos.y - Player::RADIUS - POSITION_OFFSET;
			hitPoint_.isOverHead = true;
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
	isSide_ = false;
	//プレイヤーの体の球が当たったら
	if (bodyShere->IsHit())
	{
		auto& hitInfo = hitModel.GetHitInfo();
		for (int i = 0; i < hitInfo.HitNum; i++)
		{
			isSide_ = true;
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
