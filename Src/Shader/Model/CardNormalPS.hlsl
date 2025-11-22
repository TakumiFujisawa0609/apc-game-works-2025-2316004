#include "../Common/Pixel/PixelShader2DHeader.hlsli"
// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float4 g_color;
    float g_fog_Pow;
    float g_time;
    float dummy;
}
float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    float2 uv = PSInput.uv;
    float4 srcCol = srcCol = tex.Sample(texSampler, uv);
    
    if (srcCol.a < 0.01)
    {
        discard;
    }
    //return outlineCol + srcCol;
    
    //白いモヤを付ける
    float area = sin(g_time * 1.0);
    area = area * g_fog_Pow;
    area = abs(area);
    srcCol.rgb += area;
    return srcCol + g_color;
   
}