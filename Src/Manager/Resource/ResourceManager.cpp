#include <DxLib.h>
#include "../../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

void ResourceManager::Init(void)
{
	static std::wstring PATH_IMG = Application::PATH_IMAGE;
	static std::wstring PATH_MDL = Application::PATH_MODEL;
	static std::wstring PATH_ANIM = Application::PATH_ANIM;
	static std::wstring PATH_EFF = Application::PATH_EFFECT;

	std::unique_ptr<Resource> res;

	res = std::make_unique<Resource>(Resource::TYPE::IMG, PATH_IMG + L"test.png");
	resourcesMap_.emplace(SRC::TEST, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_MDL + L"Player.mv1");
	resourcesMap_.emplace(SRC::PLAYER, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_ANIM + L"Idle.mv1");
	resourcesMap_.emplace(SRC::IDLE, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_ANIM + L"SlowRun.mv1");
	resourcesMap_.emplace(SRC::RUN, std::move(res));

	res = std::make_unique<Resource>(Resource::TYPE::MODEL, PATH_ANIM + L"PlayerAttack.mv1");
	resourcesMap_.emplace(SRC::P_ATTACK1, std::move(res));

}

void ResourceManager::SceneChangeRelease(void)
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
}

void ResourceManager::Release(void)
{
	SceneChangeRelease();
	resourcesMap_.clear();
}

const Resource& ResourceManager::Load(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return dummy_;
	}
	return res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res.handleId_);
	res.duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

Resource& ResourceManager::_Load(SRC src)
{

	// ロード済みチェック
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	// リソース登録チェック
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// 登録されていない
		return dummy_;
	}

	// ロード処理
	rPair->second->Load();

	// 念のためコピーコンストラクタ
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;

}
