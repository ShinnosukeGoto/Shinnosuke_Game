//=============================================================================
//
// 最強昔話決定戦の処理 [effect.cpp] (パーティクルを使ったエフェクト)
// Author:後藤慎之助
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "effect.h"
#include "boss.h"
#include "player.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;   //頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureEffect[MAX_EFFECT];  //ポリゴンのテクスチャ

EFFECT g_aEffect[MAX_EFFECT];    //エフェクトの情報

//=============================================================================
// エフェクトの初期化処理
// Author:後藤慎之助
//=============================================================================
void InitEffect(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();
    int nCntEffect = 0;

    //テクスチャの読み込み
    //炎
    g_aEffect[nCntEffect].type = EFFECTTYPE_FIRE;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle007.png",//ファイルの名前
            &g_pTextureEffect[EFFECTTYPE_FIRE]);
    }

    //爆発
    g_aEffect[nCntEffect].type = EFFECTTYPE_EXPLOSION;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle001.png",//ファイルの名前
            &g_pTextureEffect[EFFECTTYPE_EXPLOSION]);
    }

    //アイテム取得
    g_aEffect[nCntEffect].type = EFFECTTYPE_GET_ITEM;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle006.png",//ファイルの名前
            &g_pTextureEffect[EFFECTTYPE_GET_ITEM]);
    }

    //殺意の波動
    g_aEffect[nCntEffect].type = EFFECTTYPE_EVIL;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle000.png",//ファイルの名前
            &g_pTextureEffect[EFFECTTYPE_EVIL]);
    }

    //バレットの軌跡
    g_aEffect[nCntEffect].type = EFFECTTYPE_BULLET_TRAIL;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/particle000.png",//ファイルの名前
            &g_pTextureEffect[EFFECTTYPE_BULLET_TRAIL]);
    }

    //気絶
    g_aEffect[nCntEffect].type = EFFECTTYPE_BOSS_STUN;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/stun.png",//ファイルの名前
            &g_pTextureEffect[EFFECTTYPE_BOSS_STUN]);
    }

    //気絶
    g_aEffect[nCntEffect].type = EFFECTTYPE_PLAYER_STUN;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/stun.png",//ファイルの名前
            &g_pTextureEffect[EFFECTTYPE_PLAYER_STUN]);
    }

    //頂点バッファ生成
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_3D)*NUM_VERTEX*MAX_EFFECT,//バッファサイズ
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_3D,
        D3DPOOL_MANAGED,
        &g_pVtxBuffEffect,
        NULL);//頂点情報へのポインタ

    VERTEX_3D *pVtx = NULL;//頂点情報のポインタ

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

    for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
    {
        g_aEffect[nCntEffect].bUse = false;
        g_aEffect[nCntEffect].type = EFFECTTYPE_FIRE;
        g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        g_aEffect[nCntEffect].fRadius = 0.0f;
        g_aEffect[nCntEffect].nLife = 0;

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
        pVtx[0].col = g_aEffect[nCntEffect].col;
        pVtx[1].col = g_aEffect[nCntEffect].col;
        pVtx[2].col = g_aEffect[nCntEffect].col;
        pVtx[3].col = g_aEffect[nCntEffect].col;
        //テクスチャ座標の設定
        pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

        pVtx += 4;  //頂点データを4つ分進める
    }

    //頂点データをアンロックする
    g_pVtxBuffEffect->Unlock();
}

//=============================================================================
// エフェクトの終了処理
// Author:後藤慎之助
//=============================================================================
void UninitEffect(void)
{
    //テクスチャの解放
    for (int nCntTex = 0; nCntTex < EFFECTTYPE_MAX; nCntTex++)
    {
        //テクスチャの解放
        if (g_pTextureEffect[nCntTex] != NULL)
        {
            g_pTextureEffect[nCntTex]->Release();
            g_pTextureEffect[nCntTex] = NULL;
        }
    }
    //
    if (g_pVtxBuffEffect != NULL)
    {
        g_pVtxBuffEffect->Release();
        g_pVtxBuffEffect = NULL;
    }
}

