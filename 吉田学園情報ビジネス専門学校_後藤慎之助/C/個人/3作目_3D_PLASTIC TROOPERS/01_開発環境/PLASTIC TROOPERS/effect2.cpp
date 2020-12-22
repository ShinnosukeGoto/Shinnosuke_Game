//=============================================================================
//
// PLASTIC TROOPERS [effect2.cpp]の処理 (パーティクルを使ったエフェクト)
// Author:後藤慎之助
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "player.h"
#include "effect2.h"
#include "camera.h"
#include "collision.h"
#include "enemy.h"
#include "boss.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect2 = NULL;   //頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureEffect2[MAX_EFFECT2];  //ポリゴンのテクスチャ

EFFECT2 g_aEffect2[MAX_EFFECT2];    //エフェクトの情報

//=============================================================================
//エフェクト2の初期化処理
//=============================================================================
void InitEffect2(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();
    int nCntEffect2 = 0;

    //テクスチャの読み込み
    //ヒット
    g_aEffect2[nCntEffect2].type = EFFECT2TYPE_HIT;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle000.png",//ファイルの名前
            &g_pTextureEffect2[EFFECT2TYPE_HIT]);
    }

    //爆発
    g_aEffect2[nCntEffect2].type = EFFECT2TYPE_EXPLOSION;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle001.png",//ファイルの名前
            &g_pTextureEffect2[EFFECT2TYPE_EXPLOSION]);
    }

    //土煙
    g_aEffect2[nCntEffect2].type = EFFECT2TYPE_RUN;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle002.png",//ファイルの名前
            &g_pTextureEffect2[EFFECT2TYPE_RUN]);
    }

    //回復
    g_aEffect2[nCntEffect2].type = EFFECT2TYPE_HEAL;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle003.png",//ファイルの名前
            &g_pTextureEffect2[EFFECT2TYPE_HEAL]);
    }

    //間欠泉
    g_aEffect2[nCntEffect2].type = EFFECT2TYPE_GEYSER;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle004.png",//ファイルの名前
            &g_pTextureEffect2[EFFECT2TYPE_GEYSER]);
    }

    //ストームソード白
    g_aEffect2[nCntEffect2].type = EFFECT2TYPE_STORM;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle005.png",//ファイルの名前
            &g_pTextureEffect2[EFFECT2TYPE_STORM]);
    }

    //ストームソード黄
    g_aEffect2[nCntEffect2].type = EFFECT2TYPE_STORM2;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle006.png",//ファイルの名前
            &g_pTextureEffect2[EFFECT2TYPE_STORM2]);
    }

    //炎
    g_aEffect2[nCntEffect2].type = EFFECT2TYPE_FIRE;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle007.png",//ファイルの名前
            &g_pTextureEffect2[EFFECT2TYPE_FIRE]);
    }

    //キラキラ
    g_aEffect2[nCntEffect2].type = EFFECT2TYPE_RARE;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle006.png",//ファイルの名前
            &g_pTextureEffect2[EFFECT2TYPE_RARE]);
    }

    //炎
    g_aEffect2[nCntEffect2].type = EFFECT2TYPE_FIRE2;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle007.png",//ファイルの名前
            &g_pTextureEffect2[EFFECT2TYPE_FIRE2]);
    }

    //頂点バッファ生成
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_3D)*NUM_VERTEX*MAX_EFFECT2,//バッファサイズ
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_3D,
        D3DPOOL_MANAGED,
        &g_pVtxBuffEffect2,
        NULL);//頂点情報へのポインタ

    VERTEX_3D *pVtx;//頂点情報のポインタ

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffEffect2->Lock(0, 0, (void**)&pVtx, 0);

    for (nCntEffect2 = 0; nCntEffect2 < MAX_EFFECT2; nCntEffect2++)
    {
        g_aEffect2[nCntEffect2].bUse = false;

        g_aEffect2[nCntEffect2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aEffect2[nCntEffect2].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aEffect2[nCntEffect2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        g_aEffect2[nCntEffect2].fRadius = 0.0f;

        //頂点座標の設定
        pVtx[0].pos = D3DXVECTOR3(-(EFFECT2_SIZE_HIT / 2), (EFFECT2_SIZE_HIT / 2), 0.0f);
        pVtx[1].pos = D3DXVECTOR3((EFFECT2_SIZE_HIT / 2), (EFFECT2_SIZE_HIT / 2), 0.0f);
        pVtx[2].pos = D3DXVECTOR3(-(EFFECT2_SIZE_HIT / 2), -(EFFECT2_SIZE_HIT / 2), 0.0f);
        pVtx[3].pos = D3DXVECTOR3((EFFECT2_SIZE_HIT / 2), -(EFFECT2_SIZE_HIT / 2), 0.0f);
        //法線の設定
        pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        //頂点カラーの設定
        pVtx[0].col = g_aEffect2[nCntEffect2].col;
        pVtx[1].col = g_aEffect2[nCntEffect2].col;
        pVtx[2].col = g_aEffect2[nCntEffect2].col;
        pVtx[3].col = g_aEffect2[nCntEffect2].col;
        //テクスチャ座標の設定
        pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

        pVtx += 4;  //頂点データを4つ分進める
    }

    //頂点データをアンロックする
    g_pVtxBuffEffect2->Unlock();
}

//=============================================================================
// エフェクト2の終了処理
//=============================================================================
void UninitEffect2(void)
{
    //テクスチャの解放
    for (int nCntTex = 0; nCntTex < EFFECT2TYPE_MAX; nCntTex++)
    {
        //テクスチャの解放
        if (g_pTextureEffect2[nCntTex] != NULL)
        {
            g_pTextureEffect2[nCntTex]->Release();
            g_pTextureEffect2[nCntTex] = NULL;
        }
    }
    //
    if (g_pVtxBuffEffect2 != NULL)
    {
        g_pVtxBuffEffect2->Release();
        g_pVtxBuffEffect2 = NULL;
    }
}

//=============================================================================
//エフェクト2の更新処理
//=============================================================================
void UpdateEffect2(void)
{
    VERTEX_3D *pVtx;//頂点情報のポインタ

    //頂点カラーの更新
    g_pVtxBuffEffect2->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntEffect2 = 0; nCntEffect2 < MAX_EFFECT2; nCntEffect2++)
    {
        //使われているなら
        if (g_aEffect2[nCntEffect2].bUse == true)
        {
            switch (g_aEffect2[nCntEffect2].type)
            {
                //ヒット
            case EFFECT2TYPE_HIT:

                g_aEffect2[nCntEffect2].pos += g_aEffect2[nCntEffect2].move;

                g_aEffect2[nCntEffect2].col.a -= 0.05f;
                break;

                //爆発
            case EFFECT2TYPE_EXPLOSION:
                g_aEffect2[nCntEffect2].pos += g_aEffect2[nCntEffect2].move;

                g_aEffect2[nCntEffect2].move -= D3DXVECTOR3(0.0f, 0.1f, 0.0f);  //ゆっくり落とす

                g_aEffect2[nCntEffect2].col.a -= 0.005f;  //ゆっくり消える

                break;

                //土煙
            case EFFECT2TYPE_RUN:

                g_aEffect2[nCntEffect2].pos.y += ((float)(rand() % 5) + 1);
                g_aEffect2[nCntEffect2].col.a -= 0.1f;

                //回復
            case EFFECT2TYPE_HEAL:

                g_aEffect2[nCntEffect2].pos += g_aEffect2[nCntEffect2].move;

                g_aEffect2[nCntEffect2].col.a -= 0.07f;
                break;

                //間欠泉
            case EFFECT2TYPE_GEYSER:

                g_aEffect2[nCntEffect2].pos += g_aEffect2[nCntEffect2].move;

                g_aEffect2[nCntEffect2].move -= D3DXVECTOR3(0.0f, 0.1f, 0.0f);

                g_aEffect2[nCntEffect2].col.a -= 0.008f;
                break;

                //ストームソード白
            case EFFECT2TYPE_STORM:

                g_aEffect2[nCntEffect2].pos += g_aEffect2[nCntEffect2].move;

                g_aEffect2[nCntEffect2].col.a -= 0.008f;
                break;

                //ストームソード黄
            case EFFECT2TYPE_STORM2:

                g_aEffect2[nCntEffect2].pos += g_aEffect2[nCntEffect2].move;

                g_aEffect2[nCntEffect2].col.a -= 0.008f;

                break;

                //炎
            case EFFECT2TYPE_FIRE:

                g_aEffect2[nCntEffect2].pos += g_aEffect2[nCntEffect2].move;

                g_aEffect2[nCntEffect2].col.a -= 0.015f;
                break;

                //キラキラ
            case EFFECT2TYPE_RARE:

                g_aEffect2[nCntEffect2].pos += g_aEffect2[nCntEffect2].move;

                g_aEffect2[nCntEffect2].col.a -= 0.009f;
                break;

                //炎2
            case EFFECT2TYPE_FIRE2:

                g_aEffect2[nCntEffect2].pos += g_aEffect2[nCntEffect2].move;

                g_aEffect2[nCntEffect2].col.a -= 0.01f;
                break;
            }

            //透明度が0で未使用に
            if (g_aEffect2[nCntEffect2].col.a <= 0)
            {
                g_aEffect2[nCntEffect2].bUse = false;
            }

            //色と透明度を更新
            pVtx[0].col = g_aEffect2[nCntEffect2].col;
            pVtx[1].col = g_aEffect2[nCntEffect2].col;
            pVtx[2].col = g_aEffect2[nCntEffect2].col;
            pVtx[3].col = g_aEffect2[nCntEffect2].col;

        }
        pVtx += 4;
    }

    //頂点データをアンロックする
    g_pVtxBuffEffect2->Unlock();
}

