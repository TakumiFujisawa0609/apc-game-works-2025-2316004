#include "SoundManager.h"
#include <DxLib.h>
#include <cassert>
#include "../../Application.h"

SoundManager* SoundManager::instance_ = nullptr;

SoundManager::SoundManager(void)
{  
	// 音量の初期化
	for (int i = 0; i < TYPE_MAX; ++i)
	{
		volume_[i] = DEFAULT_VOLUME;
	}
}

SoundManager::~SoundManager(void)
{
}

void SoundManager::CreateInstance(void)
{
    if (instance_ == nullptr)
    {
        instance_ = new SoundManager();
    }
    // 初期化処理を呼び出す
    instance_->Init();
}

SoundManager& SoundManager::GetInstance(void)
{
    return *instance_;
}

void SoundManager::Destroy(void)
{
    Release();
	resourcesMap_.clear();
    if (instance_ != nullptr)
    {
        delete instance_;
        instance_ = nullptr;
    }
}

void SoundManager::Release(void)
{
    if (loadedMap_.empty())return;
    for (auto& p : loadedMap_)
    {
        DeleteSoundMem(p.second.handleId);
    }

    loadedMap_.clear();
}

void SoundManager::Init(void)
{
    SoundResource res = { -1, TYPE::BGM,L"" };
	std::wstring path_Bgm = Application::PATH_SOUND_BGM;
	std::wstring path_Se = Application::PATH_SOUND_SE;

#pragma region BGM
    res.type = TYPE::BGM;
    res.path = path_Bgm + L"GameTitle.mp3";
    resourcesMap_.emplace(SRC::TITLE_BGM, res);

    res.path = path_Bgm + L"GameSceneK.mp3";
    resourcesMap_.emplace(SRC::GAME_BGM, res);
#pragma endregion

#pragma region SE
	res.type = TYPE::SE;
//    res.path = path_Se + L"CardSound.mp3";
    res.path = path_Se + L"CardSelect2.mp3";
    resourcesMap_.emplace(SRC::CARD_MOVE, res);

    res.path = path_Se + L"CardReflected.mp3";
    resourcesMap_.emplace(SRC::CARD_BE_REFLECTED, res);

    res.path = path_Se + L"CardBreak.mp3";
    resourcesMap_.emplace(SRC::CARD_BREAK, res);

 	res.path = path_Se + L"CardReload1.mp3";
	resourcesMap_.emplace(SRC::CARD_RELOAD, res);

	res.path = path_Se + L"CardReloadFinish.mp3";
	resourcesMap_.emplace(SRC::CARD_RELOAD_FINISH, res);

    res.path = path_Se + L"PlayerFoot.mp3";
	resourcesMap_.emplace(SRC::PLAYER_FOOT_SE, res);

    res.path = path_Se + L"Blade_Atk.mp3";
	resourcesMap_.emplace(SRC::PLAYER_ATTACK_SE, res);

    res.path = path_Se + L"Blade_Atk.mp3";
	resourcesMap_.emplace(SRC::PLAYER_HIT_SE, res);

    res.path = path_Se + L"EnemyFoot.mp3";
	resourcesMap_.emplace(SRC::ENEMY_FOOT_SE, res);

    res.path = path_Se + L"EnemyStomp.mp3";
	resourcesMap_.emplace(SRC::ENEMY_STOMP_SE, res);

    res.path = path_Se + L"EnemyCharge.mp3";
	resourcesMap_.emplace(SRC::ENEMY_CHARGE_SE, res);

    res.path = path_Se + L"Enemy_Bomb2.mp3";
	resourcesMap_.emplace(SRC::ENEMY_JUMP_LAND_SE, res);

#pragma endregion

}

const bool SoundManager::LoadResource(const SRC _src, const float _pitch)
{
    return _Load(_src, _pitch);
}

void SoundManager::Play(const SRC _src, const PLAYTYPE _playType)
{
	//音源が読み込まれていない場合はエラー
	assert(loadedMap_[_src].handleId == -1&& "音源が読み込まれてないです");

    //音源が再生済みか調べる
	if (CheckSoundMem(loadedMap_[_src].handleId) == 1 &&
        _playType == PLAYTYPE::BACK)
	{
		Stop(_src);  // 再生済みなら停止
	}

    //音源の再生
    int i=PlaySoundMem(loadedMap_[_src].handleId, GetPlayType(_playType));
}

void SoundManager::Stop(const SRC _src)
{
    //音源の停止
    StopSoundMem(loadedMap_[_src].handleId);
}

bool SoundManager::IsPlay(const SRC _src) const
{
    const auto it = loadedMap_.find(_src);
    if (it == loadedMap_.end())
    {
        return false; // 見つからない場合は未再生とする
    }
    return CheckSoundMem(it->second.handleId) == 1;
}

void SoundManager::SetSoundVolumeSRC(const SRC _src, const int _volumePercent)
{
    constexpr int VOLUME_MAX = 255;  //最大音量
    constexpr int DIV = 100;         //音量の割合を計算するための定数
    const auto& lPair = loadedMap_.find(_src);
    if (lPair == loadedMap_.end())
    {
        return; // 見つからない場合は処理しない
    }
    //音量設定
	ChangeVolumeSoundMem(VOLUME_MAX * _volumePercent / DIV, lPair->second.handleId);
}

void SoundManager::SetSystemVolume(const int _volumePercent, const int _type)
{    
    constexpr int VOLUME_MAX = 255;  //最大音量
    constexpr int DIV = 100;         //音量の割合を計算するための定数

    //音量設定
    volume_[_type] = _volumePercent;
   
    //音量調整
	for (const auto& pair : loadedMap_)
	{
        //種類が異なるものはスキップ
		if (pair.second.type != static_cast<TYPE>(_type)) 
        {
			continue;
		}
        ChangeVolumeSoundMem(VOLUME_MAX * volume_[_type] / DIV, pair.second.handleId);
	}
}

bool SoundManager::_Load(const SRC _src, const float _pitch)
{
    // ロード済みチェック
    const auto& lPair = loadedMap_.find(_src);
    if (lPair != loadedMap_.end())
    {
		return false;   // 既にロード済み
    }

    // リソース登録チェック
    const auto& rPair = resourcesMap_.find(_src);
    if (rPair == resourcesMap_.end())
    {
        return false;   // 登録されていない
    }

    //ピッチ調整
    if (_pitch != 0.0f)
    {
        SetCreateSoundPitchRate(_pitch);
    }
    // ロード処理
    rPair->second.handleId = LoadSoundMem(rPair->second.path.c_str());
    loadedMap_.emplace(_src, rPair->second);

    //ピッチを元に戻す
	SetCreateSoundPitchRate(0.0f);

    return true;
}

int SoundManager::GetPlayType(const PLAYTYPE _playType)
{
    switch (_playType)
    {
    case PLAYTYPE::NORMAL:
        return DX_PLAYTYPE_NORMAL;
        break;

    case PLAYTYPE::LOOP:
        return DX_PLAYTYPE_LOOP;
        break;

    case PLAYTYPE::BACK:
        return DX_PLAYTYPE_BACK;
        break;

    default:
        return DX_PLAYTYPE_NORMAL;
        break;
    }
}
