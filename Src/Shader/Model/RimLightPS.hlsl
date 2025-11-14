#include "../Common/VertexToPixelHeader.hlsli"

// IN

#define PS_INPUT VertexToPixelLit

// PS

#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)

cbuffer cbParam : register(b4)

{
	// 拡散光
	float4 g_color;

	// 環境光
	float4 g_ambient_color;

	// 反射光
	float4 g_specular_color;

	// 光の向き
	float3 g_light_dir;

	float  g_specular_pow;

	// カメラの位置
	float3 g_camera_pos;

	float  dummy;

	float3 g_rim_color;
	float dummy2;

}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
	float4 color;

	// テクスチャーの色を取得
	color = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv);
	if (color.a < 0.01f)
	{
		discard;
	}
	//法線
	float3 norm = PSInput.normal;

	//拡散光の強さ
	float lightDot = dot(norm, -g_light_dir);



	//////視線ベクトル
	float3 toEye = normalize(PSInput.worldPos - g_camera_pos);

	//頂点シェーダからアウトラインの情報を持ってくる
	float rimOutLine = PSInput.outLine;
	rimOutLine = pow(rimOutLine, 3.0f);

	float3 ret = color.rgb + rimOutLine * g_rim_color + g_ambient_color;

	return float4(ret.rgb, color.a);

}
