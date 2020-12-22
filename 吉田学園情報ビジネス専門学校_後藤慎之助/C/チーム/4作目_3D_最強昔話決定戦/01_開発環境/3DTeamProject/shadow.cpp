//=============================================================================
//
// ポリゴンの影処理 [shadow.cpp]
// Author:KUDO AYANO
//
//=============================================================================
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SHADOW 512
#define SHADOW_TEXTUER "TEXTURE/shadow.png"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;		//ポリゴンのテクスチャへのポインタ
SHADOW g_Shadow[MAX_SHADOW];//影の情報

//=============================================================================
// 初期化処理
// Author:KUDO AYANO
//=============================================================================
void InitShadow(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得

    //頂点バッファの生成
    pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_3D,		//頂点フォーマット
        D3DPOOL_MANAGED,//（固定）
        &g_pVtxBuffShadow,//変数名が変わると変更が必要
        NULL);

    D3DXCreateTextureFromFile(pDevice, SHADOW_TEXTUER, &g_pTextureShadow);//テクスチャの読み込み

    VERTEX_3D *pVtx = NULL;//頂点情報のポインタ

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと
    for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
    {
        //構造体の初期化
        g_Shadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Shadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Shadow[nCntShadow].fDiameter = 0.0f;
        g_Shadow[nCntShadow].fDiameterMemory = 0.0f;
        g_Shadow[nCntShadow].fHeight = 0.0f;
        g_Shadow[nCntShadow].fHeightOld = 0.0f;
        g_Shadow[nCntShadow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, SHADOW_ALPHA_MAX);
        g_Shadow[nCntShadow].use = false;

        //頂点座標の設定
        pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

        //法線ベクトル
        pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

        //頂点カラーの設定
        pVtx[0].col = g_Shadow[nCntShadow].col;
        pVtx[1].col = g_Shadow[nCntShadow].col;
        pVtx[2].col = g_Shadow[nCntShadow].col;
        pVtx[3].col = g_Shadow[nCntShadow].col;

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
// 更新処理
// Author:後藤慎之助
//=============================================================================
void UpdateShadow(void)
{
    VERTEX_3D *pVtx = NULL;//頂点情報のポインタ

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
    {
        //使われているなら
        if (g_Shadow[nCntShadow].use == true)
        {
            //地面に影をつける
            g_Shadow[nCntShadow].pos.y = SHADOW_POS_Y;

            //高さに応じて、影の大きさと透明度を変える
            //下降中
            if (g_Shadow[nCntShadow].fHeight < g_Shadow[nCntShadow].fHeightOld)
            {
                g_Shadow[nCntShadow].fDiameter = g_Shadow[nCntShadow].fDiameter + g_Shadow[nCntShadow].fHeight / SHADOW_SIZE_RATE;
                g_Shadow[nCntShadow].col.a += SHADOW_ALPHA_RATE;
            }
            //上降中
            else if (g_Shadow[nCntShadow].fHeight > g_Shadow[nCntShadow].fHeightOld)
            {
                g_Shadow[nCntShadow].fDiameter = g_Shadow[nCntShadow].fDiameter - g_Shadow[nCntShadow].fHeight / SHADOW_SIZE_RATE;
                g_Shadow[nCntShadow].col.a -= SHADOW_ALPHA_RATE;
            }
            //同じなら
            else if (g_Shadow[nCntShadow].fHeight == g_Shadow[nCntShadow].fHeightOld)
            {
                //地面からの高さが0
                if (g_Shadow[nCntShadow].fHeight == 0.0f)
                {
                    g_Shadow[nCntShadow].fDiameter = g_Shadow[nCntShadow].fDiameterMemory;  //元の大きさにする
                    g_Shadow[nCntShadow].col.a = SHADOW_ALPHA_MAX;
                }
            }

            //大きさの上限、下限
            //上限
            if (g_Shadow[nCntShadow].fDiameter > g_Shadow[nCntShadow].fDiameterMemory)
            {
                g_Shadow[nCntShadow].fDiameter = g_Shadow[nCntShadow].fDiameterMemory;  //元の大きさにする
            }
            //下限
            if (g_Shadow[nCntShadow].fDiameter < 0.0f)
            {
                g_Shadow[nCntShadow].fDiameter = 0.0f;  //0を下回らないようにする
            }

            //透明度の上限、下限
            //上限
            if (g_Shadow[nCntShadow].col.a > SHADOW_ALPHA_MAX)
            {
                g_Shadow[nCntShadow].col.a = SHADOW_ALPHA_MAX;
            }
            //下限
            if (g_Shadow[nCntShadow].col.a < SHADOW_ALPHA_MIN)
            {
                g_Shadow[nCntShadow].col.a = SHADOW_ALPHA_MIN;
            }

            //1F前の高さを記録
            g_Shadow[nCntShadow].fHeightOld = g_Shadow[nCntShadow].fHeight;

            //頂点座標の設定
            pVtx[0].pos = D3DXVECTOR3(-(g_Shadow[nCntShadow].fDiameter / 2), 0.0f, (g_Shadow[nCntShadow].fDiameter / 2));
            pVtx[1].pos = D3DXVECTOR3((g_Shadow[nCntShadow].fDiameter / 2), 0.0f, (g_Shadow[nCntShadow].fDiameter / 2));
            pVtx[2].pos = D3DXVECTOR3(-(g_Shadow[nCntShadow].fDiameter / 2), 0.0f, -(g_Shadow[nCntShadow].fDiameter / 2));
            pVtx[3].pos = D3DXVECTOR3((g_Shadow[nCntShadow].fDiameter / 2), 0.0f, -(g_Shadow[nCntShadow].fDiameter / 2));

            //頂点カラーの設定
            pVtx[0].col = g_Shadow[nCntShadow].col;
            pVtx[1].col = g_Shadow[nCntShadow].col;
            pVtx[2].col = g_Shadow[nCntShadow].col;
            pVtx[3].col = g_Shadow[nCntShadow].col;
        }

        pVtx += 4;  //頂点データを4つ分進める
    }

    //頂点データをアンロックする
    g_pVtxBuffShadow->Unlock();
}

//=============================================================================
// 影の設定
// Author:後藤慎之助
//=============================================================================
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fDiameter, float fHeight ,D3DXCOLOR col)
{
    VERTEX_3D *pVtx = NULL;//頂点情報のポインタ

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

    int nCntShadow = 0;
    for (; nCntShadow < MAX_SHADOW; nCntShadow++)
    {
        if (!g_Shadow[nCntShadow].use)
        {
            g_Shadow[nCntShadow].use = true;
            g_Shadow[nCntShadow].pos = pos;
            g_Shadow[nCntShadow].rot = rot;
            g_Shadow[nCntShadow].fDiameter = fDiameter;
            g_Shadow[nCntShadow].fDiameterMemory = fDiameter;
            g_Shadow[nCntShadow].fHeight = fHeight;
            g_Shadow[nCntShadow].fHeightOld = fHeight;
            g_Shadow[nCntShadow].col.a = col.a;     //透明度を結びつける

            //頂点座標の設定
            pVtx[0].pos = D3DXVECTOR3(-(g_Shadow[nCntShadow].fDiameter / 2), 0.0f, (g_Shadow[nCntShadow].fDiameter / 2));
            pVtx[1].pos = D3DXVECTOR3((g_Shadow[nCntShadow].fDiameter / 2), 0.0f, (g_Shadow[nCntShadow].fDiameter / 2));
            pVtx[2].pos = D3DXVECTOR3(-(g_Shadow[nCntShadow].fDiameter / 2), 0.0f, -(g_Shadow[nCntShadow].fDiameter / 2));
            pVtx[3].pos = D3DXVECTOR3((g_Shadow[nCntShadow].fDiameter / 2), 0.0f, -(g_Shadow[nCntShadow].fDiameter / 2));

            break;
        }

        pVtx += 4;  //頂点データを4つ分進める
    }

    //頂点データをアンロックする
    g_pVtxBuffShadow->Unlock();

    return nCntShadow;
}

