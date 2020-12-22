//=============================================================================
//
// PLASTIC TROOPERS [item.cpp]の処理 (アイテム)
// Author:後藤慎之助
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "item.h"
#include "collision.h"
#include "effect2.h"
#include "player.h"
#include "score.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;   //頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureItem[ITEMTYPE_MAX];  //ポリゴンのテクスチャ

ITEM g_aItem[MAX_ITEM];    //エフェクトの情報

//=============================================================================
//アイテムの初期化処理
//=============================================================================
void InitItem(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();
    int nCntItem = 0;

    //テクスチャの読み込み
    //ハート
    g_aItem[nCntItem].type = ITEMTYPE_000;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/item003.png",//ファイルの名前
            &g_pTextureItem[ITEMTYPE_000]);
    }
    //ダイヤ
    g_aItem[nCntItem].type = ITEMTYPE_001;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/item001.png",//ファイルの名前
            &g_pTextureItem[ITEMTYPE_001]);
    }
    g_aItem[nCntItem].type = ITEMTYPE_002;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/magic001.png",//ファイルの名前
            &g_pTextureItem[ITEMTYPE_002]);
    }
    //頂点バッファ生成
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_3D)*NUM_VERTEX*MAX_ITEM,//バッファサイズ
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_3D,
        D3DPOOL_MANAGED,
        &g_pVtxBuffItem,
        NULL);//頂点情報へのポインタ

    VERTEX_3D *pVtx;//頂点情報のポインタ

                    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

    for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
    {
        g_aItem[nCntItem].bUse = false;

        g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aItem[nCntItem].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aItem[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        //g_aItem[nCntItem].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

        //頂点座標の設定
        pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        //法線の設定
        pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        //頂点カラーの設定
        pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        //テクスチャ座標の設定
        pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

        pVtx += 4;  //頂点データを4つ分進める
    }

    //頂点データをアンロックする
    g_pVtxBuffItem->Unlock();
}

//=============================================================================
// アイテムの終了処理
//=============================================================================
void UninitItem(void)
{
    //テクスチャの解放
    for (int nCntTex = 0; nCntTex < ITEMTYPE_MAX; nCntTex++)
    {
        //テクスチャの解放
        if (g_pTextureItem[nCntTex] != NULL)
        {
            g_pTextureItem[nCntTex]->Release();
            g_pTextureItem[nCntTex] = NULL;
        }
    }
    //
    if (g_pVtxBuffItem != NULL)
    {
        g_pVtxBuffItem->Release();
        g_pVtxBuffItem = NULL;
    }
}

//=============================================================================
//アイテムの更新処理
//=============================================================================
void UpdateItem(void)
{
    for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
    {
        if (g_aItem[nCntItem].bUse == true)
        {
            //ハート
            if (g_aItem[nCntItem].type == ITEMTYPE_000)
            {
                //移動量と位置を紐づけ
                g_aItem[nCntItem].pos.y += g_aItem[nCntItem].move.y;
                //重力
                g_aItem[nCntItem].move.y -= GRAVITY;
                //床の判定
                if(g_aItem[nCntItem].pos.y < 0 + g_aItem[nCntItem].size.y / 2)
                {
                    g_aItem[nCntItem].move.y = 0;
                    g_aItem[nCntItem].pos.y = 0 + g_aItem[nCntItem].size.y / 2;
                }
            }
        }
    }
}

//=============================================================================
//アイテムの描画処理
//=============================================================================
void DrawItem(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();
    D3DXMATRIX mtxRot, mtxTrans;//行列計算用のマトリクス

    //アルファテストを有効化
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    //アルファテスト基準値の設定
    pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
    //アルファテストの比較方法の設定
    pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    //マテリアルの設定
    D3DMATERIAL9 material;
    ZeroMemory(&material, sizeof(D3DMATERIAL9));
    material.Ambient.r = 1.0f;
    material.Ambient.g = 1.0f;
    material.Ambient.b = 1.0f;
    material.Ambient.a = 1.0f;
    pDevice->SetMaterial(&material);
    pDevice->SetRenderState(D3DRS_AMBIENT, 0x44444444);

    //ライトの影響を受けなくする
    DWORD ambient;
    pDevice->GetRenderState(D3DRS_AMBIENT, &ambient);
    pDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
    pDevice->LightEnable(0, FALSE);

    for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
    {
        if (g_aItem[nCntItem].bUse == true)
        {
            //ワールドマトリクスの初期化
            D3DXMatrixIdentity(&g_aItem[nCntItem].mtxWorld);
            //回転の逆行列を生成
            pDevice->GetTransform(D3DTS_VIEW, &mtxRot);
            D3DXMatrixInverse(&g_aItem[nCntItem].mtxWorld, NULL, &mtxRot);
            g_aItem[nCntItem].mtxWorld._41 = 0;
            g_aItem[nCntItem].mtxWorld._42 = 0;
            g_aItem[nCntItem].mtxWorld._43 = 0;

            ////位置を反映
            D3DXMatrixTranslation(&mtxTrans,
                g_aItem[nCntItem].pos.x,
                g_aItem[nCntItem].pos.y,
                g_aItem[nCntItem].pos.z);

            D3DXMatrixMultiply(
                &g_aItem[nCntItem].mtxWorld,
                &g_aItem[nCntItem].mtxWorld,
                &mtxTrans);

            //ワールドマトリックスの設定
            pDevice->SetTransform(D3DTS_WORLD, &g_aItem[nCntItem].mtxWorld);

            pDevice->SetTexture(0, g_pTextureItem[g_aItem[nCntItem].type]);
            //頂点バッファをデバイスのデータストリームに設定
            pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_3D));
            //頂点フォーマットの設定
            pDevice->SetFVF(FVF_VERTEX_3D);

            //ポリゴンの描画
            pDevice->DrawPrimitive(
                D3DPT_TRIANGLESTRIP,
                nCntItem * 4,
                NUM_POLYGON);
        }
    }

    //アルファテストを無効化
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    //ライトの影響を受けるようにする
    pDevice->SetRenderState(D3DRS_AMBIENT, ambient);
    pDevice->LightEnable(0, TRUE);

    //テクスチャの設定の解除
    pDevice->SetTexture(0, NULL);
}

