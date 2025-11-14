// VS/PS共通

#include "../Common/VertexToPixelHeader.hlsli"

// IN

#include "../Common/Vertex/VertexInputType.hlsli"

#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_NMAP_1FRAME

// OUT

#define VS_OUTPUT VertexToPixelLit

#include "../Common/Vertex/VertexShader3DHeader.hlsli"
// 定数バッファ：スロット7番目(b7と書く)
cbuffer cbParam : register(b7)
{
	float g_time;		//爆発の進行度
	float3 centerPos;	//爆発の中心
}
VS_OUTPUT main(VS_INPUT VSInput)

{

	VS_OUTPUT ret;

	// 頂点座標変換 +++++++++++++++++++++++++++++++++++++( 開始 )
	float4 lLocalPosition;
	float4 lWorldPosition;
	float4 lViewPosition;

	// float3 → float4
	lLocalPosition.xyz = VSInput.pos;
	float3 dir = normalize(lLocalPosition - centerPos);
	float moveDist = g_time * 5.0f; // 5.0 は爆発の拡散速度
	// 頂点を移動
	float3 explodedPos = lLocalPosition.xyz + dir * moveDist;
	lLocalPosition.w = 1.0f;
	lLocalPosition.xyz = explodedPos;
	// ローカル座標をワールド座標に変換(剛体)
	lWorldPosition.w = 1.0f;
	lWorldPosition.xyz = mul(lLocalPosition, g_base.localWorldMatrix);
	ret.worldPos = lWorldPosition.xyz;
	// ワールド座標をビュー座標に変換
	lViewPosition.w = 1.0f;
	lViewPosition.xyz = mul(lWorldPosition, g_base.viewMatrix);
	ret.vwPos.xyz = lViewPosition.xyz;

	// ビュー座標を射影座標に変換
	ret.svPos = mul(lViewPosition, g_base.projectionMatrix);

	// 頂点座標変換 +++++++++++++++++++++++++++++++++++++( 終了 )


	// その他、ピクセルシェーダへ引継&初期化 ++++++++++++( 開始 )

	// UV座標

	ret.uv.x = VSInput.uv0.x;

	ret.uv.y = VSInput.uv0.y;

	// 法線をローカル空間からワールド空間へ変換

	ret.normal = normalize(

		mul(VSInput.norm, (float3x3)g_base.localWorldMatrix));

	// ディフューズカラー

	ret.diffuse = VSInput.diffuse;

	// ライト方向(ローカル)

	ret.lightDir = float3(0.0f, 0.0f, 0.0f);

	// ライトから見た座標

	ret.lightAtPos = float3(0.0f, 0.0f, 0.0f);

	// その他、ピクセルシェーダへ引継&初期化 ++++++++++++( 終了 )


	// 出力パラメータを返す

	return ret;

}
