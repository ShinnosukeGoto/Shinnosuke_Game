//=============================================================================
//
// 最強昔話決定戦の処理 [block.cpp](ブロック)
// Author:後藤慎之助
//
//=============================================================================
#include "block.h"
#include "collision.h"
#include "stage.h"
#include "player.h"
#include "fire.h"
#include "item.h"
#include "effect.h"
#include "setting.h"
#include "score.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPD3DXMESH g_pMeshBlock[BLOCKTYPE_MAX] = {};                     //メッシュ情報へのポインタ
DWORD g_nNumMatBlock[BLOCKTYPE_MAX] = {};                           //マテリアル情報の数
LPD3DXBUFFER g_pBuffMatBlock[BLOCKTYPE_MAX] = {};                //マテリアル情報のポインタ

BLOCK g_aBlock[MAX_BLOCK];

//=============================================================================
// ブロック初期化処理
// Author:後藤慎之助
//=============================================================================
void InitBlock(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの読み込み

    //Xファイルの読み込み
    //壊せないブロック
    D3DXLoadMeshFromX
    (
        "MODEL/normal_block.x",         //xファイルのパス
        D3DXMESH_SYSTEMMEM, //メッシュ作成用のオプションを指定
        pDevice,            //デバイス
        NULL,               //(未使用)
        &g_pBuffMatBlock[BLOCKTYPE_000],   //マテリアル情報
        NULL,               //(未使用)
        &g_nNumMatBlock[BLOCKTYPE_000],    //Xファイルに使用されているマテリアルの数
        &g_pMeshBlock[BLOCKTYPE_000]       //xファイルのメッシュデータ
    );

    //Xファイルの読み込み
    //壊せるブロック
    D3DXLoadMeshFromX
    (
        "MODEL/broken_block.x",         //xファイルのパス
        D3DXMESH_SYSTEMMEM, //メッシュ作成用のオプションを指定
        pDevice,            //デバイス
        NULL,               //(未使用)
        &g_pBuffMatBlock[BLOCKTYPE_001],   //マテリアル情報
        NULL,               //(未使用)
        &g_nNumMatBlock[BLOCKTYPE_001],    //Xファイルに使用されているマテリアルの数
        &g_pMeshBlock[BLOCKTYPE_001]       //xファイルのメッシュデータ
    );

    //Xファイルの読み込み
    //炎が出現するブロック
    D3DXLoadMeshFromX
    (
        "MODEL/broken_block.x",         //xファイルのパス
        D3DXMESH_SYSTEMMEM, //メッシュ作成用のオプションを指定
        pDevice,            //デバイス
        NULL,               //(未使用)
        &g_pBuffMatBlock[BLOCKTYPE_002],   //マテリアル情報
        NULL,               //(未使用)
        &g_nNumMatBlock[BLOCKTYPE_002],    //Xファイルに使用されているマテリアルの数
        &g_pMeshBlock[BLOCKTYPE_002]       //xファイルのメッシュデータ
    );

    //Xファイルの読み込み
    //ボムが出現するブロック
    D3DXLoadMeshFromX
    (
        "MODEL/broken_block.x",         //xファイルのパス
        D3DXMESH_SYSTEMMEM, //メッシュ作成用のオプションを指定
        pDevice,            //デバイス
        NULL,               //(未使用)
        &g_pBuffMatBlock[BLOCKTYPE_003],   //マテリアル情報
        NULL,               //(未使用)
        &g_nNumMatBlock[BLOCKTYPE_003],    //Xファイルに使用されているマテリアルの数
        &g_pMeshBlock[BLOCKTYPE_003]       //xファイルのメッシュデータ
    );

    //Xファイルの読み込み
    //スピードが出現するブロック
    D3DXLoadMeshFromX
    (
        "MODEL/broken_block.x",         //xファイルのパス
        D3DXMESH_SYSTEMMEM, //メッシュ作成用のオプションを指定
        pDevice,            //デバイス
        NULL,               //(未使用)
        &g_pBuffMatBlock[BLOCKTYPE_004],   //マテリアル情報
        NULL,               //(未使用)
        &g_nNumMatBlock[BLOCKTYPE_004],    //Xファイルに使用されているマテリアルの数
        &g_pMeshBlock[BLOCKTYPE_004]       //xファイルのメッシュデータ
    );

    //Xファイルの読み込み
    //キックが出現するブロック
    D3DXLoadMeshFromX
    (
        "MODEL/broken_block.x",         //xファイルのパス
        D3DXMESH_SYSTEMMEM, //メッシュ作成用のオプションを指定
        pDevice,            //デバイス
        NULL,               //(未使用)
        &g_pBuffMatBlock[BLOCKTYPE_005],   //マテリアル情報
        NULL,               //(未使用)
        &g_nNumMatBlock[BLOCKTYPE_005],    //Xファイルに使用されているマテリアルの数
        &g_pMeshBlock[BLOCKTYPE_005]       //xファイルのメッシュデータ
    );

    //Xファイルの読み込み
    //プレッシャーブロック
    D3DXLoadMeshFromX
    (
        "MODEL/pressure_block.x",         //xファイルのパス
        D3DXMESH_SYSTEMMEM, //メッシュ作成用のオプションを指定
        pDevice,            //デバイス
        NULL,               //(未使用)
        &g_pBuffMatBlock[BLOCKTYPE_006],   //マテリアル情報
        NULL,               //(未使用)
        &g_nNumMatBlock[BLOCKTYPE_006],    //Xファイルに使用されているマテリアルの数
        &g_pMeshBlock[BLOCKTYPE_006]       //xファイルのメッシュデータ
    );

    //Xファイルの読み込み
    //柵ブロック
    D3DXLoadMeshFromX
    (
        "MODEL/fence_block.x",         //xファイルのパス
        D3DXMESH_SYSTEMMEM, //メッシュ作成用のオプションを指定
        pDevice,            //デバイス
        NULL,               //(未使用)
        &g_pBuffMatBlock[BLOCKTYPE_007],   //マテリアル情報
        NULL,               //(未使用)
        &g_nNumMatBlock[BLOCKTYPE_007],    //Xファイルに使用されているマテリアルの数
        &g_pMeshBlock[BLOCKTYPE_007]       //xファイルのメッシュデータ
    );

    for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBlock[nCntBlock].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBlock[nCntBlock].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBlock[nCntBlock].size = D3DXVECTOR3(GRID_SIZE, GRID_SIZE, GRID_SIZE);
        g_aBlock[nCntBlock].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        g_aBlock[nCntBlock].type = BLOCKTYPE_000;
        g_aBlock[nCntBlock].bUse = false;
    }
}