//=============================================================================
// 影の情報の送信
// Author:KUDO AYANO
//=============================================================================
SHADOW *GetShadow()
{
    return &g_Shadow[0];
}

//=============================================================================
// 影の設定
// Author:KUDO AYANO
//=============================================================================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, float fHeight)
{
    //高さ以外を紐づけ
    g_Shadow[nIdxShadow].pos.x = pos.x;
    g_Shadow[nIdxShadow].pos.z = pos.z;

    g_Shadow[nIdxShadow].fHeight = fHeight;
}

//=============================================================================
// 描画処理
// Author:KUDO AYANO
//=============================================================================
void DrawShadow(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタを取得
    D3DXMATRIX mtxRot, mtxTrans;//行列計算用のマトリクス
    for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
    {
        if (g_Shadow[nCntShadow].use == true)
        {
            //減算合成に設定
            pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
            pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

            pDevice->SetTexture(0, g_pTextureShadow);

            //ワールドマトリクスの初期化
            D3DXMatrixIdentity(&g_Shadow[nCntShadow].mtxWorld);
            //向きを反転
            D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Shadow[nCntShadow].rot.y, g_Shadow[nCntShadow].rot.x, g_Shadow[nCntShadow].rot.z);
            D3DXMatrixMultiply(&g_Shadow[nCntShadow].mtxWorld, &g_Shadow[nCntShadow].mtxWorld, &mtxRot);
            //位置を反転
            D3DXMatrixTranslation(&mtxTrans, g_Shadow[nCntShadow].pos.x, g_Shadow[nCntShadow].pos.y, g_Shadow[nCntShadow].pos.z);
            D3DXMatrixMultiply(&g_Shadow[nCntShadow].mtxWorld, &g_Shadow[nCntShadow].mtxWorld, &mtxTrans);
            //ワールドマトリックスの設定
            pDevice->SetTransform(D3DTS_WORLD, &g_Shadow[nCntShadow].mtxWorld);
            //頂点バッファをデバイスのデータストリームに設定
            pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));
            //頂点フォーマットの設定
            pDevice->SetFVF(FVF_VERTEX_3D);
            //ポリゴンの描画
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntShadow, NUM_POLYGON);
            //レンダーステート設定をもとに戻す
            pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
            pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        }
    }

    //テクスチャの設定の解除
    pDevice->SetTexture(0, NULL);
}

//=============================================================================
// 終了処理
// Author:KUDO AYANO
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

