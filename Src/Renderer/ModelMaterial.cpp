#include "../Application.h"
#include "ModelMaterial.h"

ModelMaterial::ModelMaterial(
	std::wstring shaderFileNameVS, int constBufFloat4SizeVS,
	std::wstring shaderFileNamePS, int constBufFloat4SizePS)
{

	// 頂点シェーダのロード
	shaderVS_ = LoadVertexShader(
		(Application::PATH_SHADER + shaderFileNameVS).c_str());

	// 頂点定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	constBufFloat4SizeVS_ = constBufFloat4SizeVS;

	// 頂点シェーダー用の定数バッファを作成
	constBufVS_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4SizeVS);

	// ピクセルシェーダのロード
	shaderPS_ = LoadPixelShader(
		(Application::PATH_SHADER + shaderFileNamePS).c_str());

	// ピクセル定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	constBufFloat4SizePS_ = constBufFloat4SizePS;

	// ピクセルシェーダー用の定数バッファを作成
	constBufPS_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4SizePS);

	// テクスチャアドレス
	texAddress_ = TEXADDRESS::CLAMP;

}

void ModelMaterial::AddConstBufVS(const FLOAT4& contBuf)
{
	if (constBufFloat4SizeVS_ > constBufsVS_.size())
	{
		constBufsVS_.emplace_back(contBuf);
	}
}

void ModelMaterial::AddConstBufPS(const FLOAT4& contBuf)
{
	if (constBufFloat4SizePS_ > constBufsPS_.size())
	{
		constBufsPS_.emplace_back(contBuf);
	}
}

void ModelMaterial::SetConstBufVS(int idx, const FLOAT4& contBuf)
{
	if (idx >= constBufsVS_.size())
	{
		return;
	}

	constBufsVS_[idx] = contBuf;
}

void ModelMaterial::SetConstBufPS(int idx, const FLOAT4& contBuf)
{
	if (idx >= constBufsPS_.size())
	{
		return;
	}

	constBufsPS_[idx] = contBuf;
}

void ModelMaterial::SetTextureBuf(int slot, int texDiffuse)
{
	if (textures_.count(slot) == 0)
	{
		textures_.emplace(slot, texDiffuse);
	}
	else
	{
		textures_[slot] = texDiffuse;
	}
}

ModelMaterial::~ModelMaterial(void)
{
	DeleteShader(shaderVS_);
	DeleteShaderConstantBuffer(constBufVS_);
	DeleteShader(shaderPS_);
	DeleteShaderConstantBuffer(constBufPS_);
}