//=============================================================================
//ブロック終了処理
// Author:後藤慎之助
//=============================================================================
void UninitBlock(void)
{
    // メッシュの破棄
    for (int nCntBlock = 0; nCntBlock < BLOCKTYPE_MAX; nCntBlock++)
    {
        if (g_pMeshBlock[nCntBlock] != NULL)
        {
            g_pMeshBlock[nCntBlock]->Release();
            g_pMeshBlock[nCntBlock] = NULL;
        }
        // マテリアルの破棄
        if (g_pBuffMatBlock[nCntBlock] != NULL)
        {
            g_pBuffMatBlock[nCntBlock]->Release();
            g_pBuffMatBlock[nCntBlock] = NULL;
        }
    }
}

//=============================================================================
// ブロック更新処理
// Author:後藤慎之助
//=============================================================================
void UpdateBlock(void)
{
    SETTING *pSetting = GetSetting();

    for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        if (g_aBlock[nCntBlock].bUse == true)
        {
            //1F前の位置を記憶
            g_aBlock[nCntBlock].posOld = g_aBlock[nCntBlock].pos;

            //壊せないブロック、プレッシャーブロック、柵ブロック以外
            if (g_aBlock[nCntBlock].type != BLOCKTYPE_000 && g_aBlock[nCntBlock].type != BLOCKTYPE_006 &&g_aBlock[nCntBlock].type != BLOCKTYPE_007)
            {
                //炎に当たったら
                if (HitFire(&g_aBlock[nCntBlock].pos, &g_aBlock[nCntBlock].pos,
                    D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT * 2, GRID_SIZE)) == true)
                {
                    //30fかけて透明にする
                    g_aBlock[nCntBlock].col.a -= 0.034f;

                }
            }

            //プレッシャーブロックの移動
            if (g_aBlock[nCntBlock].type == BLOCKTYPE_006)
            {
                //重力
                g_aBlock[nCntBlock].move.y -= GRAVITY;

                //移動量と位置を紐づけ
                g_aBlock[nCntBlock].pos += g_aBlock[nCntBlock].move;

                //床に当たった
                if (g_aBlock[nCntBlock].pos.y < 0.0f)
                {
                    g_aBlock[nCntBlock].pos.y = 0.0f;
                    g_aBlock[nCntBlock].move.y = 0.0f;
                }
            }

            //柵ブロックの移動
            if (g_aBlock[nCntBlock].type == BLOCKTYPE_007)
            {
                //上に上がる
                g_aBlock[nCntBlock].move.y += GRAVITY * 2;

                //移動量と位置を紐づけ
                g_aBlock[nCntBlock].pos += g_aBlock[nCntBlock].move;

                //床に乗った
                if (g_aBlock[nCntBlock].pos.y > 0.0f)
                {
                    g_aBlock[nCntBlock].pos.y = 0.0f;
                    g_aBlock[nCntBlock].move.y = 0.0f;
                }
            }

            //透明になったら未使用に
            if (g_aBlock[nCntBlock].col.a <= 0.0f)
            {
                //未使用に
                g_aBlock[nCntBlock].bUse = false;

                //グリッド情報を更新
                SetGridInfo(g_aBlock[nCntBlock].pos, false);

                //スコアが入る
                ScoreCalculation(BREAK_BLOCK_SCORE);    //ブロックを壊した時のスコア

                //ブロックの種類ごとに、アイテムの出現
                switch (g_aBlock[nCntBlock].type)
                {
                    //炎出現
                case BLOCKTYPE_002:
                    SetItem(D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y + GRID_SIZE / 2.0f
                        , g_aBlock[nCntBlock].pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_000);
                    break;

                    //ボム出現
                case BLOCKTYPE_003:
                    SetItem(D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y + GRID_SIZE / 2.0f
                        , g_aBlock[nCntBlock].pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_001);
                    break;

                    //スピード出現
                case BLOCKTYPE_004:
                    SetItem(D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y + GRID_SIZE / 2.0f
                        , g_aBlock[nCntBlock].pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_002);
                    break;

                    //キック出現
                case BLOCKTYPE_005:
                    SetItem(D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y + GRID_SIZE / 2.0f
                        , g_aBlock[nCntBlock].pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_003);
                    break;
                }

                //初期化
                g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }

            //ブロックのクラッシュ判定 (サドンデスがあるなら、判定を使用）
            if (pSetting->bSuddenDeath == true)
            {
                if (CrushingBlock(&g_aBlock[nCntBlock].pos, &g_aBlock[nCntBlock].posOld,
                    D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT * 2, GRID_SIZE), nCntBlock) == true)
                {
                    //未使用に
                    g_aBlock[nCntBlock].bUse = false;

                    //グリッド情報を更新
                    SetGridInfo(g_aBlock[nCntBlock].pos, false);

                    //ブロック爆発エフェクト
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
                        SetEffect(g_aBlock[nCntBlock].pos, move, EFFECT_SIZE_EXPLODE_ITEM, EFFECTTYPE_EXPLOSION);
                    }

                    //初期化
                    g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                }
            }
        }
    }
}