//=============================================================================
// エフェクトの更新処理
// Author:後藤慎之助
//=============================================================================
void UpdateEffect(void)
{
    VERTEX_3D *pVtx = NULL;//頂点情報のポインタ

    //頂点カラー更新のため、ロック
    g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
    {
        //使われているなら
        if (g_aEffect[nCntEffect].bUse == true)
        {
            switch (g_aEffect[nCntEffect].type)
            {
                //炎
            case EFFECTTYPE_FIRE:

                g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;

                g_aEffect[nCntEffect].col.a -= FADE_SPEED_EFFECT_FIRE;
                break;

                //爆発
            case EFFECTTYPE_EXPLOSION:

                g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;

                g_aEffect[nCntEffect].move -= D3DXVECTOR3(0.0f, 0.1f, 0.0f);  //ゆっくり落とす

                g_aEffect[nCntEffect].col.a -= FADE_SPEED_EFFECT_EXPLOSION;  //ゆっくり消える

                break;

                //アイテム取得
            case EFFECTTYPE_GET_ITEM:

                g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;

                g_aEffect[nCntEffect].col.a -= FADE_SPEED_EFFECT_ITEM;
                break;

                //殺意の波動
            case EFFECTTYPE_EVIL:

                g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;

                g_aEffect[nCntEffect].col.a -= FADE_SPEED_EFFECT_EVIL;
                break;

                //バレットの軌跡
            case EFFECTTYPE_BULLET_TRAIL:

                g_aEffect[nCntEffect].fRadius -= FADE_SPEED_EFFECT_BULLET_TRAIL;

                //大きさが0になったら未使用に
                if (g_aEffect[nCntEffect].fRadius <= 0.0f)
                {
                    g_aEffect[nCntEffect].fRadius = 0.0f;
                    g_aEffect[nCntEffect].bUse = false;
                }

                break;

                //ボスのスタン
            case EFFECTTYPE_BOSS_STUN:

                g_aEffect[nCntEffect].nLife--;

                //ライフが0になったら、透明度を下げる
                if (g_aEffect[nCntEffect].nLife <= 0)
                {
                    g_aEffect[nCntEffect].nLife = 0;
                    g_aEffect[nCntEffect].col.a -= FADE_SPEED_EFFECT_STUN;
                }

                break;

                //プレイヤーのスタン
            case EFFECTTYPE_PLAYER_STUN:

                g_aEffect[nCntEffect].nLife--;

                //ライフが0になったら、透明度を下げる
                if (g_aEffect[nCntEffect].nLife <= 0)
                {
                    g_aEffect[nCntEffect].nLife = 0;
                    g_aEffect[nCntEffect].col.a -= FADE_SPEED_EFFECT_STUN;
                }

                break;
            }

            //透明度が0で未使用に
            if (g_aEffect[nCntEffect].col.a <= 0.0f)
            {
                g_aEffect[nCntEffect].col.a = 0.0f;
                g_aEffect[nCntEffect].bUse = false;
            }

            //頂点座標の設定
            pVtx[0].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fRadius / 2), (g_aEffect[nCntEffect].fRadius / 2), 0.0f);
            pVtx[1].pos = D3DXVECTOR3((g_aEffect[nCntEffect].fRadius / 2), (g_aEffect[nCntEffect].fRadius / 2), 0.0f);
            pVtx[2].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fRadius / 2), -(g_aEffect[nCntEffect].fRadius / 2), 0.0f);
            pVtx[3].pos = D3DXVECTOR3((g_aEffect[nCntEffect].fRadius / 2), -(g_aEffect[nCntEffect].fRadius / 2), 0.0f);

            //色と透明度を更新
            pVtx[0].col = g_aEffect[nCntEffect].col;
            pVtx[1].col = g_aEffect[nCntEffect].col;
            pVtx[2].col = g_aEffect[nCntEffect].col;
            pVtx[3].col = g_aEffect[nCntEffect].col;

        }
        pVtx += 4;
    }

    //頂点データをアンロックする
    g_pVtxBuffEffect->Unlock();
}

