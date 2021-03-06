//=============================================================================
//
// 最強昔話決定戦の処理 [light.cpp] (照明)
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
// Author:後藤慎之助
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

}

//=============================================================================
// ライト終了処理
// Author:後藤慎之助
//=============================================================================
void UninitLight(void)
{

}

//=============================================================================
// ライト更新処理
// Author:後藤慎之助
//=============================================================================
void UpdateLight(void)
{

}