#include"../Common/VertexToPixelHeader.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

//PS
#include"../Common/Pixel/PixelShader3DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
	float4 g_color;
	float3 g_light_dir;
	float4 g_ambient_color;
	float dummy;
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
	float4 color;
	
	//テクスチャの色を取得
	color = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv);
	if (color.a < 0.01f)
	{
		discard;
	}

	//ライト方向
	float3 lightDir = g_common.light[0].direction;

	//法線と定数バッファのライト方向の内積
	float3 dotL = dot(-g_light_dir,PSInput.normal);

	//モデルからの色に乗算
	float3 ret = (color.rgb * g_color * dotL)+g_ambient_color;


	//return float4(lightDir.x, lightDir.y, lightDir.z, 1.0f);
	//return float4(g_light_dir.x, g_light_dir.y, g_light_dir.z, 1.0f);

	// ③法線がワールド空間になっているか確認
	//return float4(
	//	PSInput.normal, 1.0f);


	return float4(
		ret, color.a);


}