//=============================================================================
// ブロック描画処理
// Author:後藤慎之助
//=============================================================================
void DrawBlock(void)
{

    LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの読み込み
    D3DXMATRIX mtxRot, mtxTrans, mtxscale;
    D3DMATERIAL9 matDef;//現在のマテリアル保持用
    D3DXMATERIAL *pMat;//マテリアルデータへのポインタ

    for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        //ワールドマトリックスの初期化
        D3DXMatrixIdentity(&g_aBlock[nCntBlock].mtxWorld);
        if (g_aBlock[nCntBlock].bUse)
        {
            //拡縮を反映
            D3DXMatrixScaling(&mtxscale, g_aBlock[nCntBlock].size.x / GRID_SIZE, g_aBlock[nCntBlock].size.y / GRID_SIZE, g_aBlock[nCntBlock].size.z / GRID_SIZE);
            D3DXMatrixMultiply(&g_aBlock[nCntBlock].mtxWorld, &g_aBlock[nCntBlock].mtxWorld, &mtxscale);
            //向きを反映
            D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBlock[nCntBlock].rot.y, g_aBlock[nCntBlock].rot.x, g_aBlock[nCntBlock].rot.z);
            D3DXMatrixMultiply(&g_aBlock[nCntBlock].mtxWorld, &g_aBlock[nCntBlock].mtxWorld, &mtxRot);
            //位置を反映
            D3DXMatrixTranslation(&mtxTrans, g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y, g_aBlock[nCntBlock].pos.z);
            D3DXMatrixMultiply(&g_aBlock[nCntBlock].mtxWorld, &g_aBlock[nCntBlock].mtxWorld, &mtxTrans);

            //ワールドマトリックスの設定
            pDevice->SetTransform(D3DTS_WORLD, &g_aBlock[nCntBlock].mtxWorld);

            //現在のマテリアルを取得する
            pDevice->GetMaterial(&matDef);

            //マテリアルデータのポインタを取得
            pMat = (D3DXMATERIAL*)g_pBuffMatBlock[g_aBlock[nCntBlock].type]->GetBufferPointer();
            for (int nCntMat = 0; nCntMat < (int)g_nNumMatBlock[g_aBlock[nCntBlock].type]; nCntMat++)
            {
                //マテリアルの設定
                pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

                //モデルパーツの描画
                g_pMeshBlock[g_aBlock[nCntBlock].type]->DrawSubset(nCntMat);
            }

            //保持していたマテリアルを戻す
            pDevice->SetMaterial(&matDef);
        }
    }

    //テクスチャの設定の解除
    pDevice->SetTexture(0, NULL);
}

