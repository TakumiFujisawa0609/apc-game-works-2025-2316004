#include <DxLib.h>
#include "../../Application.h"
#include "../Utility/UtilityCommon.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager(void):
	dummy_(ResourceData::TYPE::NONE,L"")
{
	//素材の登録文字列とSRCの結び付け
	resStr_ = {
		{ "STAGE",SRC::STAGE },
		{ "STAGE_WALL",SRC::STAGE_WALL  },
		{ "PLAYER" ,SRC::PLAYER},
		{ "ENEMY_1",SRC::ENEMY},
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
		{ "BLAST" ,SRC::BLAST},
		{ "KEY_BLADE_HIT_EFF" ,SRC::KEY_BLADE_HIT_EFF},
		{ "E_JUMP_CHARGE_EFF" ,SRC::E_JUMP_CHARGE_EFF},
		{ "E_DEATH_EFF" ,SRC::E_DEATH_EFF},
		{ "RELOAD_EFF" ,SRC::RELOAD_EFF},
		{ "RELOAD_END_EFF" ,SRC::RELOAD_END_EFF},
		//BGM
		{"TITLE_BGM",SRC::TITLE_BGM},
		{"GAME_BGM",SRC::GAME_BGM},
		{"GAME_CLEAR_BGM",SRC::GAME_CLEAR_BGM},
		{"GAME_OVER_BGM",SRC::GAME_OVER_BGM},
		{"PLAYER_FOOT_SE",SRC::PLAYER_FOOT_SE},
		{"ENEMY_FOOT_SE",SRC::ENEMY_FOOT_SE},
		{"ENEMY_STOMP_SE",SRC::ENEMY_STOMP_SE},
		{"ENEMY_CHARGE_SE",SRC::ENEMY_CHARGE_SE},
		{"ENEMY_JUMP_LAND_SE",SRC::ENEMY_JUMP_LAND_SE},
		{"ENEMY_HIT_SE",SRC::ENEMY_HIT_SE},
		{"PLAYER_ATTACK_SE",SRC::PLAYER_ATTACK_SE},
		{"PLAYER_DODGE_SE",SRC::PLAYER_DODGE_SE},
		{"PLAYER_HIT_SE",SRC::PLAYER_HIT_SE},
		{"CARD_PUT_SE",SRC::CARD_PUT_SE},
		{"CARD_MOVE_SE",SRC::CARD_MOVE_SE},
		{"CARD_BE_REFLECTED_SE",SRC::CARD_BE_REFLECTED_SE},
		{"CARD_BREAK_SE",SRC::CARD_BREAK_SE},
		{"CARD_RELOAD_SE",SRC::CARD_RELOAD_SE},
		{"CARD_RELOAD_FINISH_SE",SRC::CARD_RELOAD_FINISH_SE},
		{"MOVE_BTN_SE",SRC::MOVE_BTN_SE},
		{"DESIDE_BTN_SE",SRC::DESIDE_BTN_SE},
		{"GAME_START_SE",SRC::GAME_START_SE}
	};

	//リソース種類と文字列の結び付け
	resTypeStr_ = {
		{"model",{ResourceData::TYPE::MODEL,Application::PATH_MODEL}},
		{"image",{ResourceData::TYPE::IMG,Application::PATH_IMAGE}},
		{"images",{ResourceData::TYPE::IMGS,Application::PATH_IMAGE}},
		{"effect",{ResourceData::TYPE::EFFEKSEER,Application::PATH_EFFECT}},
		{"sound",{ResourceData::TYPE::SOUND,Application::PATH_SOUND}}
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

		//リソースの配列に挿入
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
		else if(type== ResourceData::TYPE::SOUND)
		{
			ResourceData::SOUND_TYPE soundType = ResourceData::SOUND_TYPE::MAX;
			if (data["soundtype"] == "BGM")
			{
				soundType = ResourceData::SOUND_TYPE::BGM;
				path = Application::PATH_SOUND_BGM + UtilityCommon::GetWStringFromString(data["handle"]);
			}
			else if (data["soundtype"] == "SE")
			{
				soundType = ResourceData::SOUND_TYPE::SE;
				path = Application::PATH_SOUND_SE + UtilityCommon::GetWStringFromString(data["handle"]);
			}
			float pitch = 0.0f;
			float timeStretch = 1.0f;
			float volume = 1.0f;
			float loopStartTime = 0.0f;
			float loopEndTime = 0.0f;

			//サウンドの情報を読み込む。存在しない場合はデフォルト値を入れる
			data.contains("pitch") ? pitch = data["pitch"] : pitch = 0.0f;
			data.contains("timeStretch") ? timeStretch = data["timeStretch"] : timeStretch = 1.0f;
			data.contains("volume") ? volume = data["volume"] : volume = 1.0f;
			data.contains("loopStartTime") ? loopStartTime = data["loopStartTime"] : loopStartTime = 0.0f;
			data.contains("loopEndTime") ? loopEndTime = data["loopEndTime"] : loopEndTime = 0.0f;

			//サウンドの情報を渡す
			res = std::make_unique<ResourceData>(type, path, soundType, pitch, timeStretch, volume, loopStartTime, loopEndTime);
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

const ResourceData ResourceManager::GetResource(const SRC src) const
{
	const auto it = loadedMap_.find(src);
	if (it == loadedMap_.end())
	{
		return dummy_;
	}
	return it->second;
}

std::vector<const ResourceData*> ResourceManager::GetSoundResources(ResourceData::SOUND_TYPE _soundType) const
{
	std::vector<const ResourceData*>retArray;
	for (auto& load : loadedMap_)
	{
		if (load.second.type_ == ResourceData::TYPE::SOUND)
		{
			//サウンドの種類に指定がなければサウンドをすべて渡す
			if (_soundType == ResourceData::SOUND_TYPE::MAX)
			{
				retArray.push_back(&load.second);
			}
			//BGMかSEが指定されていれば、そのサウンドの種類のみを渡す
			else if (load.second.soundType_ == _soundType)
			{
				retArray.push_back(&load.second);
			}
		}
	}

	return retArray;
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
