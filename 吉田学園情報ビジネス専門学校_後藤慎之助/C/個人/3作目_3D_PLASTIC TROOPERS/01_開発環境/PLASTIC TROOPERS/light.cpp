//=============================================================================
//
// DirectX3Dの処理 [light.cpp] (照明)
// Author : 後藤慎之助
//
//=============================================================================
#include "light.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
D3DLIGHT9 g_Light;	//ライトの情報

					//=============================================================================
					// ライト初期化処理
					//=============================================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスポインタ
	D3DXVECTOR3 vecDir;	//ライトの方向ベクトル

						//ライトをクリアする
	ZeroMemory(&g_Light, sizeof(D3DLIGHT9));

	//ライトの種類を設定
	g_Light.Type = D3DLIGHT_DIRECTIONAL;

	//ライトの拡散光を設定
	g_Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//ライトの方向を設定
	vecDir = D3DXVECTOR3(0.2f, -0.8f, 0.4f);
	D3DXVec3Normalize(&vecDir, &vecDir);	//正規化する(大きさ1のベクトルにする必要がある)
	g_Light.Direction = vecDir;

	//ライトを設定する
	pDevice->SetLight(0, &g_Light);

	//ライトを有効にする
	pDevice->LightEnable(0, TRUE);

	//// テクスチャ・フィルタリング
	//pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// 拡大用(バイリニア・フィルタリング)
	//pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// 縮小用(バイリニア・フィルタリング)

	//pDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// 拡大用(バイリニア・フィルタリング)
	//pDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// 縮小用(バイリニア・フィルタリング)


	//pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);	// Zバッファ使う
	//pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);	// 頂点法線の自動正規化


	//															// ステンシルバッファの設定
	//pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_NOTEQUAL);
	//pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	//pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	//pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

}

//=============================================================================
//ライト終了処理
//=============================================================================
void UninitLight(void)
{

}

//=============================================================================
// ライト更新処理
//=============================================================================
void UpdateLight(void)
{

}