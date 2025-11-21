#include "../Common/Pixel/PixelShader2DHeader.hlsli"
// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float4 g_color;
    float4 g_outLineColor;
    float g_outLineWidth;
    float g_outLinePower;
    float g_time;
    float g_isOutLine;
    //float2 dummy;
}
float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    float2 uv = PSInput.uv;
    float4 srcCol = srcCol = tex.Sample(texSampler, uv);
    

    float alpha = srcCol.a;
    
    //周囲のピクセルを調べる
    float up = tex.Sample(texSampler, uv + float2(0.0, g_outLineWidth)).a;
    float down = tex.Sample(texSampler, uv + float2(0.0, -g_outLineWidth)).a;
    float left = tex.Sample(texSampler, uv + float2(-g_outLineWidth, 0.0)).a;
    float right = tex.Sample(texSampler, uv + float2(g_outLineWidth, 0.0)).a;
    
    // アルファ値にしきい値を適用し、輪郭を検出
    if (up < 0.1 || down < 0.1 || left < 0.1 || right < 0.1) // 適切なアルファしきい値を設定
    {
        // ここで元のピクセルが透明でないことも確認すると、オブジェクトの内部が塗りつぶされない
        if (alpha < 0.1)
        {
            return g_outLineColor;
        }
    }
    
    if (srcCol.a < 0.01)
    {
        discard;
    }
    //return outlineCol + srcCol;
    
    return g_color * srcCol;
}