#include <DxLib.h>
#include "../../Application.h"
#include "../Utility/UtilityCommon.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager(void)
{
	//素材の登録文字列とSRCの結び付け
	resStr_ = {
		{ "STAGE",SRC::STAGE },
		{ "STAGE_WALL",SRC::STAGE_WALL  },
		{ "PLAYER" ,SRC::PLAYER},
		{ "ENEMY_1",SRC::ENEMY_1},
		{ "SKY_DOME",SRC::SKY_DOME },
		{ "SPHERE_ROCK" ,SRC::SPHERE_ROCK},
		{ "KEY_BLADE" ,SRC::KEY_BLADE},
		{ "P_IDLE", SRC::P_IDLE},
		{ "P_RUN" ,SRC::P_RUN},
		{ "P_ATTACK_1_SHORT" ,SRC::P_ATTACK_1_SHORT },
		{ "P_ATTACK_1_MIDDLE" ,SRC::P_ATTACK_1_MIDDLE},
		{ "P_ATTACK_2",SRC::P_ATTACK_2 },
		{ "P_ATTACK_3",SRC::P_ATTACK_3 },
		{ "P_DODGE",SRC::P_DODGE },
		{ "P_RELOAD",SRC::P_RELOAD },
		{ "P_DEATH" ,SRC::P_DEATH},
		{ "E_IDLE" ,SRC::E_IDLE},
		{ "E_RUN" ,SRC::E_RUN},
		{ "E_STOMP_ATK",SRC::E_STOMP_ATK},
		{ "E_JUMP_ATK" ,SRC::E_JUMP_ATK},
		{ "E_ROAR_ATK",SRC::E_ROAR_ATK },
		{ "E_KNOCK_DOWN",SRC::E_KNOCK_DOWN },
		{ "REACT" ,SRC::REACT},
		{ "E_DEATH" ,SRC::E_DEATH},
		{ "TITLE_BACK_IMG",SRC::TITLE_BACK_IMG },
		{ "TITLE_LOGO" ,SRC::TITLE_LOGO},
		{ "GAME_OVER_IMG" ,SRC::GAME_OVER_IMG},
		{ "GAME_CLEAR_IMG" ,SRC::GAME_CLEAR_IMG},
		{ "PLAYER_ATK_CARD_IMG" ,SRC::PLAYER_ATK_CARD_IMG},
		{ "ENEMY_ATK_CARD_IMG" ,SRC::ENEMY_ATK_CARD_IMG},
		{ "RELOAD_CARD_IMG" ,SRC::RELOAD_CARD_IMG},
		{ "RELOAD_GAGE" ,SRC::RELOAD_GAGE},
		{ "P_HP_ARC_OUTLINE" ,SRC::P_HP_ARC_OUTLINE },
		{ "P_HP_LINE_OUT_LINE" ,SRC::P_HP_LINE_OUT_LINE},
		{ "P_CARD_NUM_GAUGE_MASK" ,SRC::P_CARD_NUM_GAUGE_MASK},
		{ "P_CARD_NUM_GAUGE_FRAME" ,SRC::P_CARD_NUM_GAUGE_FRAME},
		{ "P_CARD_NUM_GAUGE_BACK" ,SRC::P_CARD_NUM_GAUGE_BACK},
		{ "E_HP_BAR_MASK" ,SRC::E_HP_BAR_MASK},
		{ "E_HP_BAR_FRAME" ,SRC::E_HP_BAR_FRAME},
		{ "E_HP_COVER" ,SRC::E_HP_COVER},
		{ "SKIP_BUTTOM_MASK" ,SRC::SKIP_BUTTOM_MASK },
		{ "CARD_REVOLVER_L_ARROW" ,SRC::CARD_REVOLVER_L_ARROW},
		{ "INTENSIVE_LINE_1" ,SRC::INTENSIVE_LINE_1},
		{ "INTENSIVE_LINE_2" ,SRC::INTENSIVE_LINE_2},
		{ "NUMBERS_IMGS" ,SRC::NUMBERS_IMGS},
		{ "CONTROLLER_UI_IMGS" ,SRC::CONTROLLER_UI_IMGS},
		{ "CARD_PS" ,SRC::CARD_PS},
		{ "BLAST" ,SRC::BLAST},
		{ "KEY_BLADE_HIT_EFF" ,SRC::KEY_BLADE_HIT_EFF},
		{ "E_JUMP_CHARGE_EFF" ,SRC::E_JUMP_CHARGE_EFF},
		{ "E_DEATH_EFF" ,SRC::E_DEATH_EFF},
		{ "RELOAD_EFF" ,SRC::RELOAD_EFF},
		{ "RELOAD_END_EFF" ,SRC::RELOAD_END_EFF}
	};

	//リソース種類と文字列の結び付け
	resTypeStr_ = {
		{"model",{ResourceData::TYPE::MODEL,Application::PATH_MODEL}},
		{"image",{ResourceData::TYPE::IMG,Application::PATH_IMAGE}},
		{"images",{ResourceData::TYPE::IMGS,Application::PATH_IMAGE}},
		{"effect",{ResourceData::TYPE::EFFEKSEER,Application::PATH_EFFECT}}
	};
}

void ResourceManager::Init(void)
{

	//リソースをJsonファイルから読み込む
	nlohmann::json json;
	json = UtilityCommon::LoadJsonData(UtilityCommon::GetStringFromWString(Application::PATH_JSON+ L"ResourceData.json"));


	for (const auto& data : json["ResourceData"])
	{
		//ハンドルとリソースの種類を読み込む
		std::unique_ptr<ResourceData> res;
		ResourceData::TYPE type = resTypeStr_[data["type"]].resType;
		SRC src = resStr_[data["name"]];
		std::wstring path = resTypeStr_[data["type"]].typePath + UtilityCommon::GetWStringFromString(data["handle"]);

		res = std::make_unique<ResourceData>(type, path);

		//複数の画像を読み込んだ時は個数とサイズを読み込む
		if (type == ResourceData::TYPE::IMGS)
		{
			int numX = data["numX"];
			int numY = data["numY"];
			int sizeX = data["sizeX"];
			int sizeY = data["sizeY"];

			res = std::make_unique<ResourceData>(type, path,
				numX, numY, sizeX, sizeY);
		}
		resourcesMap_.emplace(src, std::move(res));

	}

	//Jsonを読み込み終わったので、関連の情報は解放する
	resTypeStr_.clear();
	resStr_.clear();


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
