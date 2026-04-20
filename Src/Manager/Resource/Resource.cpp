#include <DxLib.h>
#include <wingdi.h>
#include <EffekseerForDXLib.h>
#include <cassert>
#include "Resource.h"

ResourceData::ResourceData(void):
	type_(TYPE::NONE),
	path_(L""),
	numX_(-1),
	numY_(-1),
	sizeX_(-1),
	sizeY_(-1),
	handleId_(-1),
	handleIds_(nullptr),
	soundType_(SOUND_TYPE::NONE),
	pitch_(0.0f),
	timeStretch_(0.0f),
	volume_(0.0f),
	loopStartTime_(0.0f),
	loopEndTime_(0.0f)
{

}

ResourceData::ResourceData(TYPE type, const std::wstring& path):
	type_(type),
	path_(path),
	numX_(-1),
	numY_(-1),
	sizeX_(-1),
	sizeY_(-1),
	handleId_(-1),
	handleIds_(nullptr),
	soundType_(SOUND_TYPE::NONE),
	pitch_(0.0f),
	timeStretch_(0.0f),
	volume_(0.0f),
	loopStartTime_(0.0f),
	loopEndTime_(0.0f)
{
}

ResourceData::ResourceData(TYPE type, const std::wstring& path, int numX, int numY, int sizeX, int sizeY):
	type_(type),
	path_(path),
	numX_(numX),
	numY_(numY),
	sizeX_(sizeX),
	sizeY_(sizeY),
	handleId_(-1),
	handleIds_(nullptr),
	soundType_(SOUND_TYPE::NONE),
	pitch_(0.0f),
	timeStretch_(0.0f),
	volume_(0.0f),
	loopStartTime_(0.0f),
	loopEndTime_(0.0f)
{

}

ResourceData::ResourceData(TYPE type, const std::wstring& path
	, SOUND_TYPE soundType
	, const float pitch
	, const float timeStretch
	, const float volume
	, const float loopStartTime, const float loopEndTime):
	type_(type),
	path_(path),
	soundType_(soundType),
	pitch_(pitch),
	timeStretch_(timeStretch),
	volume_(volume),
	loopStartTime_(loopStartTime),
	loopEndTime_(loopEndTime),
	handleId_(-1),
	handleIds_(nullptr)
{
}

ResourceData::~ResourceData(void)
{
	duplicateModelIds_.clear();
}

void ResourceData::Load(void)
{

	switch (type_)
	{
	case ResourceData::TYPE::IMG:
		// 画像
		handleId_ = LoadGraph(path_.c_str());
		break;

	case ResourceData::TYPE::IMGS:
		// 複数画像
		handleIds_ = new int[numX_ * numY_];
		LoadDivGraph(
			path_.c_str(),
			numX_ * numY_,
			numX_, numY_,
			sizeX_, sizeY_,
			&handleIds_[0]);
		break;

	case ResourceData::TYPE::MODEL:
		// モデル
		handleId_ = MV1LoadModel(path_.c_str());
		break;

	case ResourceData::TYPE::EFFEKSEER:
		//エフェクト
		handleId_ = LoadEffekseerEffect(path_.c_str());
		break;

	case ResourceData::TYPE::FONT:
		//フォント
		handleId_ = AddFontResourceEx(path_.c_str(), FR_PRIVATE, NULL);
		break;

	case ResourceData::TYPE::VERTEX_SHADER:
		//頂点シェーダー
		handleId_ = LoadVertexShader(path_.c_str());
		break;

	case ResourceData::TYPE::PIXEL_SHADER:
		//ピクセルシェーダー
		handleId_ = LoadPixelShader(path_.c_str());
		break;

	case ResourceData::TYPE::SOUND:
		//音声
		handleId_ = LoadSoundMem(path_.c_str());
		break;
	}

	//読み込みできたか確認
	assert(handleId_ != -1); // 読み込みに失敗してたら即終了

}

void ResourceData::Release(void)
{

	switch (type_)
	{
	case ResourceData::TYPE::IMG:
		DeleteGraph(handleId_);
		break;

	case ResourceData::TYPE::IMGS:
	{
		int num = numX_ * numY_;
		for (int i = 0; i < num; i++)
		{
			DeleteGraph(handleIds_[i]);
		}
		delete[] handleIds_;
	}
		break;

	case ResourceData::TYPE::MODEL:
	{
		MV1DeleteModel(handleId_);
		auto ids = duplicateModelIds_;
		for (auto id : ids)
		{
			MV1DeleteModel(id);
		}
	}
		break;

	case ResourceData::TYPE::EFFEKSEER:
		DeleteEffekseerEffect(handleId_);
		break;

	case ResourceData::TYPE::FONT:
		RemoveFontResourceEx(path_.c_str(), FR_PRIVATE, NULL);
		break;

	case ResourceData::TYPE::VERTEX_SHADER:
		DeleteShader(handleId_);
		break;

	case ResourceData::TYPE::PIXEL_SHADER:
		DeleteShader(handleId_);
		break;

	case ResourceData::TYPE::SOUND:
		DeleteSoundMem(handleId_);
		break;
	}

}

void ResourceData::CopyHandle(int* imgs)
{

	if (handleIds_ == nullptr)
	{
		return;
	}

	int num = numX_ * numY_;
	for (int i = 0; i < num; i++)
	{
		imgs[i] = handleIds_[i];
	}

}

void ResourceData::LoadImg(void)
{
	handleId_ = LoadGraph(path_.c_str());
}

void ResourceData::LoadImgs(void)
{
	handleIds_ = new int[numX_ * numY_];
	LoadDivGraph(
		path_.c_str(),
		numX_ * numY_,
		numX_, numY_,
		sizeX_, sizeY_,
		&handleIds_[0]);
}

void ResourceData::LoadModel(void)
{
	handleId_ = MV1LoadModel(path_.c_str());
}

void ResourceData::LoadSound(void)
{
	handleId_ = LoadSoundMem(path_.c_str());
}

void ResourceData::LoadFont(void)
{
	handleId_ = AddFontResourceEx(path_.c_str(), FR_PRIVATE, NULL);
}

void ResourceData::LoadEffekseer(void)
{
	handleId_ = LoadEffekseerEffect(path_.c_str());
}

void ResourceData::LoadVS(void)
{
	handleId_ = LoadVertexShader(path_.c_str());
}

void ResourceData::LoadPS(void)
{
	handleId_ = LoadPixelShader(path_.c_str());
}

void ResourceData::ReleaseImg(void)
{
	DeleteGraph(handleId_);
}

void ResourceData::ReleaseImgs(void)
{
	int num = numX_ * numY_;
	for (int i = 0; i < num; i++)
	{
		DeleteGraph(handleIds_[i]);
	}
	delete[] handleIds_;
}

void ResourceData::ReleaseModel(void)
{
	MV1DeleteModel(handleId_);
	auto ids = duplicateModelIds_;
	for (auto id : ids)
	{
		MV1DeleteModel(id);
	}
}

void ResourceData::ReleaseSound(void)
{
	DeleteSoundMem(handleId_);
}

void ResourceData::ReleaseFont(void)
{
	RemoveFontResourceEx(path_.c_str(), FR_PRIVATE, NULL);
}

void ResourceData::ReleaseEffekseer(void)
{
	DeleteEffekseerEffect(handleId_);
}

void ResourceData::ReleaseVS(void)
{
	DeleteShader(handleId_);
}

void ResourceData::ReleasePS(void)
{
	DeleteShader(handleId_);
}
