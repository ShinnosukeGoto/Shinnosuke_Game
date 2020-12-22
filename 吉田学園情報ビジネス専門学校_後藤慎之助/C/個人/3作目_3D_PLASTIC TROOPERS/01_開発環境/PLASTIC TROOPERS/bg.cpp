//=============================================================================
//
// PLASTIC TROOPERSの処理 [bg.cpp] (宇宙の背景)
// Author : 後藤慎之助
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "input.h"
#include "bg.h"

//=============================================================================
// グローバル変数
//=============================================================================

//メッシュフィールドのテクスチャ
LPDIRECT3DTEXTURE9 g_pTextureBg = NULL;
//頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg = NULL;
//インデックスバッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffBg = NULL;

//メッシュフィールドのワールドマトリックス
D3DXMATRIX g_mtxWorldBg;

BG g_Bg;//フィールドの情報

//=============================================================================
//バックグラウンド初期化処理
//=============================================================================
void InitBg(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    g_Bg.nBgVertexNUM = (g_Bg.nBgBlockX + 1) * (g_Bg.nBgBlockZ + 1);
    g_Bg.nBgIndexNUM = ((g_Bg.nBgBlockX + 1) * 2) * g_Bg.nBgBlockZ + ((g_Bg.nBgBlockZ - 1) * 2);
    g_Bg.nBgPrimitiveNUM = ((g_Bg.nBgBlockX * g_Bg.nBgBlockZ) * 2) + ((g_Bg.nBgBlockZ - 1) * 4);

    //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice,
        "TEXTURE/宇宙.jpg",//ファイルの名前
        &g_pTextureBg);
    //頂点バッファ生成
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_3D)* g_Bg.nBgVertexNUM,//バッファサイズ
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_3D,
        D3DPOOL_MANAGED,
        &g_pVtxBuffBg,
        NULL);//頂点情報へのポインタ

    VERTEX_3D *pVtx;//頂点情報のポインタ

                    //頂点バッファをロック
    g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

    int nCntPos = 0;
    int nRadius = 50;
    float fAngle = 360.0f / g_Bg.nBgBlockX;
    float fRadiusAngle = 0;
    float fRadius = 0;
    float fRadiusY = 0;

    for (int nCntMeshZ = 0; nCntMeshZ < g_Bg.nBgBlockZ + 1; nCntMeshZ++)
    {
        fRadiusAngle = 90 - (180 / g_Bg.nBgBlockZ) * nCntMeshZ;
        fRadius = cosf(D3DXToRadian(fRadiusAngle)) * BG_SIZE;
        fRadiusY = sinf(D3DXToRadian(fRadiusAngle)) * BG_SIZE;

        for (int nCntMeshX = 0; nCntMeshX < g_Bg.nBgBlockX + 1; nCntMeshX++)
        {
            //pVtx[nCntPos].pos = D3DXVECTOR3(sinf(D3DXToRadian(fAngle * nCntMeshX)) * Bg_SIZE, nRadius * nCntMeshZ, cosf(D3DXToRadian(fAngle * nCntMeshX)) * Bg_SIZE);

            pVtx[nCntPos].pos = D3DXVECTOR3((sinf(D3DXToRadian(fAngle * nCntMeshX)) * fRadius), fRadiusY, (cosf(D3DXToRadian(fAngle * nCntMeshX)))* fRadius);
            nCntPos++;
        }
    }

    //法線の設定
    for (int nCntnor = 0; nCntnor < g_Bg.nBgVertexNUM; nCntnor++)
    {
        //頂点カラーの設定
        pVtx[nCntnor].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    }

    //テクスチャ座標の設定
    int nCntTex = 0;

    for (int nCntMeshY = 0; nCntMeshY< g_Bg.nBgBlockZ + 1; nCntMeshY++)
    {
        for (int nCntMeshX = 0; nCntMeshX < g_Bg.nBgBlockX + 1; nCntMeshX++)
        {
            pVtx[nCntTex].tex = D3DXVECTOR2(nCntMeshX, nCntMeshY);

            nCntTex++;
        }
    }

    for (int nCntcol = 0; nCntcol < g_Bg.nBgVertexNUM; nCntcol++)
    {
        //頂点カラーの設定
        pVtx[nCntcol].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    }

    //頂点データをアンロックする
    g_pVtxBuffBg->Unlock();

    int nCntIndex = 0;
    int nCntMeshX;
    int nCntMeshZ;

    pDevice->CreateIndexBuffer(
        sizeof(WORD) * g_Bg.nBgIndexNUM,//インデックスの数
        D3DUSAGE_WRITEONLY,
        D3DFMT_INDEX16,
        D3DPOOL_MANAGED,
        &g_pIdxBuffBg,
        NULL);//頂点情報へのポインタ

              //インデックスバッファをロック
    WORD* pIdx;
    g_pIdxBuffBg->Lock(0, 0, (void**)&pIdx, 0);

    for (nCntMeshZ = 0; nCntMeshZ < g_Bg.nBgBlockZ; nCntMeshZ++)
    {
        if (nCntMeshZ != 0)
        {
            pIdx[nCntIndex] = nCntMeshX + (g_Bg.nBgBlockX + 1) * (nCntMeshZ);

            nCntIndex++;
        }

        for (nCntMeshX = 0; nCntMeshX < g_Bg.nBgBlockX + 1; nCntMeshX++)
        {
            pIdx[nCntIndex] = nCntMeshX + (g_Bg.nBgBlockX + 1) * (nCntMeshZ + 1);

            nCntIndex++;

            pIdx[nCntIndex] = nCntMeshX + (g_Bg.nBgBlockX + 1) * (nCntMeshZ);

            nCntIndex++;
        }

        if (nCntMeshZ != g_Bg.nBgBlockZ - 1)
        {
            pIdx[nCntIndex] = nCntMeshX - 1 + (g_Bg.nBgBlockX + 1) * (nCntMeshZ);

            nCntIndex++;
        }
    }

    //インデックスバッファのアンロック
    g_pIdxBuffBg->Unlock();
}

