#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Game/CollisionManager.h"
#include "ObjectBase.h"

ObjectBase::ObjectBase(void)
	: resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance())
{
}

ObjectBase::~ObjectBase(void)
{
}
void ObjectBase::OnHit(const std::weak_ptr<Collider> _hitCol)
{

}

void ObjectBase::MakeCollider(const std::set<Collider::TAG> _tag, std::unique_ptr<Geometry> _geometry, const std::set<Collider::TAG> _notHitTags)
{
	//当たり判定情報
	ColParam colParam;

	//形状情報の挿入
	colParam.geometry_ = std::move(_geometry);

	//情報を使ってコライダの作成
	colParam.collider_ = std::make_shared<Collider>(*this, _tag, *colParam.geometry_, _notHitTags);

	//コライダを管理マネージャーに追加
	CollisionManager::GetInstance().AddCollider(colParam.collider_);

	//配列にセット
	colParam_.push_back(std::move(colParam));
}

void ObjectBase::MakeCol(const Collider::TAG _chataTag, const Collider::TAG _tag)
{
	//事前に配列のサイズを取得する
	auto ParamSize = colParam_.size();

	for (int i = 0; i < ParamSize; i++)
	{
		auto tags = colParam_[i].collider_->GetTags();
		auto tagSize = colParam_[i].collider_->GetTags().size();
		for (int j = 0; j < tagSize; j++)
		{
			auto tagSize = colParam_[i].collider_->GetTags().size();
			//i,jの値が最後まで行ったとき
			if (i == ParamSize - 1 && j == tagSize - 1)
			{
				//コライダのパンチがなかったら生成する
				//if (tags[j] != _tag)
				//{
				//	//プレイヤーの手(パンチの当たり判定)
				//	std::unique_ptr<Sphere>handSphereGeo = std::make_unique<Sphere>(action_->GetPunchPos(), PUNCH_RADIUS);
				//	MakeCollider({ tag_,Collider::TAG::PUNCH }, std::move(handSphereGeo));
				//	return;
				//}
			}
		}
	}
}

void ObjectBase::DeleteCollider(const int _arrayNum)
{
	//配列番号-1
	int arrayNum = _arrayNum - 1;

	//コライダの削除
	colParam_[_arrayNum].collider_->Kill();

	//配列の削除
	colParam_.erase(colParam_.begin() + arrayNum);
}

void ObjectBase::DeleteAllCollider(void)
{
	for (auto& col : colParam_)
	{
		//当たり判定が存在しないならスキップ
		if (col.collider_ == nullptr)continue;

		//コライダの削除
		col.collider_->Kill();
	}

	//当たり判定情報の削除
	colParam_.clear();
}

