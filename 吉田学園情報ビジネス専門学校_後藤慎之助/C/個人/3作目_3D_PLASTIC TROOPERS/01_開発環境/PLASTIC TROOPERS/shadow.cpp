//=============================================================================
//
// 3Dゲームのひな形の処理[sahdow.cpp]
// Author:後藤慎之助
//
//=============================================================================
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SHADOW_SIZE (8.0f)
#define MAX_SHADOW (2)
#define SHADOW_TEXTUER "TEXTURE/shadow.png"
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;		//ポリゴンのテクスチャへのポインタ
SHADOW g_Shadow[MAX_SHADOW];//影の情報

                            //=============================================================================
                            //初期化処理
                            //=============================================================================
void InitShadow(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
    for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
    {
        g_Shadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Shadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Shadow[nCntShadow].bUse = false;
    }

    D3DXCreateTextureFromFile(pDevice, SHADOW_TEXTUER, &g_pTextureShadow);//テクスチャの読み込み

                                                                          //頂点バッファの生成
    pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_SHADOW,
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_3D,		//頂点フォーマット
        D3DPOOL_MANAGED,//（固定）
        &g_pVtxBuffShadow,//変数名が変わると変更が必要
        NULL);

    VERTEX_3D *pVtx;//頂点情報のポインタ

                    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
    {
        //頂点座標の初期化（右回りで設定する）
        pVtx[0].pos = D3DXVECTOR3(0.0f - SHADOW_SIZE, 0.0f, 0.0f + SHADOW_SIZE);
        pVtx[1].pos = D3DXVECTOR3(0.0f + SHADOW_SIZE, 0.0f, 0.0f + SHADOW_SIZE);
        pVtx[2].pos = D3DXVECTOR3(0.0f - SHADOW_SIZE, 0.0f, 0.0f - SHADOW_SIZE);
        pVtx[3].pos = D3DXVECTOR3(0.0f + SHADOW_SIZE, 0.0f, 0.0f - SHADOW_SIZE);

        //法線ベクトル
        pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

        //頂点カラーの設定
        pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

        pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

        pVtx += 4;
    }
    //頂点データアンロック
    g_pVtxBuffShadow->Unlock();
}
//=============================================================================
//更新処理
//=============================================================================
void UpdateShadow(void)
{

}
//=============================================================================
//影の設定
//=============================================================================
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
    int nCntShadow = 0;
    for (; nCntShadow < MAX_SHADOW; nCntShadow++)
    {
        if (g_Shadow[nCntShadow].bUse == false)
        {

            g_Shadow[nCntShadow].bUse = true;
            g_Shadow[nCntShadow].pos = pos;
            g_Shadow[nCntShadow].rot = rot;
            break;
        }
    }
    return nCntShadow;
}
//=============================================================================
//影の設定
//model.cppで呼び出す
//=============================================================================
void SetPositionShadow(int nldxShadow, D3DXVECTOR3 pos)
{
    g_Shadow[nldxShadow].pos = pos;
}
//=============================================================================
//描画処理
//=============================================================================
void DrawShadow(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタを取得
    D3DXMATRIX mtxRot, mtxTrans;//行列計算用のマトリクス

    //減算合成に設定
    pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
    pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    //テクスチャの設定
    pDevice->SetTexture(0, g_pTextureShadow);

    for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
    {

        if (g_Shadow[nCntShadow].bUse == true)
        {
            //ワールドマトリクスの初期化
            D3DXMatrixIdentity(&g_Shadow[nCntShadow].mtxWorld);
            //向きを反映
            D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Shadow[nCntShadow].rot.y, g_Shadow[nCntShadow].rot.x, g_Shadow[nCntShadow].rot.z);
            D3DXMatrixMultiply(&g_Shadow[nCntShadow].mtxWorld, &g_Shadow[nCntShadow].mtxWorld, &mtxRot);
            //位置を反映
            D3DXMatrixTranslation(&mtxTrans, g_Shadow[nCntShadow].pos.x, g_Shadow[nCntShadow].pos.y, g_Shadow[nCntShadow].pos.z);
            D3DXMatrixMultiply(&g_Shadow[nCntShadow].mtxWorld, &g_Shadow[nCntShadow].mtxWorld, &mtxTrans);
            //ワールドマトリックスの設定
            pDevice->SetTransform(D3DTS_WORLD, &g_Shadow[nCntShadow].mtxWorld);
            //頂点バッファをデバイスのデータストリームに設定
            pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));
            //頂点フォーマットの設定
            pDevice->SetFVF(FVF_VERTEX_3D);
            //ポリゴンの描画
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
        }
    }

    //レンダーステート設定をもとに戻す
    pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

    //テクスチャの設定の解除
    pDevice->SetTexture(0, NULL);
}
//=============================================================================
//終了処理
//=============================================================================
void UninitShadow(void)
{
    //頂点バッファの開放
    if (g_pVtxBuffShadow != NULL)
    {
        g_pVtxBuffShadow->Release();
        g_pVtxBuffShadow = NULL;
    }
    //テクスチャの開放
    if (g_pTextureShadow != NULL)
    {
        g_pTextureShadow->Release();
        g_pTextureShadow = NULL;
    }
}