#include "../Common/Pixel/PixelShader2DHeader.hlsli"
// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float4 g_color;
    float g_hp_Per;
    float3 dummy;
}
float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    float2 uv = PSInput.uv;
    float4 srcCol = srcCol = tex.Sample(texSampler, uv);
    
    if (uv.x > g_hp_Per)
    {
        discard;
    }
    
    return srcCol;
   
}