//=============================================================================
//アイテムの設定
//=============================================================================
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, ITEMTYPE type)
{
    VERTEX_3D *pVtx;//頂点情報のポインタ
    //PLAYER *pPlayer;
    //pPlayer = GetPlayer();

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
    {


        if (/*pPlayer->pos.x > pos.x - CAMERA_VIEW_RANGE&&
            pPlayer->pos.x < pos.x + CAMERA_VIEW_RANGE&&
            pPlayer->pos.z > pos.z - CAMERA_VIEW_RANGE&&
            pPlayer->pos.z < pos.z + CAMERA_VIEW_RANGE&&*/
            g_aItem[nCntItem].bUse == false)
        {
            //初期化
            g_aItem[nCntItem].pos = pos;
            g_aItem[nCntItem].size = size;
            g_aItem[nCntItem].move = move;
            //g_aItem[nCntItem].col = col;
            g_aItem[nCntItem].type = type;
            g_aItem[nCntItem].bUse = true;//使用している状態にする

            //頂点座標の設定
            pVtx[0].pos = D3DXVECTOR3(-(g_aItem[nCntItem].size.x / 2), (g_aItem[nCntItem].size.y / 2), 0.0f);
            pVtx[1].pos = D3DXVECTOR3((g_aItem[nCntItem].size.x / 2), (g_aItem[nCntItem].size.y / 2), 0.0f);
            pVtx[2].pos = D3DXVECTOR3(-(g_aItem[nCntItem].size.x / 2), -(g_aItem[nCntItem].size.y / 2), 0.0f);
            pVtx[3].pos = D3DXVECTOR3((g_aItem[nCntItem].size.x / 2), -(g_aItem[nCntItem].size.y / 2), 0.0f);

            break;
        }


        pVtx += 4;  //頂点データを4つ分進める
    }
    //頂点データをアンロックする
    g_pVtxBuffItem->Unlock();
}

//=============================================================================
//アイテムの取得
//=============================================================================
ITEM*GetItem(void)
{
    return &g_aItem[0];
}

//=====================================================
// アイテムの当たり判定の設定
//=====================================================
void CollisionItem(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    int nCntItem;					// ローカル変数

    for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
    {
        // アイテムが使われたら
        if (g_aItem[nCntItem].bUse == true)
        {
            if (RectangleCollision(pPos, pPosOld, size, &g_aItem[nCntItem].pos, g_aItem[nCntItem].size) == true)
            {
                float fAngle = 0.0f;    //角度
                float fSpeedY = 0.0f;   //速さ
                float fRadius = 50.0f; //エフェクト半径
                PLAYER*pPlayer = GetPlayer();
                //ハート
                switch (g_aItem[nCntItem].type)
                {
                case ITEMTYPE_000:
                    //回復エフェクト
                    pPlayer->fLife += 125.0f;    //回復
                    ScoreCalculation(1000);     //スコア加算
                    for (int nCnt = 0; nCnt < 200; nCnt++)
                    {
                        fSpeedY = float(rand() % 800 + 300) / 100.0f;
                        //0～3.14を出す*(0 or 1)*-1で正負を決める
                        fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                        D3DXVECTOR3 pos = D3DXVECTOR3(pPlayer->pos.x - sinf(fAngle)*fRadius, pPlayer->pos.y, pPlayer->pos.z - cosf(fAngle)*fRadius);
                        D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
                        SetEffect2(pos, move, EFFECT2_SIZE_HEAL, EFFECT2_INIT_COLOR_HEAL, EFFECT2TYPE_HEAL); //回復エフェクト
                    }
                    break;

                case ITEMTYPE_001:
                    //キラキラエフェクト
                    ScoreCalculation(5000);     //スコア加算
                    for (int nCnt = 0; nCnt < 200; nCnt++)
                    {
                        fSpeedY = float(rand() % 800 + 300) / 100.0f;
                        //0～3.14を出す*(0 or 1)*-1で正負を決める
                        fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                        D3DXVECTOR3 pos = D3DXVECTOR3(pPlayer->pos.x - sinf(fAngle)*fRadius, pPlayer->pos.y, pPlayer->pos.z - cosf(fAngle)*fRadius);
                        D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
                        SetEffect2(pos, move, EFFECT2_SIZE_HEAL, EFFECT2_INIT_COLOR_HEAL, EFFECT2TYPE_RARE); //キラキラエフェクト
                    }
                    break;
                }

                g_aItem[nCntItem].bUse = false;
            }
        }
    }
}