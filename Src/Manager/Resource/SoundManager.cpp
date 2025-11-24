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
    res.path = path_Bgm + L"Battle3.mp3";
    resourcesMap_.emplace(SRC::GAME_BGM, res);
#pragma endregion

#pragma region SE
	res.type = TYPE::SE;
    res.path = path_Se + L"CardSound.mp3";
    resourcesMap_.emplace(SRC::CARD_MOVE, res);

#pragma endregion

}

const bool SoundManager::LoadResource(const SRC _src)
{
    return _Load(_src);
}

void SoundManager::Play(const SRC _src, const PLAYTYPE _playType)
{
	//音源が読み込まれていない場合はエラー
	assert(loadedMap_[_src].handleId == -1&& "音源が読み込まれてないです");

    //音源が再生済みか調べる
	if (CheckSoundMem(loadedMap_[_src].handleId) == 1 &&
        _playType != PLAYTYPE::BACK)
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

bool SoundManager::_Load(const SRC _src)
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

    // ロード処理
    rPair->second.handleId = LoadSoundMem(rPair->second.path.c_str());

    // 念のためコピーコンストラクタ
    loadedMap_.emplace(_src, rPair->second);

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