//=============================================================================
// ブロック情報の取得
// Author:後藤慎之助
//=============================================================================
BLOCK * GetBlock(void)
{
    return &g_aBlock[0];
}

//=====================================================
// ブロックの衝突判定の設定
// Author:後藤慎之助
//=====================================================
bool CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    // 変数宣言
    bool bCollision = false;    // 衝突しているかどうか
    int nCntBlock;

    for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        // ブロックが使われたら
        if (g_aBlock[nCntBlock].bUse == true)
        {
            // 衝突判定を計算
            if (BoxCollision(pPos, pPosOld, size, &g_aBlock[nCntBlock].pos, 
                D3DXVECTOR3(g_aBlock[nCntBlock].size.x, g_aBlock[nCntBlock].size.y * 4, g_aBlock[nCntBlock].size.z)) == true)
            {
                // 衝突のフラグをtrueに
                bCollision = true;
            }
        }
    }

    return bCollision;
}

//=====================================================
// 壊れないブロックの当たり判定の設定
// Author:後藤慎之助
//=====================================================
bool HitNormalBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    // 変数宣言
    bool bHit = false;  // 当たっているかどうか
    int nCntBlock;

    for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        // ブロックが使われたら
        if (g_aBlock[nCntBlock].bUse == true)
        {
            //壊れないブロックなら
            if (g_aBlock[nCntBlock].type == BLOCKTYPE_000 || g_aBlock[nCntBlock].type == BLOCKTYPE_006 || g_aBlock[nCntBlock].type == BLOCKTYPE_007)
            {
                //当たり判定を計算
                if (RectangleCollision(pPos, pPosOld, size, &g_aBlock[nCntBlock].pos, g_aBlock[nCntBlock].size) == true)
                {
                    bHit = true;
                }
            }
        }
    }

    return bHit;
}

