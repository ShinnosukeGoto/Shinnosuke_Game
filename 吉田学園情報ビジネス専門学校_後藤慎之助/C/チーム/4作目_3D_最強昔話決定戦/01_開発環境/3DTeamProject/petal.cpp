//=============================================================================
//
// 最強昔話決定戦[petal.cpp]の処理 (花びら）
// Author :後藤慎之助
//
//=============================================================================
#include "petal.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTexturePetal[PETALTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPetal = NULL;
PETAL g_aPetal[MAX_PETAL];						//花びらの情報

//=============================================================================
// 花びら初期化処理
// Author:後藤慎之助
//=============================================================================
void InitPetal(void)
{
    //宣言
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/petal.png", &g_pTexturePetal[PETALTYPE_000]);	//桜の花びら

    //頂点バッファ生成
    pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*MAX_PETAL, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPetal, NULL);
    VERTEX_2D *pVtx = NULL;	//頂点情報へのポインタ

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffPetal->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

    //花びらのpVtxを生成
    for (int nCntPetal = 0; nCntPetal < MAX_PETAL; nCntPetal++)
    {
        g_aPetal[nCntPetal].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aPetal[nCntPetal].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aPetal[nCntPetal].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        g_aPetal[nCntPetal].fSize = 0.0f;
        g_aPetal[nCntPetal].fAngle = 0.0f;
        g_aPetal[nCntPetal].fRotSpeed = 0.0f;
        g_aPetal[nCntPetal].type = PETALTYPE_000;
        g_aPetal[nCntPetal].bUse = false;

        g_aPetal[nCntPetal].vertex1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aPetal[nCntPetal].vertex2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aPetal[nCntPetal].vertex3 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aPetal[nCntPetal].vertex4 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

        //頂点座標の設定（右回りで設定する）
        pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

        //rhwの設定（値は1.0で設定する）
        pVtx[0].rhw = 1.0f;
        pVtx[1].rhw = 1.0f;
        pVtx[2].rhw = 1.0f;
        pVtx[3].rhw = 1.0f;

        //頂点カラーを設定（0～255の値で設定する）
        pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

        //テクスチャ座標の設定
        pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

        pVtx += 4;//頂点データのポインタを4つ分進める
    }

    //頂点データをアンロックする
    g_pVtxBuffPetal->Unlock();
}

//=============================================================================
// 花びら終了処理
// Author:後藤慎之助
//=============================================================================
void UninitPetal(void)
{
    //テクスチャの開放
    for (int nCntPetal = 0; nCntPetal < PETALTYPE_MAX; nCntPetal++)
    {
        if (g_pTexturePetal[nCntPetal] != NULL)
        {
            g_pTexturePetal[nCntPetal]->Release();
            g_pTexturePetal[nCntPetal] = NULL;
        }
    }

    //頂点バッファの開放
    if (g_pVtxBuffPetal != NULL)
    {
        g_pVtxBuffPetal->Release();
        g_pVtxBuffPetal = NULL;
    }
}

//=============================================================================
// 花びら更新処理
// Author:後藤慎之助
//=============================================================================
void UpdatePetal(void)
{
    //宣言
    LPDIRECT3DDEVICE9 pDevice = GetDevice();
    VERTEX_2D *pVtx = NULL;

    //頂点バッファをロック
    g_pVtxBuffPetal->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntPetal = 0; nCntPetal < MAX_PETAL; nCntPetal++)
    {
        //使われているなら
        if (g_aPetal[nCntPetal].bUse == true)
        {
            //移動量と位置を紐づけ
            g_aPetal[nCntPetal].pos += g_aPetal[nCntPetal].move;

            //角度に、回転速度を足す
            g_aPetal[nCntPetal].fAngle += g_aPetal[nCntPetal].fRotSpeed;

            //==========================================================================================================
            //画像を、画像の中心を軸に回転させる
            //左上の頂点
            g_aPetal[nCntPetal].vertex1.x = -(g_aPetal[nCntPetal].fSize / 2)*cosf(g_aPetal[nCntPetal].fAngle)
                - ( - (g_aPetal[nCntPetal].fSize / 2))*sinf(g_aPetal[nCntPetal].fAngle);
            g_aPetal[nCntPetal].vertex1.y = -(g_aPetal[nCntPetal].fSize / 2)*sinf(g_aPetal[nCntPetal].fAngle)
                + ( - (g_aPetal[nCntPetal].fSize / 2))*cosf(g_aPetal[nCntPetal].fAngle);

            //右上の頂点
            g_aPetal[nCntPetal].vertex2.x = (g_aPetal[nCntPetal].fSize / 2)*cosf(g_aPetal[nCntPetal].fAngle)
                - ( - (g_aPetal[nCntPetal].fSize / 2))*sinf(g_aPetal[nCntPetal].fAngle);
            g_aPetal[nCntPetal].vertex2.y = (g_aPetal[nCntPetal].fSize / 2)*sinf(g_aPetal[nCntPetal].fAngle)
                + ( - (g_aPetal[nCntPetal].fSize / 2))*cosf(g_aPetal[nCntPetal].fAngle);

            //左下の頂点
            g_aPetal[nCntPetal].vertex3.x = -(g_aPetal[nCntPetal].fSize / 2)*cosf(g_aPetal[nCntPetal].fAngle)
                - (g_aPetal[nCntPetal].fSize / 2)*sinf(g_aPetal[nCntPetal].fAngle);
            g_aPetal[nCntPetal].vertex3.y = -(g_aPetal[nCntPetal].fSize / 2)*sinf(g_aPetal[nCntPetal].fAngle)
                + (g_aPetal[nCntPetal].fSize / 2)*cosf(g_aPetal[nCntPetal].fAngle);

            //右下の頂点
            g_aPetal[nCntPetal].vertex4.x = (g_aPetal[nCntPetal].fSize / 2)*cosf(g_aPetal[nCntPetal].fAngle)
                - (g_aPetal[nCntPetal].fSize / 2)*sinf(g_aPetal[nCntPetal].fAngle);
            g_aPetal[nCntPetal].vertex4.y = (g_aPetal[nCntPetal].fSize / 2)*sinf(g_aPetal[nCntPetal].fAngle)
                + (g_aPetal[nCntPetal].fSize / 2)*cosf(g_aPetal[nCntPetal].fAngle);
            //==========================================================================================================

            //頂点情報の更新
            pVtx[0].pos = D3DXVECTOR3(g_aPetal[nCntPetal].pos.x, g_aPetal[nCntPetal].pos.y , 0.0f) + g_aPetal[nCntPetal].vertex1;
            pVtx[1].pos = D3DXVECTOR3(g_aPetal[nCntPetal].pos.x, g_aPetal[nCntPetal].pos.y , 0.0f) + g_aPetal[nCntPetal].vertex2;
            pVtx[2].pos = D3DXVECTOR3(g_aPetal[nCntPetal].pos.x, g_aPetal[nCntPetal].pos.y , 0.0f) + g_aPetal[nCntPetal].vertex3;
            pVtx[3].pos = D3DXVECTOR3(g_aPetal[nCntPetal].pos.x, g_aPetal[nCntPetal].pos.y , 0.0f) + g_aPetal[nCntPetal].vertex4;

            //頂点カラー
            pVtx[0].col = g_aPetal[nCntPetal].col;
            pVtx[1].col = g_aPetal[nCntPetal].col;
            pVtx[2].col = g_aPetal[nCntPetal].col;
            pVtx[3].col = g_aPetal[nCntPetal].col;

            //画面外で未使用に
            switch (g_aPetal[nCntPetal].type)
            {
                //花びらの場合
            case PETALTYPE_000:
                //左
                if (g_aPetal[nCntPetal].pos.x < -g_aPetal[nCntPetal].fSize)
                {
                    g_aPetal[nCntPetal].bUse = false;
                }

                //下
                if (g_aPetal[nCntPetal].pos.y > SCREEN_HEIGHT + g_aPetal[nCntPetal].fSize)
                {
                    g_aPetal[nCntPetal].bUse = false;
                }

                break;
            }
        }
        pVtx += 4;
    }

    //頂点データをアンロックする
    g_pVtxBuffPetal->Unlock();
}