//=============================================================================
// バックグラウンド終了処理
//=============================================================================
void UninitBg(void)
{
    //テクスチャの解放
    if (g_pTextureBg != NULL)
    {
        g_pTextureBg->Release();
        g_pTextureBg = NULL;
    }
    //頂点バッファの開放
    if (g_pVtxBuffBg != NULL)
    {
        g_pVtxBuffBg->Release();
        g_pVtxBuffBg = NULL;
    }
    //インデックスバッファの開放
    if (g_pIdxBuffBg != NULL)
    {
        g_pIdxBuffBg->Release();
        g_pIdxBuffBg = NULL;
    }
}

//=============================================================================
//バックグラウンド更新処理
//=============================================================================
void UpdateBg(void)
{
    g_Bg.rot.y += BG_ROT_SPEED;
}


//=============================================================================
//バックグラウンド描画処理
//=============================================================================
void DrawBg(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスポインタ
    D3DXMATRIX mtxRot, mtxTrans;//行列計算用のマトリクス

    //アルファテストを有効化
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    //アルファテスト基準値の設定
    pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
    //アルファテストの比較方法の設定（GREATERは基準値より大きい場合)
    pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    //ワールドマトリクスの初期化
    D3DXMatrixIdentity(&g_mtxWorldBg);
    //向きを反映
    D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Bg.rot.y, g_Bg.rot.x, g_Bg.rot.z);
    D3DXMatrixMultiply(&g_mtxWorldBg, &g_mtxWorldBg, &mtxRot);
    //位置を反映
    D3DXMatrixTranslation(&mtxTrans, g_Bg.pos.x, g_Bg.pos.y, g_Bg.pos.z);
    D3DXMatrixMultiply(&g_mtxWorldBg, &g_mtxWorldBg, &mtxTrans);
    //ワールドマトリックスの設定
    pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBg);
    pDevice->SetTexture(0, g_pTextureBg);
    //インデックスバッファをデータストリームに設定
    pDevice->SetIndices(g_pIdxBuffBg);
    //頂点バッファをデバイスのデータストリームに設定
    pDevice->SetStreamSource(0, g_pVtxBuffBg, 0, sizeof(VERTEX_3D));
    //頂点フォーマットの設定
    pDevice->SetFVF(FVF_VERTEX_3D);
    //ポリゴンの描画
    pDevice->DrawIndexedPrimitive(
        D3DPT_TRIANGLESTRIP,
        0, 0,
        g_Bg.nBgVertexNUM,//頂点数
        0,
        g_Bg.nBgPrimitiveNUM//ポリゴン数
    );

    //アルファテストを無効化
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}