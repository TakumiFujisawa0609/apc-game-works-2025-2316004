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
	static std::wstring PATH_SHADER = Application::PATH_SHADER;

	std::unique_ptr<ResourceData> res;

	//モデル登録
	//res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_MDL + L"Player.mv1");
	//resourcesMap_.emplace(SRC::PLAYER, std::move(res));
	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_MDL + L"Ninja.mv1");
	resourcesMap_.emplace(SRC::PLAYER, std::move(res));
	
	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_MDL + L"Mutant.mv1");
	resourcesMap_.emplace(SRC::ENEMY_1, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_MDL + L"Card.mv1");
	resourcesMap_.emplace(SRC::CARD_MDL, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_MDL + L"SkyDome/SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_MDL + L"Rock.mv1");
	resourcesMap_.emplace(SRC::SPHERE_ROCK, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_MDL + L"KeyBlade/KeyBlade1.mv1");
	resourcesMap_.emplace(SRC::KEY_BLADE, std::move(res));


	//アニメーション登録
	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + L"PlayerIdle.mv1");
	resourcesMap_.emplace(SRC::P_IDLE, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + L"PlayerRun.mv1");
	resourcesMap_.emplace(SRC::P_RUN, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + L"PlayerAttack1.mv1");
	resourcesMap_.emplace(SRC::P_ATTACK_1, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + L"PlayerAttack2.mv1");
	resourcesMap_.emplace(SRC::P_ATTACK_2 , std::move(res));

	//res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + L"PlayerAttack3.mv1");
	//resourcesMap_.emplace(SRC::P_ATTACK_3, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + L"PlayerAttackCombo.mv1");
	resourcesMap_.emplace(SRC::P_ATTACK_3, std::move(res));
	
	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + L"PlayerJump.mv1");
	resourcesMap_.emplace(SRC::P_JUMP, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + L"PlayerDodge.mv1");
	resourcesMap_.emplace(SRC::P_DODGE, std::move(res));	

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + L"CardReload.mv1");
	resourcesMap_.emplace(SRC::P_RELOAD, std::move(res));	
	
	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + L"PlayerReact.mv1");
	resourcesMap_.emplace(SRC::REACT, std::move(res));	

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_ENEMY + L"Mutant_Idle.mv1");
	resourcesMap_.emplace(SRC::E_IDLE, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_ENEMY + L"Mutant_Run.mv1");
	resourcesMap_.emplace(SRC::E_RUN, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_ENEMY + L"MutantStomp.mv1");
	resourcesMap_.emplace(SRC::E_STOMP_ATK, std::move(res));
	
	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_ENEMY + L"Mutant_JumpAttack.mv1");
	resourcesMap_.emplace(SRC::E_JUMP_ATK, std::move(res));
	
	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_ENEMY + L"MutantKnockDown.mv1");
	resourcesMap_.emplace(SRC::E_KNOCK_DOWN, std::move(res));


	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_ENEMY + L"Mutant_Roar.mv1");
	resourcesMap_.emplace(SRC::E_ROAR_ATK, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_ENEMY + L"Mutant_Rush.mv1");
	resourcesMap_.emplace(SRC::E_ROLE_ATK, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_MDL + L"SandStage.mv1");
	resourcesMap_.emplace(SRC::STAGE, std::move(res));

	//ピクセルシェーダ登録
	res = std::make_unique<ResourceData>(ResourceData::TYPE::PIXEL_SHADER, PATH_SHADER + L"CardPS.cso");
	resourcesMap_.emplace(SRC::CARD_PS, std::move(res));
	

	//画像登録
	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + L"TitleBack.png");
	resourcesMap_.emplace(SRC::TITLE_BACK_IMG, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + L"TitleLogo.png");
	resourcesMap_.emplace(SRC::TITLE_LOGO, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + L"GameClear.png");
	resourcesMap_.emplace(SRC::GAME_CLEAR_IMG, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + L"GameOver.png");
	resourcesMap_.emplace(SRC::GAME_OVER_IMG, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + L"PlayerAttackCard.png");
	resourcesMap_.emplace(SRC::PLAYER_ATK_CARD_IMG, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + L"EnemyAttackCard.png");
	resourcesMap_.emplace(SRC::ENEMY_ATK_CARD_IMG, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + L"ReloadCard.png");
	resourcesMap_.emplace(SRC::RELOAD_CARD_IMG, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + L"ReloadCard.png");
	resourcesMap_.emplace(SRC::RELOAD_CARD_IMG, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + L"CardSelectFrame2.png");
	resourcesMap_.emplace(SRC::CARD_SELECT_FRAME_IMG, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + L"ReloadFrame.png");
	resourcesMap_.emplace(SRC::RELOAD_FRAME, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + L"ReloadGage.png");
	resourcesMap_.emplace(SRC::RELOAD_GAGE, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + L"E_HpBarMask.png");
	resourcesMap_.emplace(SRC::E_HP_BAR_MASK, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + L"E_HpBarFrame.png");
	resourcesMap_.emplace(SRC::E_HP_BAR_FRAME, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + L"E_GaugeCover.png");
	resourcesMap_.emplace(SRC::E_HP_COVER, std::move(res));



	//複数画像
	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMGS, PATH_IMG + L"CardNumber.png", CARD_NO_X, CARD_NO_Y, CARD_NO_SIZE_X, CARD_NO_SIZE_Y);
	resourcesMap_.emplace(SRC::NUMBERS_IMG, std::move(res));

	//エフェクト登録
	res = std::make_unique<ResourceData>(ResourceData::TYPE::EFFEKSEER, PATH_EFF + L"Blast/Blast.efkefc");
	resourcesMap_.emplace(SRC::BLAST, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::EFFEKSEER, PATH_EFF + L"ExpandStar/ExpandStar.efkefc");
	resourcesMap_.emplace(SRC::KEY_BLADE_HIT_EFF, std::move(res));

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
