#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float4 g_color;
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    //textureから色を取得
    float4 srcCol = tex.Sample(texSampler, PSInput.uv);
    
    return g_color * srcCol;
}