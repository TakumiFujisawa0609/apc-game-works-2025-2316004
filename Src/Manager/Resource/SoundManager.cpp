#include "SoundManager.h"
#include <DxLib.h>
#include <cassert>
#include "../Resource/ResourceManager.h"
#include "../../Application.h"

//SoundManager* SoundManager::instance_ = nullptr;

SoundManager::SoundManager(void):
	resMng_(ResourceManager::GetInstance()),
    dummy_()
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
//    SoundResource res = { -1, TYPE::BGM,L"" };
//	std::wstring path_Bgm = Application::PATH_SOUND_BGM;
//	std::wstring path_Se = Application::PATH_SOUND_SE;
//
//#pragma region BGM
//    res.type = TYPE::BGM;
//    res.path = path_Bgm + L"GameTitle.mp3";
//    resourcesMap_.emplace(SRC::TITLE_BGM, res);
//
//    res.path = path_Bgm + L"GameSceneK.mp3";
//    resourcesMap_.emplace(SRC::GAME_BGM, res);
//
//    res.path = path_Bgm + L"GameClear.mp3";
//    resourcesMap_.emplace(SRC::GAME_CLEAR, res);
//
//    res.path = path_Bgm + L"GameOver.mp3";
//    resourcesMap_.emplace(SRC::GAME_OVER, res);
//
//#pragma endregion
//
//#pragma region SE
//	res.type = TYPE::SE;
//    res.path = path_Se + L"PutCard.mp3";
//    resourcesMap_.emplace(SRC::CARD_PUT, res);
//
//    res.path = path_Se + L"CardSelect2.mp3";
//    resourcesMap_.emplace(SRC::CARD_MOVE, res);
//
//    res.path = path_Se + L"CardReflected.mp3";
//    resourcesMap_.emplace(SRC::CARD_BE_REFLECTED_SE, res);
//
//    res.path = path_Se + L"CardBreak.mp3";
//    resourcesMap_.emplace(SRC::CARD_BREAK_SE, res);
//
// 	res.path = path_Se + L"CardReload1.mp3";
//	resourcesMap_.emplace(SRC::CARD_RELOAD, res);
//
//	res.path = path_Se + L"CardReloadFinish.mp3";
//	resourcesMap_.emplace(SRC::CARD_RELOAD_FINISH, res);
//
//    res.path = path_Se + L"PlayerFoot.mp3";
//	resourcesMap_.emplace(SRC::PLAYER_FOOT_SE, res);
//
//    res.path = path_Se + L"Blade_Atk.mp3";
//	resourcesMap_.emplace(SRC::PLAYER_ATTACK_SE, res);
//
//    res.path = path_Se + L"KeyBladeHit.mp3";
//	resourcesMap_.emplace(SRC::PLAYER_HIT_SE, res);
//
//    res.path = path_Se + L"EnemyFoot.mp3";
//	resourcesMap_.emplace(SRC::ENEMY_FOOT_SE, res);
//
//    res.path = path_Se + L"EnemyStomp.mp3";
//	resourcesMap_.emplace(SRC::ENEMY_STOMP_SE, res);
//
//    res.path = path_Se + L"EnemyCharge.mp3";
//	resourcesMap_.emplace(SRC::ENEMY_CHARGE_SE, res);
//
//    res.path = path_Se + L"Enemy_Bomb2.mp3";
//	resourcesMap_.emplace(SRC::ENEMY_JUMP_LAND_SE, res);
//
//
//    res.path = path_Se + L"PlayerDamage.mp3";
//    resourcesMap_.emplace(SRC::ENEMY_HIT_SE, res);
//
//
//    res.path = path_Se + L"CursolButtonSE.mp3";
//	resourcesMap_.emplace(SRC::MOVE_BTN_SE, res);
//
//    res.path = path_Se + L"DesideButton3.mp3";
//	resourcesMap_.emplace(SRC::DESIDE_BTN_SE, res);
//
//    res.path = path_Se + L"DesideButton.mp3";
//	resourcesMap_.emplace(SRC::GAME_START_SE, res);
//

#pragma endregion

}

void SoundManager::Play(const SRC _src, const PLAYTYPE _playType)
{
	//音源が読み込まれていない場合はエラー
	int handleId = resMng_.GetResource(_src).handleId_;
	assert(handleId == -1 && "音源が読み込まれてないです");

    //音源が再生済みか調べる
	if (CheckSoundMem(handleId) == 1 &&
        _playType != PLAYTYPE::BACK)
	{
		Stop(_src);  // 再生済みなら停止
	}

    //音源の再生
    int i=PlaySoundMem(handleId, GetPlayType(_playType));
}

void SoundManager::Stop(const SRC _src)
{
    //音源の停止
    StopSoundMem(loadedMap_[_src].handleId);
}

bool SoundManager::IsPlay(const SRC _src) const
{
    const auto res = resMng_.GetResource(_src);
    if (res.handleId_==-1)
    {
        return false; // 見つからない場合は未再生とする
    }
    return CheckSoundMem(res.handleId_) == 1;
}

void SoundManager::SetSoundVolumeSRC(const SRC _src, const float _volumePercent)
{
    constexpr float VOLUME_MAX = 255.0f;  //最大音量
    constexpr float DIV = 1.0f;         //音量の割合を計算するための定数
    const auto& lPair = loadedMap_.find(_src);
    if (lPair == loadedMap_.end())
    {
        return; // 見つからない場合は処理しない
    }
	int volume = static_cast<int>(VOLUME_MAX * _volumePercent);
    //音量設定
	ChangeVolumeSoundMem(volume, lPair->second.handleId);
}

void SoundManager::SetSystemVolume(const float _volumePercent, const TYPE _type)
{    
    constexpr float VOLUME_MAX = 255.0f;  //最大音量
    constexpr float DIV = 1.0f;         //音量の割合を計算するための定数

	const int type = static_cast<int>(_type);
    //音量設定
    volume_[type] = _volumePercent;
   
    //音量調整
	for (const auto& pair : loadedMap_)
	{
        //種類が異なるものはスキップ
		if (pair.second.type != _type)
        {
			continue;
		}
        int volume = static_cast<int>(VOLUME_MAX * volume_[type]);
        ChangeVolumeSoundMem(volume, pair.second.handleId);
	}
}

bool SoundManager::_Load(const SRC _src, const float _pitch)
{
 //   // ロード済みチェック
 //   const auto& lPair = loadedMap_.find(_src);
 //   if (lPair != loadedMap_.end())
 //   {
	//	return false;   // 既にロード済み
 //   }

 //   // リソース登録チェック
 //   const auto& rPair = resourcesMap_.find(_src);
 //   if (rPair == resourcesMap_.end())
 //   {
 //       return false;   // 登録されていない
 //   }

 //   //ピッチ調整
 //   if (_pitch != 0.0f)
 //   {
 //       SetCreateSoundPitchRate(_pitch);
 //   }
 //   // ロード処理
 //   rPair->second.handleId = LoadSoundMem(rPair->second.path.c_str());
 //   loadedMap_.emplace(_src, rPair->second);

   ////ピッチを元に戻す
   // SetCreateSoundPitchRate(0.0f);

	if (resMng_.GetResource(_src).type_==ResourceData::TYPE::NONE)
    {
        return false;   // 登録されていない
	}
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