//=============================================================================
// エフェクトの描画処理
// Author:後藤慎之助
//=============================================================================
void DrawEffect(void)
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

    for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
    {
        if (g_aEffect[nCntEffect].bUse == true)
        {
            //爆発とスタン以外は加算合成
            if (g_aEffect[nCntEffect].type != EFFECTTYPE_EXPLOSION &&
                g_aEffect[nCntEffect].type != EFFECTTYPE_BOSS_STUN &&
                g_aEffect[nCntEffect].type != EFFECTTYPE_PLAYER_STUN)
            {
                //レンダーステート(加算合成にする)
                pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
                pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
                pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
            }

            //ワールドマトリクスの初期化
            D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorld);
            //回転の逆行列を生成
            pDevice->GetTransform(D3DTS_VIEW, &mtxRot);
            D3DXMatrixInverse(&g_aEffect[nCntEffect].mtxWorld, NULL, &mtxRot);
            g_aEffect[nCntEffect].mtxWorld._41 = 0;
            g_aEffect[nCntEffect].mtxWorld._42 = 0;
            g_aEffect[nCntEffect].mtxWorld._43 = 0;

            //位置を反映
            D3DXMatrixTranslation(&mtxTrans,
                g_aEffect[nCntEffect].pos.x,
                g_aEffect[nCntEffect].pos.y,
                g_aEffect[nCntEffect].pos.z);

            D3DXMatrixMultiply(
                &g_aEffect[nCntEffect].mtxWorld,
                &g_aEffect[nCntEffect].mtxWorld,
                &mtxTrans);

            //ワールドマトリックスの設定
            pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorld);

            pDevice->SetTexture(0, g_pTextureEffect[g_aEffect[nCntEffect].type]);
            //頂点バッファをデバイスのデータストリームに設定
            pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));
            //頂点フォーマットの設定
            pDevice->SetFVF(FVF_VERTEX_3D);

            //ポリゴンの描画
            pDevice->DrawPrimitive(
                D3DPT_TRIANGLESTRIP,
                nCntEffect * 4,
                NUM_POLYGON);

            //爆発以外は加算合成
            if (g_aEffect[nCntEffect].type != EFFECTTYPE_EXPLOSION &&
                g_aEffect[nCntEffect].type != EFFECTTYPE_BOSS_STUN &&
                g_aEffect[nCntEffect].type != EFFECTTYPE_PLAYER_STUN)
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
// エフェクトの設定
// Author:後藤慎之助
//=============================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fRadius, EFFECTTYPE type)
{
    VERTEX_3D *pVtx = NULL;//頂点情報のポインタ

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
    {
        if (g_aEffect[nCntEffect].bUse == false)
        {
            //初期化
            g_aEffect[nCntEffect].pos = pos;
            g_aEffect[nCntEffect].move = move;
            g_aEffect[nCntEffect].fRadius = fRadius;
            g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
            g_aEffect[nCntEffect].type = type;
            g_aEffect[nCntEffect].bUse = true;  //使用している状態にする
            g_aEffect[nCntEffect].nLife = 10;   //仮の値

            if (g_aEffect[nCntEffect].type == EFFECTTYPE_BOSS_STUN)
            {
                g_aEffect[nCntEffect].nLife = STUN_TIME;
            }
            else if (g_aEffect[nCntEffect].type == EFFECTTYPE_PLAYER_STUN)
            {
                g_aEffect[nCntEffect].nLife = PLAYER_STUN_TIME;
            }

            //頂点座標の設定
            pVtx[0].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fRadius / 2), (g_aEffect[nCntEffect].fRadius / 2), 0.0f);
            pVtx[1].pos = D3DXVECTOR3((g_aEffect[nCntEffect].fRadius / 2), (g_aEffect[nCntEffect].fRadius / 2), 0.0f);
            pVtx[2].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fRadius / 2), -(g_aEffect[nCntEffect].fRadius / 2), 0.0f);
            pVtx[3].pos = D3DXVECTOR3((g_aEffect[nCntEffect].fRadius / 2), -(g_aEffect[nCntEffect].fRadius / 2), 0.0f);

            break;
        }

        pVtx += 4;  //頂点データを4つ分進める
    }
    //頂点データをアンロックする
    g_pVtxBuffEffect->Unlock();
}