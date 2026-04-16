#include <DxLib.h>
#include "../../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

namespace RES_PATH
{
	//モデル
	const std::wstring NINJA = L"Ninja.mv1";
	const std::wstring MUTANT = L"Mutant.mv1";
	const std::wstring SKY_DOME = L"SkyDome/SkyDome.mv1";
	const std::wstring ROCK = L"Rock.mv1";
	const std::wstring KEY_BLADE = L"KeyBlade/KeyBlade1.mv1";
	const std::wstring STAGE = L"SandStage.mv1";
	const std::wstring WALL = L"Wall.mv1";

	//プレイヤーアニメ
	const std::wstring P_IDLE = L"PlayerIdle.mv1";
	const std::wstring P_RUN = L"PlayerRun.mv1";
	const std::wstring P_ATTACK1 = L"PlayerAttack1.mv1";
	const std::wstring P_STAB = L"PlayerStabAttack.mv1";
	const std::wstring P_ATTACK2 = L"PlayerAttack2.mv1";
	const std::wstring P_ATTACK3 = L"PlayerAttackCombo.mv1";
	const std::wstring P_JUMP = L"PlayerJump.mv1";
	const std::wstring P_DODGE = L"PlayerDodge.mv1";
	const std::wstring P_RELOAD = L"CardReload.mv1";
	const std::wstring P_REACT = L"PlayerReact.mv1";
	const std::wstring P_DEATH = L"PlayerDying.mv1";

	//敵アニメ
	const std::wstring E_IDLE = L"Mutant_Idle.mv1";
	const std::wstring E_RUN = L"Mutant_Run.mv1";
	const std::wstring E_STOMP = L"MutantStomp.mv1";
	const std::wstring E_JUMP = L"Mutant_JumpAttack.mv1";
	const std::wstring E_DOWN = L"MutantKnockDown.mv1";
	const std::wstring E_ROAR = L"Mutant_Roar.mv1";
	const std::wstring E_RUSH = L"Mutant_Rush.mv1";
	const std::wstring E_DEATH = L"Mutant Dying.mv1";

	//画像
	const std::wstring TITLE_BACK = L"TitleBack.png";
	const std::wstring TITLE_LOGO = L"TitleLogo.png";
	const std::wstring GAME_CLEAR = L"GameClear2.png";
	const std::wstring GAME_OVER = L"GameOver.png";
	const std::wstring P_ATK_CARD = L"PlayerAttackCard.png";
	const std::wstring E_ATK_CARD = L"EnemyAttackCard.png";
	const std::wstring RELOAD_CARD = L"ReloadCard.png";
	const std::wstring RELOAD_GAGE = L"ReloadGage.png";
	const std::wstring E_HP_BAR_MASK = L"E_HpBarMask.png";
	const std::wstring E_HP_BAR_FRAME = L"E_HpBarFrame.png";
	const std::wstring E_HP_COVER = L"E_GaugeCover.png";
	const std::wstring P_CARD_NUM_GAUGE_MASK = L"CardNumGaugeMask.png";
	const std::wstring P_CARD_NUM_GAUGE_FRAME = L"CardNumGaugeFrame.png";
	const std::wstring P_CARD_NUM_GAUGE_BACK = L"CardNumBack.png";
	const std::wstring SKIP_BUTTOM_MASK = L"SkipButtonMask.png";
	const std::wstring CARD_REVOLVER_L_ARROW = L"CardDirArrowLeft.png";
	const std::wstring INTENSIVE_LINE_1 = L"Intensive_Line_1.png";
	const std::wstring INTENSIVE_LINE_2 = L"Intensive_Line_2.png";

	//ピクセルシェーダ
	const std::wstring PS_DEFAULT = L"PS_Default.hlsl";
	const std::wstring CARD_PS = L"PS_Card.hlsl";

	//複数画像
	const std::wstring CARD_NUM = L"CardNumber.png";
	const std::wstring CONTROLLER_BOTTON_UI = L"XboxControllerBotton128.png";

