//=============================================================================
//
// PLASTIC TROOPERS [effect.cpp]の処理 (文字などのエフェクト)
// Author:後藤慎之助
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "effect.h"
#include "collision.h"
#include "player.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureEffect[EFFECTTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;//頂点バッファへのポインタ
EFFECT g_Effect[MAX_EFFECT];//アイテムの情報

//=============================================================================
//エフェクトの初期化処理
//=============================================================================
void InitEffect(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得

    //頂点バッファの生成
    pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_EFFECT,
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_3D,		//頂点フォーマット
        D3DPOOL_MANAGED,//（固定）
        &g_pVtxBuffEffect,//変数名が変わると変更が必要
        NULL);

    //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/Lock-On.png", &g_pTextureEffect[0]);  //ターゲット
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/lost.png", &g_pTextureEffect[1]);     //エナジー切れ
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/unlock.png", &g_pTextureEffect[2]);   //ロック解除
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/effect003.png", &g_pTextureEffect[3]);  //土煙

    VERTEX_3D *pVtx;//頂点情報のポインタ

                    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
    {
        g_Effect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Effect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Effect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        g_Effect[nCntEffect].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Effect[nCntEffect].type = EFFECTTYPE_000;
        g_Effect[nCntEffect].nCounterAnim = 0;
        g_Effect[nCntEffect].nPatternAnim = 0;
        g_Effect[nCntEffect].bUse = false;

        //頂点座標の設定（右回りで設定する）
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
        pVtx[0].col = g_Effect[nCntEffect].col;
        pVtx[1].col = g_Effect[nCntEffect].col;
        pVtx[2].col = g_Effect[nCntEffect].col;
        pVtx[3].col = g_Effect[nCntEffect].col;

        pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

        pVtx += 4;
    }
    //頂点データアンロック
    g_pVtxBuffEffect->Unlock();

}

//=============================================================================
// エフェクトの終了処理
//=============================================================================
void UninitEffect(void)
{
    //テクスチャの開放
    for (int nCntEffect = 0; nCntEffect < EFFECTTYPE_MAX; nCntEffect++)
    {
        if (g_pTextureEffect[nCntEffect] != NULL)
        {
            g_pTextureEffect[nCntEffect]->Release();
            g_pTextureEffect[nCntEffect] = NULL;
        }
    }

    //頂点バッファの開放
    if (g_pVtxBuffEffect != NULL)
    {
        g_pVtxBuffEffect->Release();
        g_pVtxBuffEffect = NULL;
    }
}

