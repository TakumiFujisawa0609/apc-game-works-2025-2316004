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
    //float alpha = srcCol.a;
    //srcCol.rgb = (alpha > 0.0f) ? (srcCol.rgb / alpha) : 0.0f;
    if (all(srcCol.rgb < 0.01f))
    {
        discard;
    }
    
    
    //背景の黒を透過させる
    //float alpha = srcCol.a;
    //srcCol.rgb = (alpha > 0.0f) ? (srcCol.rgb / alpha) : float3(0.0f, 0.0f, 0.0f);
    

    
    return g_color * srcCol;
    
}