	//エフェクト
	const std::wstring BLAST = L"Blast/Blast.efkefc";
	const std::wstring KEY_BLADE_HIT_EFF = L"ExpandStar/ExpandStar.efkefc";
	const std::wstring E_JUMP_CHARGE_EFF = L"Blast/EnemyCharge.efkefc";
	const std::wstring E_DEATH_EFF = L"DeathEnemy/boss_death.efkproj";
	const std::wstring RELOAD_EFF = L"Reload/PowerUp.efkproj";
	const std::wstring RELOAD_END_EFF = L"ReloadEnd/MagicHeal1.efkproj";

}



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
	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_MDL + RES_PATH::NINJA);
	resourcesMap_.emplace(SRC::PLAYER, std::move(res));
	
	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_MDL + RES_PATH::MUTANT);
	resourcesMap_.emplace(SRC::ENEMY_1, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_MDL + RES_PATH::SKY_DOME);
	resourcesMap_.emplace(SRC::SKY_DOME, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_MDL + RES_PATH::ROCK);
	resourcesMap_.emplace(SRC::SPHERE_ROCK, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_MDL + RES_PATH::KEY_BLADE);
	resourcesMap_.emplace(SRC::KEY_BLADE, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_MDL + RES_PATH::STAGE);
	resourcesMap_.emplace(SRC::STAGE, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_MDL + RES_PATH::WALL);
	resourcesMap_.emplace(SRC::STAGE_WALL, std::move(res));


	//アニメーション登録
	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + RES_PATH::P_IDLE);	
	resourcesMap_.emplace(SRC::P_IDLE, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + RES_PATH::P_RUN);
	resourcesMap_.emplace(SRC::P_RUN, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + RES_PATH::P_ATTACK1);
	resourcesMap_.emplace(SRC::P_ATTACK_1_SHORT, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + RES_PATH::P_STAB);
	resourcesMap_.emplace(SRC::P_ATTACK_1_MIDDLE, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + RES_PATH::P_ATTACK2);
	resourcesMap_.emplace(SRC::P_ATTACK_2 , std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + RES_PATH::P_ATTACK3);
	resourcesMap_.emplace(SRC::P_ATTACK_3, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + RES_PATH::P_DODGE);
	resourcesMap_.emplace(SRC::P_DODGE, std::move(res));	

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + RES_PATH::P_RELOAD);
	resourcesMap_.emplace(SRC::P_RELOAD, std::move(res));	
	
	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + RES_PATH::P_REACT);
	resourcesMap_.emplace(SRC::REACT, std::move(res));	
	
	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_PLAYER + RES_PATH::P_DEATH);
	resourcesMap_.emplace(SRC::P_DEATH, std::move(res));	

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_ENEMY + RES_PATH::E_IDLE);
	resourcesMap_.emplace(SRC::E_IDLE, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_ENEMY + RES_PATH::E_RUN);
	resourcesMap_.emplace(SRC::E_RUN, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_ENEMY + RES_PATH::E_STOMP);
	resourcesMap_.emplace(SRC::E_STOMP_ATK, std::move(res));
	
	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_ENEMY + RES_PATH::E_JUMP);
	resourcesMap_.emplace(SRC::E_JUMP_ATK, std::move(res));
	
	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_ENEMY + RES_PATH::E_DOWN);
	resourcesMap_.emplace(SRC::E_KNOCK_DOWN, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_ENEMY + RES_PATH::E_ROAR);
	resourcesMap_.emplace(SRC::E_ROAR_ATK, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::MODEL, PATH_ANIM_ENEMY + RES_PATH::E_DEATH);
	resourcesMap_.emplace(SRC::E_DEATH, std::move(res));

	//ピクセルシェーダ登録
	res = std::make_unique<ResourceData>(ResourceData::TYPE::PIXEL_SHADER, PATH_SHADER + RES_PATH::CARD_PS);
	resourcesMap_.emplace(SRC::CARD_PS, std::move(res));

	//画像登録
	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + RES_PATH::TITLE_BACK);
	resourcesMap_.emplace(SRC::TITLE_BACK_IMG, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + RES_PATH::TITLE_LOGO);
	resourcesMap_.emplace(SRC::TITLE_LOGO, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + RES_PATH::GAME_CLEAR);
	resourcesMap_.emplace(SRC::GAME_CLEAR_IMG, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + RES_PATH::GAME_OVER);
	resourcesMap_.emplace(SRC::GAME_OVER_IMG, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + RES_PATH::P_ATK_CARD);
	resourcesMap_.emplace(SRC::PLAYER_ATK_CARD_IMG, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + RES_PATH::E_ATK_CARD);
	resourcesMap_.emplace(SRC::ENEMY_ATK_CARD_IMG, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + RES_PATH::RELOAD_CARD);
	resourcesMap_.emplace(SRC::RELOAD_CARD_IMG, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + RES_PATH::RELOAD_GAGE);
	resourcesMap_.emplace(SRC::RELOAD_GAGE, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + RES_PATH::E_HP_BAR_MASK);
	resourcesMap_.emplace(SRC::E_HP_BAR_MASK, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + RES_PATH::E_HP_BAR_FRAME);
	resourcesMap_.emplace(SRC::E_HP_BAR_FRAME, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + RES_PATH::E_HP_COVER);
	resourcesMap_.emplace(SRC::E_HP_COVER, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + RES_PATH::P_CARD_NUM_GAUGE_MASK);
	resourcesMap_.emplace(SRC::P_CARD_NUM_GAUGE_MASK, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + RES_PATH::P_CARD_NUM_GAUGE_FRAME);	
	resourcesMap_.emplace(SRC::P_CARD_NUM_GAUGE_FRAME, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + RES_PATH::P_CARD_NUM_GAUGE_BACK);
	resourcesMap_.emplace(SRC::P_CARD_NUM_GAUGE_BACK, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + RES_PATH::INTENSIVE_LINE_1);
	resourcesMap_.emplace(SRC::INTENSIVE_LINE_1, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + RES_PATH::INTENSIVE_LINE_2);
	resourcesMap_.emplace(SRC::INTENSIVE_LINE_2, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + RES_PATH::SKIP_BUTTOM_MASK);
	resourcesMap_.emplace(SRC::SKIP_BUTTOM_MASK, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMG, PATH_IMG + RES_PATH::CARD_REVOLVER_L_ARROW);
	resourcesMap_.emplace(SRC::CARD_REVOLVER_L_ARROW, std::move(res));

	//複数画像
	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMGS, PATH_IMG + RES_PATH::CARD_NUM,
		CARD_NO_X, CARD_NO_Y, CARD_NO_SIZE_X, CARD_NO_SIZE_Y);
	resourcesMap_.emplace(SRC::NUMBERS_IMGS, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::IMGS, PATH_IMG + RES_PATH::CONTROLLER_BOTTON_UI,
		CONTROLLER_UI_NO_X, CONTROLLER_UI_NO_Y, CONTROLLER_UI_SIZE_X, CONTROLLER_UI_SIZE_Y);
	resourcesMap_.emplace(SRC::CONTROLLER_UI_IMGS, std::move(res));


	//エフェクト登録
	res = std::make_unique<ResourceData>(ResourceData::TYPE::EFFEKSEER, PATH_EFF + RES_PATH::BLAST);
	resourcesMap_.emplace(SRC::BLAST, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::EFFEKSEER, PATH_EFF + RES_PATH::E_JUMP_CHARGE_EFF);
	resourcesMap_.emplace(SRC::E_JUMP_CHARGE_EFF, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::EFFEKSEER, PATH_EFF + RES_PATH::KEY_BLADE_HIT_EFF);
	resourcesMap_.emplace(SRC::KEY_BLADE_HIT_EFF, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::EFFEKSEER, PATH_EFF + RES_PATH::E_DEATH_EFF);
	resourcesMap_.emplace(SRC::E_DEATH_EFF, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::EFFEKSEER, PATH_EFF + RES_PATH::RELOAD_EFF);
	resourcesMap_.emplace(SRC::RELOAD_EFF, std::move(res));

	res = std::make_unique<ResourceData>(ResourceData::TYPE::EFFEKSEER, PATH_EFF + RES_PATH::RELOAD_END_EFF);
	resourcesMap_.emplace(SRC::RELOAD_END_EFF, std::move(res));
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
