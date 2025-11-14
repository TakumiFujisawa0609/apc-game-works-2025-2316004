#include"../Common/VertexToPixelHeader.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

//PS
#include"../Common/Pixel/PixelShader3DHeader.hlsli"
SamplerState noizeSampler            : register(s11); 	    	// ノイズテクスチャ
Texture2D    noizeTexture            : register(t11);	    	// ノイズマップテクスチャ


// 定数バッファ：スロット4番目(b5と書く)
cbuffer cbParam : register(b4)
{
	float4 g_color;
	float g_rate;	//0.0f~1.0f
	float g_time;
	float2 dummy;
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
	float4 color;
	float2 uv = PSInput.uv;


	//float graya=(texColor.rgb)/

	//テクスチャの色を取得
	uv.y += sin(g_time*0.3f + uv.x);
	color = diffuseMapTexture.Sample(diffuseMapSampler, uv);
	float4 noizeColor = noizeTexture.Sample(diffuseMapSampler, uv);

	if (g_rate > noizeColor.r)
	{
		discard;
	}	
	if (g_rate + 0.03f > noizeColor.r && g_rate != 0.0f)
	{
		return float4(1.0f, 0.0f, 0.0f, 1.0f);
	}

	return color;
}