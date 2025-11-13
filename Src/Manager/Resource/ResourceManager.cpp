#include <DxLib.h>
#include "../../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

void ResourceManager::Init(void)
{
	static std::wstring PATH_IMG = Application::PATH_IMAGE;
	static std::wstring PATH_MDL = Application::PATH_MODEL;
	static std::wstring PATH_ANIM_PLAYER = Application::PATH_ANIM_PLAYER;
	static std::wstring PATH_ANIM_ENEMY = Application::PATH_ANIM_ENEMY;
	static std::wstring PATH_EFF = Application::PATH_EFFECT;

	std::unique_ptr<ResourceData> res;

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + L"test.png");
	resourcesMap_.emplace(SRC::TEST, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_MDL + L"Player.mv1");
	resourcesMap_.emplace(SRC::PLAYER, std::move(res));
	
	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_MDL + L"Mutant.mv1");
	resourcesMap_.emplace(SRC::ENEMY_1, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + L"Idle.mv1");
	resourcesMap_.emplace(SRC::P_IDLE, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + L"SlowRun.mv1");
	resourcesMap_.emplace(SRC::P_RUN, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + L"PlayerAttack.mv1");
	resourcesMap_.emplace(SRC::P_ATTACK_1, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + L"PlayerAttack2.mv1");
	resourcesMap_.emplace(SRC::P_ATTACK_2 , std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + L"PlayerAttack3.mv1");
	resourcesMap_.emplace(SRC::P_ATTACK_3, std::move(res));
	
	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + L"PlayerJump.mv1");
	resourcesMap_.emplace(SRC::P_JUMP, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + L"PlayerDodge.mv1");
	resourcesMap_.emplace(SRC::P_DODGE, std::move(res));	
	
	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + L"PlayerReact.mv1");
	resourcesMap_.emplace(SRC::REACT, std::move(res));	

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_ENEMY + L"Mutant_Idle.mv1");
	resourcesMap_.emplace(SRC::E_IDLE, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_ENEMY + L"Mutant_Run.mv1");
	resourcesMap_.emplace(SRC::E_RUN, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_ENEMY + L"Mutant_Swiping.mv1");
	resourcesMap_.emplace(SRC::E_SWIP_ATK, std::move(res));
	
	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_ENEMY + L"Mutant_JumpAttack.mv1");
	resourcesMap_.emplace(SRC::E_JUMP_ATK, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_ENEMY + L"Mutant_Roar.mv1");
	resourcesMap_.emplace(SRC::E_ROAR_ATK, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_ENEMY + L"Mutant_Rush.mv1");
	resourcesMap_.emplace(SRC::E_ROLE_ATK, std::move(res));





	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMGS, PATH_IMG + L"CardNumber.png",CARD_NO_X, CARD_NO_Y,CARD_NO_SIZE_X, CARD_NO_SIZE_Y);
	resourcesMap_.emplace(SRC::NUMBERS_IMG, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + L"AttackCard.png");
	resourcesMap_.emplace(SRC::ATK_CARD_IMG, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + L"ReloadCard.png");
	resourcesMap_.emplace(SRC::RELOAD_CARD_IMG, std::move(res));


	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_MDL + L"EmaBeni.mv1");
	resourcesMap_.emplace(SRC::STAGE, std::move(res));
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

const ResourceData& ResourceManager::Load(SRC src)
{
	ResourceData& res = _Load(src);
	if (res.type_ == ResourceData::TYPE::NONE)
	{
		return dummy_;
	}
	return res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	ResourceData& res = _Load(src);
	if (res.type_ == ResourceData::TYPE::NONE)
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

ResourceManager::~ResourceManager(void)
{
	loadedMap_.clear();
	resourcesMap_.clear();
}

ResourceData& ResourceManager::_Load(SRC src)
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