//=============================================================================
// 花びら描画処理
// Author:後藤慎之助
//=============================================================================
void DrawPetal(void)
{
    //宣言
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffPetal, 0, sizeof(VERTEX_2D));

    //頂点フォーマットの設定
    pDevice->SetFVF(FVF_VERTEX_2D);

    //ポリゴンの描画
    for (int nCntPetal = 0; nCntPetal < MAX_PETAL; nCntPetal++)
    {
        if (g_aPetal[nCntPetal].bUse == true)	//花びらが使用中なら描画
        {
            //テクスチャの設定
            pDevice->SetTexture(0, g_pTexturePetal[g_aPetal[nCntPetal].type]);

            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPetal * 4, 2);
        }
    }
}

//=============================================================================
// 花びら設定処理
// Author:後藤慎之助
//=============================================================================
void SetPetal(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSize, float fAngle, float fRotSpeed, PETALTYPE type)
{
    //変数宣言
    VERTEX_2D *pVtx = NULL;

    //頂点バッファをロック
    g_pVtxBuffPetal->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntPetal = 0; nCntPetal < MAX_PETAL; nCntPetal++)
    {
        if (g_aPetal[nCntPetal].bUse == false)
        {
            g_aPetal[nCntPetal].pos = pos;
            g_aPetal[nCntPetal].move = move;
            g_aPetal[nCntPetal].col = col;
            g_aPetal[nCntPetal].fSize = fSize;
            g_aPetal[nCntPetal].fAngle = fAngle;
            g_aPetal[nCntPetal].fRotSpeed = fRotSpeed;
            g_aPetal[nCntPetal].type = type;

            //頂点に対する構造体の情報の初期化
            g_aPetal[nCntPetal].vertex1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            g_aPetal[nCntPetal].vertex2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            g_aPetal[nCntPetal].vertex3 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            g_aPetal[nCntPetal].vertex4 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

            //頂点情報の更新
            //頂点座標
            pVtx[0].pos = D3DXVECTOR3(g_aPetal[nCntPetal].pos.x - (g_aPetal[nCntPetal].fSize / 2), g_aPetal[nCntPetal].pos.y - (g_aPetal[nCntPetal].fSize / 2), 0.0f);
            pVtx[1].pos = D3DXVECTOR3(g_aPetal[nCntPetal].pos.x + (g_aPetal[nCntPetal].fSize / 2), g_aPetal[nCntPetal].pos.y - (g_aPetal[nCntPetal].fSize / 2), 0.0f);
            pVtx[2].pos = D3DXVECTOR3(g_aPetal[nCntPetal].pos.x - (g_aPetal[nCntPetal].fSize / 2), g_aPetal[nCntPetal].pos.y + (g_aPetal[nCntPetal].fSize / 2), 0.0f);
            pVtx[3].pos = D3DXVECTOR3(g_aPetal[nCntPetal].pos.x + (g_aPetal[nCntPetal].fSize / 2), g_aPetal[nCntPetal].pos.y + (g_aPetal[nCntPetal].fSize / 2), 0.0f);

            g_aPetal[nCntPetal].bUse = true;

            break;
        }
        pVtx += 4;
    }

    //頂点データをアンロックする
    g_pVtxBuffPetal->Unlock();
}