//=====================================================
// 壊れるブロックの当たり判定の設定
// Author:後藤慎之助
//=====================================================
bool HitBrokenBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    bool bHit = false;
    int nCntBlock;					// ローカル変数

    for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        // ブロックが使われたら
        if (g_aBlock[nCntBlock].bUse == true)
        {
            //壊れるブロックなら
            if (g_aBlock[nCntBlock].type != BLOCKTYPE_000 && g_aBlock[nCntBlock].type != BLOCKTYPE_006)
            {
                //当たり判定を計算
                if (RectangleCollision(pPos, pPosOld, size, &g_aBlock[nCntBlock].pos, g_aBlock[nCntBlock].size) == true)
                {
                    bHit = true;
                }
            }
        }
    }

    return bHit;
}

//=====================================================
// すべての種類のブロックの当たり判定の設定
// Author:後藤慎之助
//=====================================================
bool HitAllTypeBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    bool bHit = false;
    int nCntBlock;					// ローカル変数

    for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        // ブロックが使われたら
        if (g_aBlock[nCntBlock].bUse == true)
        {
            //当たり判定を計算
            if (RectangleCollision(pPos, pPosOld, size, &g_aBlock[nCntBlock].pos, g_aBlock[nCntBlock].size) == true)
            {
                bHit = true;
            }
        }
    }

    return bHit;
}

//=====================================================
// ブロックの設置
// Author:後藤慎之助
//=====================================================
void SetBlock(D3DXVECTOR3 pos, BLOCKTYPE type)
{

    int nCntBlock;				// ローカル変数

    for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        // ブロックが使われてないとき
        if (g_aBlock[nCntBlock].bUse == false)
        {
            // ブロックを使用している状態にする
            g_aBlock[nCntBlock].pos = pos;
            g_aBlock[nCntBlock].posOld = pos;
            g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            g_aBlock[nCntBlock].size = D3DXVECTOR3(GRID_SIZE, GRID_SIZE, GRID_SIZE);
            g_aBlock[nCntBlock].type = type;
            g_aBlock[nCntBlock].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
            g_aBlock[nCntBlock].bUse = true;

            //プレッシャーブロックの場合、グリッド情報を入れない
            if (g_aBlock[nCntBlock].type != BLOCKTYPE_006)
            {
                //グリッドの情報を設定
                SetGridInfo(g_aBlock[nCntBlock].pos, true);
            }

            break;
        }
    }
}

//=====================================================
// クラッシュ判定の設定
// Author:後藤慎之助
//=====================================================
bool CrushingBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size, int nNumberBlock)
{
    bool bCrush = false;       //押しつぶされたかどうか     
    int nCntBlock;

    for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        // ブロックが使われたら
        if (g_aBlock[nCntBlock].bUse == true)
        {
            //番号が違うなら
            if (nCntBlock != nNumberBlock)
            {
                //プレイヤーの位置
                D3DXVECTOR3 box1Max = D3DXVECTOR3(size.x / 2, size.y, size.z / 2) + *pPos;
                D3DXVECTOR3 box1Min = D3DXVECTOR3(-size.x / 2, -size.y, -size.z / 2) + *pPos;

                //ブロックの位置
                D3DXVECTOR3 box2Max = D3DXVECTOR3(g_aBlock[nCntBlock].size.x / 2, g_aBlock[nCntBlock].size.y,
                    g_aBlock[nCntBlock].size.z / 2) + g_aBlock[nCntBlock].pos;
                D3DXVECTOR3 box2Min = D3DXVECTOR3(-g_aBlock[nCntBlock].size.x / 2, 0.0f,
                    -g_aBlock[nCntBlock].size.z / 2) + g_aBlock[nCntBlock].pos;

                // ブロックとの当たり判定
                if ((box1Max.y > box2Min.y) &&
                    (box1Min.y < box2Max.y) &&
                    (box1Max.x > box2Min.x) &&
                    (box1Min.x < box2Max.x) &&
                    (box1Min.z < box2Max.z) &&
                    (box1Max.z > box2Min.z))
                {

                    if (box1Max.y > box2Min.y&&
                        pPosOld->y + size.y / 2 < box2Min.y)
                    {   //y軸の下
                        bCrush = true;
                    }
                }
            }
        }
    }

    return bCrush;
}