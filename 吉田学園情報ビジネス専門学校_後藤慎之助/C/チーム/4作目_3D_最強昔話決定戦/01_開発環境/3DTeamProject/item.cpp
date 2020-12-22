//=============================================================================
//
// 最強昔話決定戦 [item.cpp]の処理 (アイテム)
// Author:後藤慎之助
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "item.h"
#include "collision.h"
#include "player.h"
#include "bomb.h"
#include "fire.h"
#include "effect.h"
#include "stage.h"
#include "sound.h"
#include "score.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;   //頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureItem[ITEMTYPE_MAX];  //ポリゴンのテクスチャ

ITEM g_aItem[MAX_ITEM];    //エフェクトの情報

//=============================================================================
// アイテムの初期化処理
// Author:後藤慎之助
//=============================================================================
void InitItem(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();
    int nCntItem = 0;

    //テクスチャの読み込み
    //炎
    g_aItem[nCntItem].type = ITEMTYPE_000;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/item000.png",//ファイルの名前
            &g_pTextureItem[ITEMTYPE_000]);
    }

    //ボム
    g_aItem[nCntItem].type = ITEMTYPE_001;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/item001.png",//ファイルの名前
            &g_pTextureItem[ITEMTYPE_001]);
    }

    //スピード
    g_aItem[nCntItem].type = ITEMTYPE_002;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/item002.png",//ファイルの名前
            &g_pTextureItem[ITEMTYPE_002]);
    }

    //キック
    g_aItem[nCntItem].type = ITEMTYPE_003;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/item003.png",//ファイルの名前
            &g_pTextureItem[ITEMTYPE_003]);
    }

    //ハート
    g_aItem[nCntItem].type = ITEMTYPE_004;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/item004.png",//ファイルの名前
            &g_pTextureItem[ITEMTYPE_004]);
    }

    //タイム
    g_aItem[nCntItem].type = ITEMTYPE_005;
    {
        D3DXCreateTextureFromFile(pDevice,
            "TEXTURE/item005.png",//ファイルの名前
            &g_pTextureItem[ITEMTYPE_005]);
    }

    //頂点バッファ生成
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_3D)*NUM_VERTEX*MAX_ITEM,//バッファサイズ
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_3D,
        D3DPOOL_MANAGED,
        &g_pVtxBuffItem,
        NULL);//頂点情報へのポインタ

    VERTEX_3D *pVtx = NULL;//頂点情報のポインタ

                    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

    for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
    {
        g_aItem[nCntItem].bUse = false;
        g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aItem[nCntItem].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aItem[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aItem[nCntItem].nCntTime = 0;
        g_aItem[nCntItem].nCntInvincible = ITEM_INVINCIBLE_TIME;
        g_aItem[nCntItem].nCntVisible = ITEM_VISIBLE_TIME;
        g_aItem[nCntItem].type = ITEMTYPE_000;
        g_aItem[nCntItem].bSet = false;

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
// Author:後藤慎之助
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
// アイテムの更新処理
// Author:後藤慎之助
//=============================================================================
void UpdateItem(void)
{
    for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
    {
        //セット中なら
        if (g_aItem[nCntItem].bSet == true)
        {
            g_aItem[nCntItem].nCntTime++;   //カウンタ加算

            //一定Fで、セットする
            if (g_aItem[nCntItem].nCntTime >= ITEM_SET_TIME)
            {
                //使用中に
                g_aItem[nCntItem].bUse = true;

                //初期化
                g_aItem[nCntItem].nCntTime = 0;
                g_aItem[nCntItem].bSet = false;  
            }
        }

        //使用しているなら
        if (g_aItem[nCntItem].bUse == true)
        {
            //炎への無敵時間
            g_aItem[nCntItem].nCntInvincible--;

            //アイテムをゲットできるまでの時間を数える
            g_aItem[nCntItem].nCntVisible--;

            //炎への無敵時間がないなら炎に当たる
            if (g_aItem[nCntItem].nCntInvincible <= 0)
            {
                g_aItem[nCntItem].nCntInvincible = 0;

                //炎に当たったら
                if (HitFire(&g_aItem[nCntItem].pos, &g_aItem[nCntItem].pos,
                    D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT * 2, GRID_SIZE)) == true)
                {
                    //未使用に
                    g_aItem[nCntItem].bUse = false;

                    //アイテム爆発エフェクト
                    float fSpeedX = 0.0f;   //速さX
                    float fSpeedY = 0.0f;   //速さY
                    float fSpeedZ = 0.0f;   //速さZ
                    int nPlusMinusX = 0;    //符号X
                    int nPlusMinusZ = 0;    //符号Z
                    for (int nCnt = 0; nCnt < COUNT_EFFECT_EXPLODE_ITEM; nCnt++)
                    {
                        fSpeedX = float(rand() % 200) / 100.0f;
                        fSpeedY = float(rand() % 800 + 300) / 100.0f;
                        fSpeedZ = float(rand() % 200) / 100.0f;
                        nPlusMinusX = int(rand() % 2 + 1);
                        nPlusMinusZ = int(rand() % 2 + 1);
                        if (nPlusMinusX == 1)
                        {
                            nPlusMinusX = 1;
                        }
                        else
                        {
                            nPlusMinusX = -1;
                        }

                        if (nPlusMinusZ == 1)
                        {
                            nPlusMinusZ = 1;
                        }
                        else
                        {
                            nPlusMinusZ = -1;
                        }

                        D3DXVECTOR3 move = D3DXVECTOR3(fSpeedX*nPlusMinusX, fSpeedY, fSpeedZ*nPlusMinusZ);
                        SetEffect(g_aItem[nCntItem].pos, move, EFFECT_SIZE_EXPLODE_ITEM, EFFECTTYPE_EXPLOSION);
                    }

                    //グリッド情報を更新
                    SetGridInfo(g_aItem[nCntItem].pos, false);

                    //初期化
                    g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                }
            }
            
            //アイテムをゲットできるようにする
            if (g_aItem[nCntItem].nCntVisible < 0)
            {
                g_aItem[nCntItem].nCntVisible = 0;
            }

            //ボムに当たったら
            if (HitBomb(&g_aItem[nCntItem].pos, &g_aItem[nCntItem].pos,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID) , MAX_BOMB + 1) == true)
            {
                //未使用に
                g_aItem[nCntItem].bUse = false;

                //アイテム爆発エフェクト
                float fSpeedX = 0.0f;   //速さX
                float fSpeedY = 0.0f;   //速さY
                float fSpeedZ = 0.0f;   //速さZ
                int nPlusMinusX = 0;    //符号X
                int nPlusMinusZ = 0;    //符号Z
                for (int nCnt = 0; nCnt < COUNT_EFFECT_EXPLODE_ITEM; nCnt++)
                {
                    fSpeedX = float(rand() % 200) / 100.0f;
                    fSpeedY = float(rand() % 800 + 300) / 100.0f;
                    fSpeedZ = float(rand() % 200) / 100.0f;
                    nPlusMinusX = int(rand() % 2 + 1);
                    nPlusMinusZ = int(rand() % 2 + 1);
                    if (nPlusMinusX == 1)
                    {
                        nPlusMinusX = 1;
                    }
                    else
                    {
                        nPlusMinusX = -1;
                    }

                    if (nPlusMinusZ == 1)
                    {
                        nPlusMinusZ = 1;
                    }
                    else
                    {
                        nPlusMinusZ = -1;
                    }

                    D3DXVECTOR3 move = D3DXVECTOR3(fSpeedX*nPlusMinusX, fSpeedY, fSpeedZ*nPlusMinusZ);
                    SetEffect(g_aItem[nCntItem].pos, move, EFFECT_SIZE_EXPLODE_ITEM, EFFECTTYPE_EXPLOSION);
                }

                //グリッド情報を更新
                SetGridInfo(D3DXVECTOR3(g_aItem[nCntItem].pos.x, 0.0f, g_aItem[nCntItem].pos.z), false);

                //初期化
                g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }
        }
    }
}

