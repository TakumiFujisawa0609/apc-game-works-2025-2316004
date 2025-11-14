#include"../Common/VertexToPixelHeader.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

//PS
#include"../Common/Pixel/PixelShader3DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
	float4 g_color;
	float g_time;
	float3 dummy;
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
	float4 color;
	float2 uv= PSInput.uv;

	//テクスチャの色を取得
	uv.x += g_time;
	color = diffuseMapTexture.Sample(diffuseMapSampler, uv);

	if (color.a < 0.01f)
	{
		discard;
	}
	return color;
}