//=============================================================================
//エフェクト2の描画処理
//=============================================================================
void DrawEffect2(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();
    D3DXMATRIX mtxRot, mtxTrans;//行列計算用のマトリクス

    //アルファテストを有効化
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    //アルファテスト基準値の設定
    pDevice->SetRenderState(D3DRS_ALPHAREF, 50);
    //アルファテストの比較方法の設定
    pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    //ライトの影響を受けなくする
    DWORD ambient;
    pDevice->GetRenderState(D3DRS_AMBIENT, &ambient);
    pDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
    pDevice->LightEnable(0, FALSE);

    for (int nCntEffect2 = 0; nCntEffect2 < MAX_EFFECT2; nCntEffect2++)
    {
        if (g_aEffect2[nCntEffect2].bUse == true)
        {
            //爆発と土煙以外は加算合成
            if (g_aEffect2[nCntEffect2].type != EFFECT2TYPE_EXPLOSION)
            {
                //レンダーステート(加算合成にする)
                pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
                pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
                pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
            }

            //ワールドマトリクスの初期化
            D3DXMatrixIdentity(&g_aEffect2[nCntEffect2].mtxWorld);
            //回転の逆行列を生成
            pDevice->GetTransform(D3DTS_VIEW, &mtxRot);
            D3DXMatrixInverse(&g_aEffect2[nCntEffect2].mtxWorld, NULL, &mtxRot);
            g_aEffect2[nCntEffect2].mtxWorld._41 = 0;
            g_aEffect2[nCntEffect2].mtxWorld._42 = 0;
            g_aEffect2[nCntEffect2].mtxWorld._43 = 0;

            ////位置を反映
            D3DXMatrixTranslation(&mtxTrans,
                g_aEffect2[nCntEffect2].pos.x,
                g_aEffect2[nCntEffect2].pos.y,
                g_aEffect2[nCntEffect2].pos.z);

            D3DXMatrixMultiply(
                &g_aEffect2[nCntEffect2].mtxWorld,
                &g_aEffect2[nCntEffect2].mtxWorld,
                &mtxTrans);

            //ワールドマトリックスの設定
            pDevice->SetTransform(D3DTS_WORLD, &g_aEffect2[nCntEffect2].mtxWorld);

            pDevice->SetTexture(0, g_pTextureEffect2[g_aEffect2[nCntEffect2].type]);
            //頂点バッファをデバイスのデータストリームに設定
            pDevice->SetStreamSource(0, g_pVtxBuffEffect2, 0, sizeof(VERTEX_3D));
            //頂点フォーマットの設定
            pDevice->SetFVF(FVF_VERTEX_3D);

            //ポリゴンの描画
            pDevice->DrawPrimitive(
                D3DPT_TRIANGLESTRIP,
                nCntEffect2 * 4,
                NUM_POLYGON);

            //爆発以外は加算合成
            if (g_aEffect2[nCntEffect2].type != EFFECT2TYPE_EXPLOSION)
            {
                //レンダーステート(加算合成を戻す)
                pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
                pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
                pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            }
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
//エフェクト2の設定
//=============================================================================
void SetEffect2(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fRadius, D3DXCOLOR col, EFFECT2TYPE type)
{
    VERTEX_3D *pVtx;//頂点情報のポインタ
    PLAYER *pPlayer;
    pPlayer = GetPlayer();

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffEffect2->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntEffect2 = 0; nCntEffect2 < MAX_EFFECT2; nCntEffect2++)
    {

        //視野に入っているなら
        if (pPlayer->pos.x > pos.x - CAMERA_VIEW_RANGE&&
            pPlayer->pos.x < pos.x + CAMERA_VIEW_RANGE&&
            pPlayer->pos.z > pos.z - CAMERA_VIEW_RANGE&&
            pPlayer->pos.z < pos.z + CAMERA_VIEW_RANGE&&
            g_aEffect2[nCntEffect2].bUse == false)
        {
            //初期化
            g_aEffect2[nCntEffect2].pos = pos;
            g_aEffect2[nCntEffect2].move = move;
            g_aEffect2[nCntEffect2].fRadius = fRadius;
            g_aEffect2[nCntEffect2].col = col;
            g_aEffect2[nCntEffect2].type = type;
            g_aEffect2[nCntEffect2].bUse = true;//使用している状態にする

            //頂点座標の設定
            pVtx[0].pos = D3DXVECTOR3(-(g_aEffect2[nCntEffect2].fRadius / 2), (g_aEffect2[nCntEffect2].fRadius / 2), 0.0f);
            pVtx[1].pos = D3DXVECTOR3((g_aEffect2[nCntEffect2].fRadius / 2), (g_aEffect2[nCntEffect2].fRadius / 2), 0.0f);
            pVtx[2].pos = D3DXVECTOR3(-(g_aEffect2[nCntEffect2].fRadius / 2), -(g_aEffect2[nCntEffect2].fRadius / 2), 0.0f);
            pVtx[3].pos = D3DXVECTOR3((g_aEffect2[nCntEffect2].fRadius / 2), -(g_aEffect2[nCntEffect2].fRadius / 2), 0.0f);

            break;
        }


        pVtx += 4;  //頂点データを4つ分進める
    }
    //頂点データをアンロックする
    g_pVtxBuffEffect2->Unlock();
}

//=====================================================
// 自機とエフェクトとの当たり判定の設定
//=====================================================
void CollisionEffectPlayer(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    int nCntEffect2;					// ローカル変数

    for (nCntEffect2 = 0; nCntEffect2 < MAX_EFFECT2; nCntEffect2++)
    {
        //エフェクトが使われたら
        if (g_aEffect2[nCntEffect2].bUse == true)
        {
            //間欠泉
            if (g_aEffect2[nCntEffect2].type == EFFECT2TYPE_GEYSER)
            {
                //当たっているなら
                if (RectangleCollision(pPos, pPosOld, size, &g_aEffect2[nCntEffect2].pos, D3DXVECTOR3(g_aEffect2[nCntEffect2].fRadius, g_aEffect2[nCntEffect2].fRadius, g_aEffect2[nCntEffect2].fRadius)) == true)
                {

                    PLAYER*pPlayer = GetPlayer();

                    if (pPlayer->bChanceAttack == false)    //必殺攻撃中以外
                    {
                        pPlayer->move.y += 0.12f;    //上昇
                    }
                }
            }

            //ボスの火柱
            else if (g_aEffect2[nCntEffect2].type == EFFECT2TYPE_FIRE2)
            {
                //当たっているなら
                if (RectangleCollision(pPos, pPosOld, size, &g_aEffect2[nCntEffect2].pos, D3DXVECTOR3(g_aEffect2[nCntEffect2].fRadius, g_aEffect2[nCntEffect2].fRadius, g_aEffect2[nCntEffect2].fRadius)) == true)
                {
                    PLAYER*pPlayer = GetPlayer();

                    //自機にダメージ
                    pPlayer->fLife -= 0.25f;
                }
            }
        }
    }
}

//=====================================================
// 敵とエフェクトとの当たり判定の設定
//=====================================================
void CollisionEffectEnemy(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size, int nCnt)
{
    int nCntEffect2;					// ローカル変数

    for (nCntEffect2 = 0; nCntEffect2 < MAX_EFFECT2; nCntEffect2++)
    {
        //エフェクトが使われたら
        if (g_aEffect2[nCntEffect2].bUse == true)
        {
            //炎
            if (g_aEffect2[nCntEffect2].type == EFFECT2TYPE_FIRE)
            {
                //当たっているなら
                if (RectangleCollision(pPos, pPosOld, size, &g_aEffect2[nCntEffect2].pos, D3DXVECTOR3(g_aEffect2[nCntEffect2].fRadius, g_aEffect2[nCntEffect2].fRadius, g_aEffect2[nCntEffect2].fRadius)) == true)
                {
                    HitEnemy(nCnt, 5, 0.2f);
                }
            }
        }
    }
}

//=====================================================
// ボスとエフェクトとの当たり判定の設定
//=====================================================
void CollisionEffectBoss(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    int nCntEffect2;					// ローカル変数

    for (nCntEffect2 = 0; nCntEffect2 < MAX_EFFECT2; nCntEffect2++)
    {
        //エフェクトが使われたら
        if (g_aEffect2[nCntEffect2].bUse == true)
        {
            //炎
            if (g_aEffect2[nCntEffect2].type == EFFECT2TYPE_FIRE)
            {
                //当たっているなら
                if (RectangleCollision(pPos, pPosOld, size, &g_aEffect2[nCntEffect2].pos, D3DXVECTOR3(g_aEffect2[nCntEffect2].fRadius, g_aEffect2[nCntEffect2].fRadius, g_aEffect2[nCntEffect2].fRadius)) == true)
                {
                    HitBoss(1, 0.025f);
                }
            }
        }
    }
}