//=============================================================================
//エフェクトの更新処理
//=============================================================================
void UpdateEffect(void)
{
    VERTEX_3D *pVtx;	//頂点情報へのポインタ

    //頂点バッファをロック
    g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
    {
        if (g_Effect[nCntEffect].bUse == true)
        {
            PLAYER*pPlayer = GetPlayer();

            switch (g_Effect[nCntEffect].type)
            {
                //ターゲット
            case EFFECTTYPE_000:

                //エフェクトのカウンターを進める
                g_Effect[nCntEffect].nCounterAnim++;
                if (g_Effect[nCntEffect].nCounterAnim == 10)
                {
                    g_Effect[nCntEffect].nCounterAnim = 0;
                    g_Effect[nCntEffect].bUse = false;
                }

                ////テクスチャ座標を更新
                //pVtx[0].tex = D3DXVECTOR2(g_Effect[nCntEffect].nPatternAnim*0.2f, 0.0f);
                //pVtx[1].tex = D3DXVECTOR2(g_Effect[nCntEffect].nPatternAnim*0.2f + 0.2f, 0.0f);
                //pVtx[2].tex = D3DXVECTOR2(g_Effect[nCntEffect].nPatternAnim*0.2f, 1.0f);
                //pVtx[3].tex = D3DXVECTOR2(g_Effect[nCntEffect].nPatternAnim*0.2f + 0.2f, 1.0f);

                ////パターンが最後まで終わった
                //if (g_Effect[nCntEffect].nPatternAnim > 5)
                //{
                //    g_Effect[nCntEffect].bUse = false;
                //}

                break;

                //エナジー切れ
            case EFFECTTYPE_001:

                //エフェクトのカウンターを進める
                g_Effect[nCntEffect].nCounterAnim++;
                if (g_Effect[nCntEffect].nCounterAnim == START_HEAL_OUT_ENERGY)
                {
                    g_Effect[nCntEffect].nCounterAnim = 0;
                    g_Effect[nCntEffect].bUse = false;
                }

                //プレイヤーが生存しているならプレイヤーの上に表示
                if (pPlayer->bDisp == true)
                {
                    g_Effect[nCntEffect].pos = D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y + PLAYER_HEIGHT * 2, pPlayer->pos.z);
                }
                else
                {
                    g_Effect[nCntEffect].bUse = false;
                }

                break;

                //ロック解除
            case EFFECTTYPE_002:

                //移動量と位置を結びつける
                g_Effect[nCntEffect].pos += g_Effect[nCntEffect].move;

                //エフェクトのカウンターを進める
                g_Effect[nCntEffect].col.a -= 0.01f;
                if (g_Effect[nCntEffect].col.a < 0.0f)
                {
                    g_Effect[nCntEffect].bUse = false;
                }

                break;

                //着地の煙
            case EFFECTTYPE_003:

                //エフェクト2のカウンターを進めて、パターンを切り替える
                g_Effect[nCntEffect].nCounterAnim++;
                if (g_Effect[nCntEffect].nCounterAnim == 2)
                {
                    g_Effect[nCntEffect].nCounterAnim = 0;
                    g_Effect[nCntEffect].nPatternAnim++;
                }

                //テクスチャ座標を更新
                pVtx[0].tex = D3DXVECTOR2(g_Effect[nCntEffect].nPatternAnim*0.2f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(g_Effect[nCntEffect].nPatternAnim*0.2f + 0.2f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(g_Effect[nCntEffect].nPatternAnim*0.2f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(g_Effect[nCntEffect].nPatternAnim*0.2f + 0.2f, 1.0f);

                //パターンが最後まで終わった
                if (g_Effect[nCntEffect].nPatternAnim > 4)
                {
                    g_Effect[nCntEffect].bUse = false;
                }

                break;

                //ヒット
            case EFFECTTYPE_004:

                break;
            }

            //色と透明度を更新
            pVtx[0].col = g_Effect[nCntEffect].col;
            pVtx[1].col = g_Effect[nCntEffect].col;
            pVtx[2].col = g_Effect[nCntEffect].col;
            pVtx[3].col = g_Effect[nCntEffect].col;

        }
        pVtx += 4;
    }

    //頂点データをアンロックする
    g_pVtxBuffEffect->Unlock();
}

//=============================================================================
//エフェクトの描画処理
//=============================================================================
void DrawEffect(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタを取得
    D3DXMATRIX mtxRot, mtxTrans;//行列計算用のマトリクス

    //アルファテストを有効化
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    //アルファテスト基準値の設定
    pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
    //アルファテストの比較方法の設定（GREATERは基準値より大きい場合)
    pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    //ライトの影響を受けなくする
    DWORD ambient;
    pDevice->GetRenderState(D3DRS_AMBIENT, &ambient);
    pDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
    pDevice->LightEnable(0, FALSE);

    for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
    {
        if (g_Effect[nCntEffect].bUse == true)
        {
            //ワールドマトリクスの初期化
            D3DXMatrixIdentity(&g_Effect[nCntEffect].mtxWorld);
            //回転の逆行列を生成
            pDevice->GetTransform(D3DTS_VIEW, &mtxRot);
            D3DXMatrixInverse(&g_Effect[nCntEffect].mtxWorld, NULL, &mtxRot);
            g_Effect[nCntEffect].mtxWorld._41 = 0;//移動はさせたくないので0を代入
            g_Effect[nCntEffect].mtxWorld._42 = 0;//移動はさせたくないので0を代入
            g_Effect[nCntEffect].mtxWorld._43 = 0;//移動はさせたくないので0を代入

            //位置を反映
            D3DXMatrixTranslation(&mtxTrans, g_Effect[nCntEffect].pos.x, g_Effect[nCntEffect].pos.y, g_Effect[nCntEffect].pos.z);
            D3DXMatrixMultiply(&g_Effect[nCntEffect].mtxWorld, &g_Effect[nCntEffect].mtxWorld, &mtxTrans);
            //ワールドマトリックスの設定
            pDevice->SetTransform(D3DTS_WORLD, &g_Effect[nCntEffect].mtxWorld);
            //テクスチャの設定
            pDevice->SetTexture(0, g_pTextureEffect[g_Effect[nCntEffect].type]);
            //頂点バッファをデバイスのデータストリームに設定
            pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));
            //頂点フォーマットの設定
            pDevice->SetFVF(FVF_VERTEX_3D);
            //ポリゴンの描画
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, NUM_POLYGON);

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
//エフェクトの設定
//=============================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move,EFFECTTYPE type)
{
    VERTEX_3D *pVtx;	//頂点情報へのポインタ

                        //頂点バッファをロック
    g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
    {
        if (g_Effect[nCntEffect].bUse == false)
        {
            //アニメーションパターンの初期化
            g_Effect[nCntEffect].nCounterAnim = 0;
            g_Effect[nCntEffect].nPatternAnim = 0;

            g_Effect[nCntEffect].pos = pos;
            g_Effect[nCntEffect].size = size;
            g_Effect[nCntEffect].move = move;
            g_Effect[nCntEffect].type = type;

            g_Effect[nCntEffect].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

            g_Effect[nCntEffect].bUse = true;

            //頂点座標の設定
            pVtx[0].pos = D3DXVECTOR3(-(g_Effect[nCntEffect].size.x / 2), (g_Effect[nCntEffect].size.y / 2), 0.0f);
            pVtx[1].pos = D3DXVECTOR3((g_Effect[nCntEffect].size.x / 2), (g_Effect[nCntEffect].size.y / 2), 0.0f);
            pVtx[2].pos = D3DXVECTOR3(-(g_Effect[nCntEffect].size.x / 2), -(g_Effect[nCntEffect].size.y / 2), 0.0f);
            pVtx[3].pos = D3DXVECTOR3((g_Effect[nCntEffect].size.x / 2), -(g_Effect[nCntEffect].size.y / 2), 0.0f);

            break;
        }
        pVtx += 4;
    }

    //頂点データをアンロックする
    g_pVtxBuffEffect->Unlock();
}

//=============================================================================
//エフェクトの取得
//=============================================================================
EFFECT*GetEffect(void)
{
    return &g_Effect[0];
}