//=============================================================================
// アイテムの描画処理
// Author:後藤慎之助
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
// アイテムの設定
// Author:後藤慎之助
//=============================================================================
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move, ITEMTYPE type)
{
    VERTEX_3D *pVtx = NULL;//頂点情報のポインタ

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
    {
        if (g_aItem[nCntItem].bSet == false && g_aItem[nCntItem].bUse == false)
        {
            //初期化
            g_aItem[nCntItem].pos = pos;
            g_aItem[nCntItem].size = ITEM_SIZE;
            g_aItem[nCntItem].move = move;
            g_aItem[nCntItem].type = type;
            g_aItem[nCntItem].nCntInvincible = ITEM_INVINCIBLE_TIME;
            g_aItem[nCntItem].nCntVisible = ITEM_VISIBLE_TIME;
            g_aItem[nCntItem].bSet = true;

            //グリッド情報を設定
            SetGridInfo(D3DXVECTOR3(g_aItem[nCntItem].pos.x, 0.0f , g_aItem[nCntItem].pos.z), true);

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
// アイテムの取得
// Author:後藤慎之助
//=============================================================================
ITEM*GetItem(void)
{
    return &g_aItem[0];
}

//=====================================================
// アイテムの当たり判定の設定
// Author:後藤慎之助
//=====================================================
bool HitItem(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size , bool bPlayer, ITEMTYPE type)
{
    bool bHit = false;              //当たったかどうか
    int nCntItem;					//ローカル変数

    for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
    {
        // アイテムが使われたら
        if (g_aItem[nCntItem].bUse == true)
        {
            //タイプが同じなら
            if(g_aItem[nCntItem].type == type)
            {
                //出現から、一定フレーム経過しているなら
                if (g_aItem[nCntItem].nCntVisible == 0)
                {
                    //当たり判定を計算
                    if (RectangleCollision(pPos, pPosOld, size, &g_aItem[nCntItem].pos, g_aItem[nCntItem].size) == true)
                    {
                        //当たった
                        bHit = true;

                        //プレイヤーに当たったら、未使用にして、エフェクトを設置する
                        if (bPlayer == true)
                        {
                            PlaySound(SOUND_LABEL_SE_ITEM); //アイテム取得音

                                                            //未使用に
                            g_aItem[nCntItem].bUse = false;

                            //スコアが入る
                            ScoreCalculation(GET_ITEM_SCORE);    //アイテムを取得した時のスコア

                            //エフェクト関連の変数
                            float fAngle = 0.0f;    //角度
                            float fSpeedY = 0.0f;   //速さ
                            float fRadius = GRID_SIZE / 2.0f; //エフェクト半径

                            //アイテム取得エフェクト
                            for (int nCnt = 0; nCnt < COUNT_EFFECT_ITEM; nCnt++)
                            {
                                fSpeedY = float(rand() % 800 + 300) / 100.0f;
                                //0～3.14を出す*(0 or 1)*-1で正負を決める
                                fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                                D3DXVECTOR3 pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - sinf(fAngle)*fRadius, g_aItem[nCntItem].pos.y
                                    , g_aItem[nCntItem].pos.z - cosf(fAngle)*fRadius);
                                D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
                                SetEffect(pos, move, EFFECT_SIZE_GET_ITEM, EFFECTTYPE_GET_ITEM); //アイテム取得エフェクト
                            }

                            //グリッド情報を更新
                            SetGridInfo(g_aItem[nCntItem].pos, false);

                            //初期化
                            g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                        }
                    }
                }
            }
        }
    }

    return bHit;
}


//=====================================================
// すべての種類のアイテムの当たり判定の設定
// Author:後藤慎之助
//=====================================================
bool HitAllTypeItem(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    bool bHit = false;              //当たったかどうか
    int nCntItem;					//ローカル変数

    for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
    {
        // アイテムが使われたら
        if (g_aItem[nCntItem].bUse == true)
        {
            //当たり判定を計算
            if (RectangleCollision(pPos, pPosOld, size, &g_aItem[nCntItem].pos, g_aItem[nCntItem].size) == true)
            {
                //当たった
                bHit = true;
            }           
        }
    }

    return bHit;
}